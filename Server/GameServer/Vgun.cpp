#include "pch.h"
#include "VGun.h"

Map<Protocol::Vgun, VGun> VGun::vgunMap;

void VGun::Init()
{
	// TODO
	// 나중에 DB에서 총 정보를 가져올 수 있지 않을까?

	// 당근
	{
		VGun vgun{ Protocol::VGUN_CARROT_1,	13,	11,	8,	0.18f,	1.3f,	1,	13,	20 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
	{
		VGun vgun{ Protocol::VGUN_CARROT_2,	15,	13,	10,	0.18f,	1.3f,	1,	13,	15 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
	{
		VGun vgun{ Protocol::VGUN_CARROT_3,	17,	15,	12,	0.18f,	1.3f,	1,	13,	13 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}

	// 수박
	{
		VGun vgun{ Protocol::VGUN_WATERMELON_1,	25,	15,	12,	0.21f,	1.8f,	0.8f,	6,	40 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
	{
		VGun vgun{ Protocol::VGUN_WATERMELON_2,	35,	25,	15,	0.21f,	1.8f,	0.8f,	6,	35 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
	{
		VGun vgun{ Protocol::VGUN_WATERMELON_3,	45,	35,	18,	0.21f,	1.8f,	0.8f,	6,	30 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}

	// 키위
	{
		VGun vgun{ Protocol::VGUN_KIWI_1,	17,	13,	10,	0.15f,	1.6f,	1.2f,	10,	25 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
	{
		VGun vgun{ Protocol::VGUN_KIWI_2,	19,	15,	12,	0.15f,	1.6f,	1.2f,	10,	20 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
	{
		VGun vgun{ Protocol::VGUN_KIWI_3,	21,	17,	15,	0.15f,	1.6f,	1.2f,	10,	15 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}

	// 바나나
	{
		VGun vgun{ Protocol::VGUN_BANANA_1,	13,	11,	8,	0.15f,	1.5f,	1,	15,	17 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
	{
		VGun vgun{ Protocol::VGUN_BANANA_2,	15,	13,	10,	0.15f,	1.5f,	1,	15,	13 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
	{
		VGun vgun{ Protocol::VGUN_BANANA_3,	17,	15,	12,	0.15f,	1.5f,	1,	15,	10 };
		VGun::vgunMap[vgun.vGun] = vgun;
	}
}

const VGun VGun::GetVGun(Protocol::Parts eVGun, uint32 grade)
{
	auto vGun = Protocol::VGUN_NONE;

	switch (eVGun)
	{
		case Protocol::PARTS_VGUN_CARROT:
			if (grade == 1)
				vGun = Protocol::VGUN_CARROT_1;
			else if (grade == 2)
				vGun = Protocol::VGUN_CARROT_2;
			else if (grade == 3)
				vGun = Protocol::VGUN_CARROT_3;
			break;

		case Protocol::PARTS_VGUN_WATERMELON:
			if (grade == 1)
				vGun = Protocol::VGUN_WATERMELON_1;
			else if (grade == 2)
				vGun = Protocol::VGUN_WATERMELON_2;
			else if (grade == 3)
				vGun = Protocol::VGUN_WATERMELON_3;
			break;
			
		case Protocol::PARTS_VGUN_KIWI:
			if (grade == 1)
				vGun = Protocol::VGUN_KIWI_1;
			else if (grade == 2)
				vGun = Protocol::VGUN_KIWI_2;
			else if (grade == 3)
				vGun = Protocol::VGUN_KIWI_3;
			break;
			
		case Protocol::PARTS_VGUN_BANANA:
			if (grade == 1)
				vGun = Protocol::VGUN_BANANA_1;
			else if (grade == 2)
				vGun = Protocol::VGUN_BANANA_2;
			else if (grade == 3)
				vGun = Protocol::VGUN_BANANA_3;
			break;

		default:
			break;
	}

	return VGun::GetVGun(vGun);
}

const VGun VGun::GetVGun(pair<Protocol::Parts, uint32> vGun)
{
	return VGun::GetVGun(vGun.first, vGun.second);
}
