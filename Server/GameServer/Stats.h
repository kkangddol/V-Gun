#pragma once

class VGun;

class Stats
{
public:

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

	int32 extraJumpCount;
	int32 healthPoint;

	// dummy
	uint32 BulletShotCount = 0;

	void Init();

	Stats& operator+=(const VGun& vGun);
};

