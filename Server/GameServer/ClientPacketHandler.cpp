#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"
#include "RoomManager.h"
#include "GameSessionManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 만들기
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO validation 체크

	Protocol::S_LOGIN loginPkt;

	loginPkt.set_success(true);
	// Todo
	// DB에서 유저 정보 꺼내기
	// GameSession에 저장

	// 임시 ID 발급 InGame ID
	static Atomic<uint64> idGenerator = 1;

	/*{
		auto player = loginPkt.add_players();
		player->set_name(u8"TestName2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerGameSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}*/

	auto player = loginPkt.mutable_player();
	player->set_id(idGenerator++);

	PlayerRef playerRef = MakeShared<Player>();
	playerRef->playerId = player->id();
	playerRef->ownerGameSession = gameSession;

	gameSession->_player = playerRef;

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	// Temp
	std::cout << "S_LOGIN : " << playerRef->playerId << std::endl;

	return true;
}


bool Handle_C_CREATE_ROOM(PacketSessionRef& session, Protocol::C_CREATE_ROOM& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO validation 체크

	auto room = GRoomManager.Pop();

	Protocol::S_CREATE_ROOM createRoomPkt;

	if (room == nullptr)
	{
		createRoomPkt.set_success(false);
		createRoomPkt.set_reason(Protocol::CREATE_ROOM);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(createRoomPkt);
		session->Send(sendBuffer);
		return false;
	}

	createRoomPkt.set_success(true);

	room->WriteRoomInfo(createRoomPkt.mutable_roominfo());

	gameSession->_player->isHost = true;
	room->_host = gameSession->_player;

	auto player = createRoomPkt.mutable_player();
	player->set_id(gameSession->_player->playerId);

	room->Enter(gameSession->_player);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(createRoomPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_ROOM(PacketSessionRef& session, Protocol::C_ENTER_ROOM& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	if (pkt.roomcode() < 1000 || pkt.roomcode() > 9999)
	{
		// TODO 연결 끊기
		return false;
	}

	Protocol::S_ENTER_ROOM enterRoomPkt;

	// 없는 방
	auto room = GRoomManager.GetRoom(pkt.roomcode());
	if (room == nullptr)
	{
		enterRoomPkt.set_success(false);
		enterRoomPkt.set_reason(Protocol::EXIST_ROOM);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterRoomPkt);
		session->Send(sendBuffer);
		return false;
	}

	if (room->Enter(gameSession->_player) == false)
		return false;

	return true;
}

bool Handle_C_LEAVE_ROOM(PacketSessionRef& session, Protocol::C_LEAVE_ROOM& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& player = gameSession->_player;
	GRoomManager.LeaveRoom(player);

	return true;
}

bool Handle_C_ROOM_SETTING(PacketSessionRef& session, Protocol::C_ROOM_SETTING& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& room = gameSession->_player->_currentRoom;
	auto& player = gameSession->_player;

	if (room->_host != player)
		return false; // TODO 방장 아님

	auto roomInfo = pkt.mutable_roominfo();
	//if (roomInfo->roomcode() != room->_roomCode)
	//	return false; // TODO 방번호 다름

	// 설정 범위 다름
	if (room->UpdateSetting(roomInfo->roundkill(), roomInfo->roundtime()) == false)
		return false;

	return true;
}

bool Handle_C_ROOM_START(PacketSessionRef& session, Protocol::C_ROOM_START& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& room = gameSession->_player->_currentRoom;
	auto& player = gameSession->_player;

	if (player->isHost == false)
		return false;

	if (room->_players.size() < 2)
		return false;

	return room->GameStart();
}

bool Handle_C_LOAD_COMPLETE(PacketSessionRef& session, Protocol::C_LOAD_COMPLETE& pkt)
{
	return true;
}

bool Handle_C_PLAYER_TRANSFORM(PacketSessionRef& session, Protocol::C_PLAYER_TRANSFORM& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& room = gameSession->_player->_currentRoom;
	auto& player = gameSession->_player;

	if (room->_roomState != Protocol::ROOM_STATE_PLAYING ||
		room->_roundState != Protocol::ROUND_STATE_PLAYING)
		return false;

	player->transform.ReadTransform(pkt.transform());

	return true;
}

bool Handle_C_PLAYER_ANIM_STATE(PacketSessionRef& session, Protocol::C_PLAYER_ANIM_STATE& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& room = gameSession->_player->_currentRoom;
	auto& player = gameSession->_player;

	Protocol::S_PLAYER_ANIM_STATE playerAnimState;

	playerAnimState.set_state(pkt.state());
	player->WritePlayerId(playerAnimState.mutable_player());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(playerAnimState);
	room->BroadCast(sendBuffer);

	return true;
}

bool Handle_C_SELECT_PARTS(PacketSessionRef& session, Protocol::C_SELECT_PARTS& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& room = gameSession->_player->_currentRoom;
	auto& player = gameSession->_player;

	if (room->SendGoldAfterBuyParts(player,pkt.index()) == false)
		return false;

	return true;
}

bool Handle_C_REROLL_PARTS(PacketSessionRef& session, Protocol::C_REROLL_PARTS& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& room = gameSession->_player->_currentRoom;
	auto& player = gameSession->_player;

	if (player->RerollParts() == false)
		return false;

	Protocol::S_SELECT_PARTS_LIST selectPartsListPkt;

	player->parts.WriteShopPartsList(selectPartsListPkt.mutable_partslist());
	selectPartsListPkt.set_gold(player->gold);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(selectPartsListPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_RESPAWN_CLICK(PacketSessionRef& session, Protocol::C_RESPAWN_CLICK& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& room = gameSession->_player->_currentRoom;
	auto& player = gameSession->_player;

	room->RespawnPlayer(player, true);

	return true;
}

bool Handle_C_ATTACK_PLAYER(PacketSessionRef& session, Protocol::C_ATTACK_PLAYER& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	auto& room = gameSession->_player->_currentRoom;
	auto& player = gameSession->_player;

	room->AttackPlayer(player, pkt);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	/*std::cout << pkt.msg() << std::endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());*/

	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);
	//GRoom.BroadCast(sendBuffer); // WRITE Lock

	return true;
}
