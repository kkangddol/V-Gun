#pragma once
#include "RocketAPILoader.h"
#include "Enum.pb.h"
#include "Struct.pb.h"
#include "Types.h"

class PlayerInfo : public RocketEngine::Component
{
public:
	PlayerInfo(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void Init();
	void InitRound();

public:
	bool IsMine();

public:
	void ReadPlayerInfo(const Protocol::PlayerInfo& playerInfo);

public:
	void SetAnimState(Protocol::AnimState state);
	void SetCurrentHP(int newHP);
	void OnHit();
	void SetCurrentGold(int g);

private:
	bool _isGoldChanging = false;

public:
	int GetGunIndex();

private:
	RocketEngine::AnimationController* _animCtrl = nullptr;

public:
	uint64 playerId = 0;
	bool isHost = false;
	uint32 playerIndex = 0;

	struct Stats
	{
		Protocol::Vgun vGun;

		uint32 damageHead;
		uint32 damageBody;
		uint32 damageLeg;

		float fireRate;
		float reloadTime;

		float moveSpeed;
		uint32 bulletCapacity;
		uint32 recoil;

		uint32 damage;

		int32 ableJumpCount;
		uint32 healthPoint;


		// dummy
		uint32 BulletShotCount = 0;
	} stats;

	std::map<Protocol::Parts, uint32> parts;

	int32 gold = 0;

	uint32 kill = 0;
	uint32 death = 0;
	uint32 currentAmmoCount = 5;

	int32 totalGold = 0;

	int32 currentHealthPoint;
	bool isDie;
	Protocol::AnimState animState;
};

