#include "pch.h"
#include "Stats.h"
#include "VGun.h"

void Stats::Init()
{
	vGun = Protocol::VGUN_NONE;

	damageHead = 0;
	damageBody = 0;
	damageLeg = 0;

	fireRate = 0;
	reloadTime = 0;

	moveSpeed = 0.0f;
	bulletCapacity = 0;
	recoil = 0;

	damage = 0;

	extraJumpCount = 0;
	healthPoint = 100;
}

Stats& Stats::operator+=(const VGun& vGun)
{
	this->vGun = vGun.vGun;

	this->damageHead += vGun.damageHead;
	this->damageBody += vGun.damageBody;
	this->damageLeg += vGun.damageLeg;

	this->fireRate += vGun.fireRate;
	this->reloadTime += vGun.reloadTime;

	this->moveSpeed += vGun.moveSpeed;
	this->bulletCapacity += vGun.bulletCapacity;
	this->recoil += vGun.recoil;

	return *this;
}
