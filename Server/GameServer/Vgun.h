#pragma once

class VGun
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

public:
	static void Init();

	const static VGun GetVGun(Protocol::Vgun vGun) { return vgunMap[vGun]; }

	const static VGun GetVGun(Protocol::Parts vGun, uint32 grade);
	const static VGun GetVGun(pair<Protocol::Parts, uint32> vGun);

private:
	static Map<Protocol::Vgun, class VGun> vgunMap;
	//TODO protoBuf에 쓰는 함수 만들기? 클라에도 이걸 주면 되는가?
};
