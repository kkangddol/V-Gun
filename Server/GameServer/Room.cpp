#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "RoomManager.h"
#include "ClientPacketHandler.h"
#include "ThreadManager.h"

void Room::Init()
{
	WRITE_LOCK;
	_roundKill = KILL_COUNT;
	_roundTime = GAME_TIME;

	_host = nullptr;
	_players.clear();
	_roomState = Protocol::ROOM_STATE_NONE;
	_time.Init();
	_roundState = Protocol::ROUND_STATE_NONE;

	_playersIndex.clear();
	_playersIndex.resize(5);

	_roundremainTime = Time::GetNowTime();
}

bool Room::Enter(PlayerRef player)
{
	WRITE_LOCK;

	Protocol::S_ENTER_ROOM enterRoomPkt;

	// 풀방 입장 실패
	if (_players.size() >= 4)
	{
		enterRoomPkt.set_success(false);
		enterRoomPkt.set_reason(Protocol::RoomFailReason::FULL_ROOM);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterRoomPkt);
		player->ownerGameSession->Send(sendBuffer);

		return false;
	}

	// 게임중
	if (this->_roomState != Protocol::ROOM_STATE_LOBBY)
	{
		enterRoomPkt.set_success(false);
		enterRoomPkt.set_reason(Protocol::RoomFailReason::PLAYED_ROOM);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterRoomPkt);
		player->ownerGameSession->Send(sendBuffer);

		return false;
	}

	/// 입장가능

	for (uint32 i = 1; i <= 4; i++)
	{
		if (_playersIndex[i] == nullptr)
		{
			_playersIndex[i] = player;
			player->playerIndex = i;
			break;
		}
	}

	// 들여보내고 등록하기
	_players[player->playerId] = player;
	player->_currentRoom = shared_from_this();

	// 기존 방 사람들에게 입장한 사람 보내기
	{
		Protocol::S_ANOTHER_ENTER_ROOM anotherEnterRoom;
		auto anotherPlayer = anotherEnterRoom.mutable_player();
		anotherPlayer->set_id(player->playerId);
		anotherPlayer->set_index(player->playerIndex);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(anotherEnterRoom);
		this->BroadCast(sendBuffer);
	}


	// 입장한 사람한테 기존 방 정보 보내기
	{
		enterRoomPkt.set_success(true);

		this->WriteRoomInfo(enterRoomPkt.mutable_roominfo());

		for (auto& pair : this->_players)
		{
			auto player = enterRoomPkt.add_players();
			player->set_id(pair.second->playerId);
			player->set_index(pair.second->playerIndex);
		}

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterRoomPkt);
		player->ownerGameSession->Send(sendBuffer);
	}

	return true;
}

bool Room::Leave(PlayerRef player)
{
	WRITE_LOCK;

	_playersIndex[player->playerIndex] = nullptr;
	player->playerIndex = 0;
	_players.erase(player->playerId);

	if (_players.empty() == true)
	{
		// Todo
		//GRoomManager.Push(shared_from_this());
		std::cout << "Room To Pool" << std::endl;
		return false;
	}

	std::cout << "남은 인원 : " << _players.size() << endl;

	if (player == this->_host)
	{
		this->_host = (*this->_players.begin()).second;
		_host->isHost = true;
	}

	// TODO 떠난 패킷 보내기
	{
		Protocol::S_ANOTHER_LEAVE_ROOM leaveRoom;
		leaveRoom.mutable_player()->set_id(player->playerId);
		leaveRoom.mutable_player()->set_index(player->playerIndex);
		leaveRoom.set_hostid(_host->playerId);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leaveRoom);
		this->BroadCast(sendBuffer);
	}

	player->_currentRoom = nullptr;

	return true;
}

void Room::BroadCast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (auto& p : _players)
	{
		p.second->ownerGameSession->Send(sendBuffer);
	}
}

void Room::InitGame()
{
	_roomState = Protocol::ROOM_STATE_LOBBY;
	_roundState = Protocol::ROUND_STATE_NONE;
}

void Room::WriteRoomInfo(Protocol::RoomInfo* roomInfo)
{
	READ_LOCK;
	roomInfo->set_roomcode(this->_roomCode);
	roomInfo->set_roundkill(this->_roundKill);
	roomInfo->set_roundtime(this->_roundTime);
	roomInfo->set_state(this->_roomState);
}

void Room::WriteRoundInfo(Protocol::RoundInfo* roundInfo)
{
	READ_LOCK;
	roundInfo->set_state(this->_roundState);
	roundInfo->set_time((uint32)std::chrono::duration_cast<std::chrono::seconds>(this->_roundremainTime - Time::GetNowTime()).count());
}

bool Room::UpdateSetting(uint32 roundKill, uint32 roundTime)
{
	if (roundKill < MIN_KILL_COUNT ||
		roundKill > MAX_KILL_COUNT ||
		roundTime < MIN_GAME_TIME ||
		roundTime > MAX_GAME_TIME)
	{
		// TODO 방 설정 범위 벗어남
		return false;
	}

	WRITE_LOCK;

	if (this->_roomState != Protocol::ROOM_STATE_LOBBY)
		return false;

	this->_roundKill = roundKill;
	this->_roundTime = roundTime;

	Protocol::S_ROOM_SETTING roomSettingPkt;
	this->WriteRoomInfo(roomSettingPkt.mutable_roominfo());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(roomSettingPkt);
	this->BroadCast(sendBuffer);

	return true;
}

bool Room::GameStart()
{
	WRITE_LOCK;

	if (this->_roomState != Protocol::ROOM_STATE_LOBBY)
		return false;
	if (this->_players.size() < 2)
		return false;

	this->_roomState = Protocol::ROOM_STATE_PLAYING;
	this->SetRoundState(Protocol::ROUND_STATE_LODDINGWAIT, 5);

	{
		Protocol::S_ROOM_START roomStartPkt;

		this->WriteRoomInfo(roomStartPkt.mutable_roominfo());
		this->WriteRoundInfo(roomStartPkt.mutable_roundinfo());

		for (auto& player : this->_players)
		{
			player.second->PlayerInit();

			auto playerInfo = roomStartPkt.add_players();
			player.second->WritePlayerInfo(playerInfo);
		}

		auto numbers = Random::GenerateRandomNumbers(0, (SPAWN_POINT_COUNT - 1), (uint32)this->_players.size());

		for (int i = 0; i < roomStartPkt.players().size(); i++)
		{
			roomStartPkt.add_spawnpoint(numbers[i]);
		}

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(roomStartPkt);
		this->BroadCast(sendBuffer);
	}

	GThreadManager->Launch([&]() { while (this->Update()) { this_thread::sleep_for(0ms); }});

	return true;
}

bool Room::Update()
{
	WRITE_LOCK;

	if (this->_roomState != Protocol::ROOM_STATE_PLAYING) return false;
	if (this->_roundState == Protocol::ROUND_STATE_NONE) return false;
	if (this->_time.IsAbleUpdate() == false)	return true;

	// 
	{
		// Temp
		// cout << this->_roomCode << " update" << endl;
		// Todo 죽은사람 살라기
		this->CheckRespawnPlayers();
		this->SendPlayersTransform();
	}

	// 아직 라운드 시간 안끝남
	if (Time::IsTimeOver(this->_roundremainTime) == false) return true;

	if (this->ChangeRoundState() == false) return false;

	// Temp
	// 이제 드물게 작동할 코드
	static uint32 updateCount = 1;
	std::cout << this->_roomCode << " - ChangeRoundState : " << this->_roundState << " : " << updateCount++ << std::endl;

	return true;
}

void Room::SetRoundState(Protocol::RoundState state, uint32 seconds)
{
	WRITE_LOCK;

	this->_roundState = state;
	this->_roundremainTime = Time::GetRemainingTime(seconds);
}

bool Room::ChangeRoundState()
{
	// 시간 끝났으니 상태 바꿔줘야함
	switch (this->_roundState)
	{
	case Protocol::ROUND_STATE_NONE:
		return false;

	case Protocol::ROUND_STATE_LODDINGWAIT:
		LoddingWait();
		break;

	case Protocol::ROUND_STATE_PLAYING:
		Playing();
		break;

	case Protocol::ROUND_STATE_END_GAME:
		EndGame();
		break;

	case Protocol::ROUND_STATE_RESULT:
		Result();
		break;


	default:
		return false;
	}

	return true;
}

void Room::LoddingWait()
{
	this->SetRoundState(Protocol::ROUND_STATE_PLAYING, _roundTime);

	this->SendRoundStart();
}

void Room::Playing()
{
	/// Round -> EndGame
	this->SetRoundState(Protocol::ROUND_STATE_END_GAME, 3);
	this->RankCalculation();

	this->SendEndGame();
}

void Room::EndGame()
{
	this->SetRoundState(Protocol::ROUND_STATE_RESULT, 15);

	this->SendResultStart();
}

void Room::Result()
{
	this->SetRoundState(Protocol::ROUND_STATE_NONE, 0);

	this->_roomState = Protocol::ROOM_STATE_LOBBY;

	// 끝?
}

void Room::RespawnPlayer(PlayerRef player, bool isForce)
{
	WRITE_LOCK;
	if (player->isDie == false)
		return;

	// 부활 버튼용
	if (Time::IsTimeOver(player->respawnTime) == false &&
		isForce == false)
		return;

	player->RespawnInit();
	auto spawnPointIndex = Random::GenerateRandomNumber(0, (SPAWN_POINT_COUNT - 1));

	SendPlayerSpawnPoint(player, spawnPointIndex);
}

void Room::CheckRespawnPlayers()
{
	WRITE_LOCK;

	for (auto& p : this->_players)
	{
		auto& player = p.second;

		RespawnPlayer(player);
	}
}

void Room::RewardKillGold(PlayerRef& killPlayer, PlayerRef& deathPlayer)
{
	WRITE_LOCK;
	killPlayer->gold += GOLD_KILL;
	killPlayer->totalGold += GOLD_KILL;
	deathPlayer->gold += GOLD_DEATH;
	deathPlayer->gold += GOLD_DEATH;
}

void Room::AttackPlayer(PlayerRef attackPlayer, Protocol::C_ATTACK_PLAYER& pkt)
{
	WRITE_LOCK;

	if (pkt.attackpoint() == Protocol::ATTACK_POINT_NONE)
	{
		this->SendAttackPlayer(attackPlayer, pkt.transform());
		return;
	}

	auto attackedPlayer = _players[pkt.attackedplayer().id()];

	auto totalDamage = attackPlayer->stats.damage;

	switch (pkt.attackpoint())
	{
	case Protocol::ATTACK_POINT_HEAD:
		totalDamage += attackPlayer->stats.damageHead;
		break;
	case Protocol::ATTACK_POINT_BODY:
		totalDamage += attackPlayer->stats.damageBody;
		break;
	case Protocol::ATTACK_POINT_LEG:
		totalDamage += attackPlayer->stats.damageLeg;
		break;
	default:
		break;
	}

	attackedPlayer->currentHealthPoint -= totalDamage;
	
	if (attackedPlayer->currentHealthPoint <= 0 &&
		attackedPlayer->isDie == false)
	{
		auto& deathPlayer = attackedPlayer;
		deathPlayer->isDie = true;
		deathPlayer->currentHealthPoint = 0;
		deathPlayer->respawnTime = Time::GetRemainingTime(RESPAWN_TIME);

		attackPlayer->kill += 1;
		deathPlayer->death += 1;
		this->RewardKillGold(attackPlayer, deathPlayer);

		this->SendDeathPlayer(attackPlayer, deathPlayer, pkt.transform());

		// 죽은 사람에게 공짜리롤 해서 결과 보내기
		{
			deathPlayer->RerollParts(0);

			Protocol::S_SELECT_PARTS_LIST selectPartsListPkt;

			deathPlayer->parts.WriteShopPartsList(selectPartsListPkt.mutable_partslist());

			selectPartsListPkt.set_gold(deathPlayer->gold);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(selectPartsListPkt);
			deathPlayer->ownerGameSession->Send(sendBuffer);
		}

		// 킬 수 채우면 끝나기
		if (attackPlayer->kill >= KILL_COUNT)
			this->Playing();
	}
	else
	{
		this->SendAttackPlayer(attackPlayer, attackedPlayer, pkt.transform(), pkt.attackpoint(), attackedPlayer->currentHealthPoint);
	}

}

void Room::InitRound()
{
	WRITE_LOCK;

	for (auto& p : this->_players)
	{
		auto& player = p.second;


		player->RespawnInit();
	}
}

void Room::RankCalculation()
{
	WRITE_LOCK;
	Vector<std::pair<uint64, PlayerRef>> playerRanking(_players.begin(), _players.end());

	std::sort(playerRanking.begin(), playerRanking.end(),
		[](const std::pair<uint64, PlayerRef>& a, const std::pair<uint64, PlayerRef>& b)
		{
			if (a.second->kill == b.second->kill)
			{
				if (a.second->death == b.second->death)
				{
					return a.second->gold > b.second->gold;
				}
				return a.second->death <= b.second->death;
			}
			return a.second->kill > b.second->kill;
		});

	uint32 rank = 1;
	uint32 currentKill = playerRanking[0].second->kill;

	for (int32 i = 0; i < playerRanking.size(); i++)
	{
		auto& player = playerRanking[i].second;

		player->roundRank = rank;

		// if (player->kill < currentKill)
			rank += 1;
	}
}

bool Room::SendGoldAfterBuyParts(PlayerRef player, uint32 index)
{
	WRITE_LOCK;

	Protocol::S_SELECT_PARTS pkt;

	if (player->BuyParts(index) == false)
	{
		pkt.set_success(false);
		pkt.set_gold(player->gold);
		pkt.set_index(index);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		player->ownerGameSession->Send(sendBuffer);

		return false;
	}
	else
	{
		pkt.set_success(true);
		pkt.set_gold(player->gold);
		pkt.set_index(index);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		player->ownerGameSession->Send(sendBuffer);

		return true;
	}

	return true;
}

void Room::SendPlayerSpawnPoint(PlayerRef player, uint32 spawnPointIndex)
{
	WRITE_LOCK;

	Protocol::S_SPAWN_POINT spawnPointPkt;

	player->WritePlayerId(spawnPointPkt.mutable_player());

	spawnPointPkt.set_healthpoint(player->currentHealthPoint);
	spawnPointPkt.set_spawnpointindex(spawnPointIndex);

	player->WritePlayerInfo(spawnPointPkt.mutable_playerinfo());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPointPkt);
	this->BroadCast(sendBuffer);
}

void Room::SendPlayersTransform()
{
	WRITE_LOCK;

	Protocol::S_PLAYERS_TRANSFORM playersTransformPkt;

	for (auto& p : this->_players)
	{
		auto& player = p.second;

		if (player->transform.isChangeFlag == true &&
			player->isDie == false)
		{
			auto playersTransform = playersTransformPkt.add_playerstransform();

			playersTransform->set_id(player->playerId);

			player->transform.WriteTransform(playersTransform->mutable_transform());
		}
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(playersTransformPkt);
	this->BroadCast(sendBuffer);
}

void Room::SendAttackPlayer(PlayerRef attackPlayer, PlayerRef attackedPlayer, Protocol::Transform transform, Protocol::AttackPoint attackPoint, uint32 healthPoint)
{
	WRITE_LOCK;

	Protocol::S_ATTACKED_PLAYER pkt;

	attackPlayer->WritePlayerId(pkt.mutable_attackplayer());
	attackedPlayer->WritePlayerId(pkt.mutable_attackedplayer());


	auto attackTransform = pkt.mutable_transform();
	auto position = attackTransform->mutable_position();
	auto rotation = attackTransform->mutable_quaternion();

	*position = transform.position();
	*rotation = transform.quaternion();

	pkt.set_attackpoint(attackPoint);

	pkt.set_attackedplayerhealthpoint(attackedPlayer->currentHealthPoint);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	this->BroadCast(sendBuffer);
}

void Room::SendAttackPlayer(PlayerRef attackPlayer, Protocol::Transform transform)
{
	WRITE_LOCK;

	Protocol::S_ATTACKED_PLAYER pkt;

	attackPlayer->WritePlayerId(pkt.mutable_attackplayer());

	auto attackTransform = pkt.mutable_transform();
	auto position = attackTransform->mutable_position();
	auto rotation = attackTransform->mutable_quaternion();

	*position = transform.position();
	*rotation = transform.quaternion();

	pkt.set_attackpoint(Protocol::ATTACK_POINT_NONE);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	this->BroadCast(sendBuffer);
}

void Room::SendDeathPlayer(PlayerRef attackPlayer, PlayerRef attackedPlayer, Protocol::Transform transform)
{
	WRITE_LOCK;

	Protocol::S_DEATH_PLAYER pkt;

	attackPlayer->WritePlayerId(pkt.mutable_attackplayer());
	attackedPlayer->WritePlayerId(pkt.mutable_attackedplayer());

	auto attackTransform = pkt.mutable_transform();
	auto position = attackTransform->mutable_position();
	auto rotation = attackTransform->mutable_quaternion();

	*position = transform.position();
	*rotation = transform.quaternion();

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	this->BroadCast(sendBuffer);
}

void Room::SendRoundStart()
{
	WRITE_LOCK;

	Protocol::S_ROUND_START roundStartPkt;
	this->WriteRoundInfo(roundStartPkt.mutable_roundinfo());

	for (auto& p : this->_players)
	{
		auto& player = p.second;
		player->CalculateTotalStats();
		player->currentHealthPoint = player->stats.healthPoint;

		auto playerInfo = roundStartPkt.add_players();

		player->WritePlayerInfo(playerInfo);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(roundStartPkt);
	this->BroadCast(sendBuffer);
}

void Room::SendEndGame()
{
	WRITE_LOCK;

	Protocol::S_END_GAME pkt;
	this->WriteRoundInfo(pkt.mutable_roundinfo());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	this->BroadCast(sendBuffer);
}

void Room::SendResultStart()
{
	WRITE_LOCK;

	Protocol::S_RESULT_START resultStartPkt;
	this->WriteRoundInfo(resultStartPkt.mutable_roundinfo());

	for (auto& player : this->_players)
	{
		auto playerInfo = resultStartPkt.add_players();

		player.second->WritePlayerInfo(playerInfo);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(resultStartPkt);
	this->BroadCast(sendBuffer);
}
