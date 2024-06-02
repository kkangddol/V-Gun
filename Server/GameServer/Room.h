#pragma once
#include "Protocol.pb.h"

class Room : public enable_shared_from_this<Room>
{
public:
	Room(int32 roomCode)
		: _roomCode(roomCode), _roundKill(KILL_COUNT), _roundTime(GAME_TIME), _roomState(Protocol::ROOM_STATE_NONE) {}

public:
	void Init();
	bool Enter(PlayerRef player);
	bool Leave(PlayerRef player);
	void BroadCast(SendBufferRef sendBuffer);

	void InitGame();
	//void SetRoomInfo()
	bool UpdateSetting(uint32 roundCount, uint32 roundTime);
	void WriteRoomInfo(Protocol::RoomInfo* roomInfo);
	void WriteRoundInfo(Protocol::RoundInfo* roundInfo);


	bool GameStart();

public:
	// 시작 후엔 이거만 돌아감
	bool Update();

public:
	void AttackPlayer(PlayerRef attackPlayer, Protocol::C_ATTACK_PLAYER& pkt);
	void RespawnPlayer(PlayerRef player, bool isForce = false);

	bool SendGoldAfterBuyParts(PlayerRef player, uint32 index);

private:
	// Round FSM
	void SetRoundState(Protocol::RoundState state, uint32 seconds);
	bool ChangeRoundState();

	void LoddingWait();
	void Playing();
	void Result();
	void EndGame();

	void CheckRespawnPlayers();
	void RewardKillGold(PlayerRef& killPlayer, PlayerRef& deathPlayer);

private:
	void InitRound(); // 라운드마다 초기화 해야 될 것들

	void RankCalculation();


	void SendPlayerSpawnPoint(PlayerRef player, uint32 spawnPointIndex);
	void SendPlayersTransform();

	void SendAttackPlayer(PlayerRef attackPlayer, Protocol::Transform transform);
	void SendAttackPlayer(PlayerRef attackPlayer, PlayerRef attackedPlayer, Protocol::Transform transform,Protocol::AttackPoint attackPoint, uint32 healthPoint);

	void SendDeathPlayer(PlayerRef attackPlayer, PlayerRef attackedPlayer, Protocol::Transform transform);

public:
	void SendRoundStart();
	void SendEndGame();
	void SendResultStart();

private:
public:
	USE_LOCK;
	const uint32 _roomCode;

	Protocol::RoomState _roomState;
	Protocol::RoundState _roundState;

	uint32 _roundKill;
	uint32 _roundTime;

	TimePoint _roundremainTime;

	Time _time;

	PlayerRef _host;
	Vector<PlayerRef> _playersIndex;
	Map<uint64, PlayerRef> _players;
};