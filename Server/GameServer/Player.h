#pragma once

#include "Stats.h"
#include "Parts.h"
#include "Transform.h"

class Room;

using RoomRef = std::shared_ptr<Room>;

class Player
{
public:
	Player();

public:
	uint64 playerId = 0;
	bool isHost = false;
	uint32 playerIndex = 0;

public:
	void CalculateTotalStats();
	void RespawnInit();
	bool BuyParts(uint32 selectIndex);
	bool RerollParts(int32 price = REROLL_PRICE);

public:
	void PlayerInit();
private:
public:
	Parts parts;

	Stats stats;

	int32 gold = 0;

	Transform transform;

	uint32 kill = 0;
	uint32 death = 0;

	uint32 roundRank = 0;

	int32 totalGold = 0;

	int32 currentHealthPoint;
	bool isDie;
	// Todo 죽었을때 시간 + spawnTime 더하기
	TimePoint respawnTime;

public:
	void WritePlayerId(Protocol::Player* playerId);
	void WritePlayerInfo(Protocol::PlayerInfo* playerInfo);

public:
	USE_LOCK;

	GameSessionRef ownerGameSession; // cycle 문제 가능성
	RoomRef _currentRoom;
};

