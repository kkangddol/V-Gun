#include "pch.h"
#include "Parts.h"
#include "Player.h"
#include "VGun.h"
#include "Random.h"

Map<Protocol::Parts, Map<uint32, Calc>> Parts::partsTable;
Vector<int32> Parts::partsPrice;

Parts::Parts()
{
	this->InitParts();
}

void Parts::Init()
{
	// 이것도 나중에 디비로 넣는다면 빠지지 않을까?
	partsPrice.push_back(0);
	partsPrice.push_back(30);
	partsPrice.push_back(70);
	partsPrice.push_back(100);
	{
		{
			Map<uint32, Calc> parts;
			parts[0] = [](Stats& stats) {  };
			parts[1] = [](Stats& stats) {  };
			parts[2] = [](Stats& stats) {  };
			parts[3] = [](Stats& stats) {  };
			partsTable[Protocol::PARTS_NONE] = std::move(parts);
		}
	}
	// VGun
	{
		{
			Map<uint32, Calc> parts;
			parts[1] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_CARROT_1); };
			parts[2] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_CARROT_2); };
			parts[3] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_CARROT_3); };
			partsTable[Protocol::PARTS_VGUN_CARROT] = std::move(parts);
		}

		{
			Map<uint32, Calc> parts;
			parts[1] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_WATERMELON_1); };
			parts[2] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_WATERMELON_2); };
			parts[3] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_WATERMELON_3); };
			partsTable[Protocol::PARTS_VGUN_WATERMELON] = std::move(parts);
		}

		{
			Map<uint32, Calc> parts;
			parts[1] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_KIWI_1); };
			parts[2] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_KIWI_2); };
			parts[3] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_KIWI_3); };
			partsTable[Protocol::PARTS_VGUN_KIWI] = std::move(parts);
		}

		{
			Map<uint32, Calc> parts;
			parts[1] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_BANANA_1); };
			parts[2] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_BANANA_2); };
			parts[3] = [](Stats& stats) { stats += VGun::GetVGun(Protocol::VGUN_BANANA_3); };
			partsTable[Protocol::PARTS_VGUN_BANANA] = std::move(parts);
		}

	}



	{
		Map<uint32, Calc> parts;
		parts[1] = [](Stats& stats) { stats.damage += 3; };
		parts[2] = [](Stats& stats) { stats.damage += 5; };
		parts[3] = [](Stats& stats) { stats.damage += 7; };
		partsTable[Protocol::PARTS_DAMAGE] = std::move(parts);
	}

	{
		Map<uint32, Calc> parts;
		parts[1] = [](Stats& stats) { stats.damage += 3; };
		parts[2] = [](Stats& stats) { stats.damage += 5; };
		parts[3] = [](Stats& stats) { stats.damage += 7; };
		partsTable[Protocol::PARTS_DAMAGE] = std::move(parts);
	}

	{
		Map<uint32, Calc> parts;
		parts[1] = [](Stats& stats) {stats.reloadTime -= 0.1f; };
		parts[2] = [](Stats& stats) {stats.reloadTime -= 0.2f; };
		parts[3] = [](Stats& stats) {stats.reloadTime -= 0.3f; };
		partsTable[Protocol::PARTS_RELOADTIME] = std::move(parts);
	}

	{
		Map<uint32, Calc> parts;
		parts[1] = [](Stats& stats) { stats.moveSpeed += 0.05f; };
		parts[2] = [](Stats& stats) { stats.moveSpeed += 0.07f; };
		parts[3] = [](Stats& stats) { stats.moveSpeed += 0.10f; };
		partsTable[Protocol::PARTS_MOVESPEED] = std::move(parts);
	}

	{
		Map<uint32, Calc> parts;
		parts[1] = [](Stats& stats) { stats.bulletCapacity += 3; };
		parts[2] = [](Stats& stats) { stats.bulletCapacity += 5; };
		parts[3] = [](Stats& stats) { stats.bulletCapacity += 7; };
		partsTable[Protocol::PARTS_BULLETCAPACITY] = std::move(parts);
	}

	{
		Map<uint32, Calc> parts;
		parts[1] = [](Stats& stats) { stats.healthPoint += 20; };
		parts[2] = [](Stats& stats) { stats.healthPoint += 30; };
		parts[3] = [](Stats& stats) { stats.healthPoint += 40; };
		partsTable[Protocol::PARTS_HEALTHPOINT] = std::move(parts);
	}

	{
		Map<uint32, Calc> parts;
		parts[2] = [](Stats& stats) { stats.extraJumpCount += 1; };
		partsTable[Protocol::PARTS_ISABLEDOUBLEJUMP] = std::move(parts);
	}
}

void Parts::InitParts()
{
	this->myPartsTable.clear();
	this->myVGunTable.clear();
	this->shopPartsList.clear();
	this->myPartsList.clear();

	for (auto& partsMap : this->partsTable)
	{
		// 인덱스 0번은 버리고 등급과 인덱스를 동일시함

		// VGun 넣기
		if (partsMap.first == Protocol::PARTS_VGUN_CARROT ||
			partsMap.first == Protocol::PARTS_VGUN_WATERMELON ||
			partsMap.first == Protocol::PARTS_VGUN_KIWI ||
			partsMap.first == Protocol::PARTS_VGUN_BANANA)
		{
			myVGunTable[partsMap.first] = Vector<bool>(4);

			for (auto& parts : partsMap.second)
			{
				myVGunTable[partsMap.first][parts.first] = true;
			}
		}
		// 나머지 파츠들
		else
		{
			myPartsTable[partsMap.first] = Vector<bool>(4);

			for (auto& parts : partsMap.second)
			{
				myPartsTable[partsMap.first][parts.first] = true;
			}
		}
	}

	this->SelectParts(Protocol::PARTS_VGUN_CARROT, 1);
}

bool Parts::SelectParts(Protocol::Parts parts, uint32 grade)
{
	WRITE_LOCK;
	if (parts == Protocol::PARTS_NONE || grade == 0)
		return false;

	myPartsList.push_back(std::make_pair(parts, grade));

	// 중복 비허용
	/*if (myPartsList[parts] >= grade)
		return true;

	myPartsList[parts] = grade;*/

	// 뽑은 파츠는 파츠 테이블에서 지우는 코드
	/*{
		auto& table = myPartsTable[parts];

		if (table[grade] == false)
			return false;

		partsList[parts] = grade;

		for (uint32 i = 1; i <= grade; i++)
			table[i] = false;
	}*/

	return true;
}

bool Parts::SelectParts(pair<Protocol::Parts, uint32> parts)
{
	return SelectParts(parts.first, parts.second);
}

bool Parts::SelectParts(uint32 index)
{
	if (this->SelectParts(shopPartsList[index]) == false)
		return false;

	this->shopPartsList[index].first = Protocol::PARTS_NONE;
	this->shopPartsList[index].second = 0;

	return true;
}

const Calc& Parts::GetCalcStats(Protocol::Parts parts, uint32 grade)
{
	WRITE_LOCK;

	auto& calc = partsTable[parts][grade];
	ASSERT_CRASH(calc != nullptr);

	return calc;
}

const Calc& Parts::GetCalcStats(pair<Protocol::Parts, uint32> parts)
{
	return GetCalcStats(parts.first, parts.second);
}

const int32 Parts::GetPartsPrice(uint32 grade)
{
	return partsPrice[grade];
}

const int32 Parts::GetPartsPrice(pair<Protocol::Parts, uint32> parts)
{
	return GetPartsPrice(parts.second);
}

//void Parts::WriteShopPartsList(Protocol::PartsList* partsList)
//{
//	for (auto& parts : this->shopPartsList)
//	{
//		auto partsInfo = partsList->add_parts();
//		partsInfo->set_parts(parts.first);
//		partsInfo->set_grade(parts.second);
//	}
//}

const Vector<std::pair<Protocol::Parts, uint32>>& Parts::GenerateShopPartsList(uint32 rank)
{
	WRITE_LOCK;
	this->ClearShopPartsList();

	{
		auto allParts = GenRandomAllParts(3, true);
		for (uint32 i = 0; i < allParts.size(); i++)
		{
			shopPartsList[i] = allParts[i];
		}

		// shopPartsList[2] = GenRandomPartsVGun();
	}

	// 총 포함 올랜덤
	/*{
		auto allParts = GenRandomAllParts(3,true);
		for (uint32 i = 0; i < allParts.size(); i++)
		{
			shopPartsList[i] = allParts[i];
		}
	}*/

	return this->shopPartsList;
}

void Parts::WriteShopPartsList(Protocol::PartsList* partsList)
{
	READ_LOCK;

	for (auto& parts : shopPartsList)
	{
		auto partsInfo = partsList->add_parts();
		partsInfo->set_parts(parts.first);
		partsInfo->set_grade(parts.second);
	}
}

Vector<std::pair<Protocol::Parts, uint32>> Parts::GenRandomAllParts(const uint32 count, bool isVGunInclude)
{
	Vector<std::pair<Protocol::Parts, uint32>> pool;

	if (isVGunInclude == true)
	{
		for (auto& parts : myVGunTable)
		{
			for (uint32 i = 1; i < parts.second.size(); i++)
			{
				if (parts.second[i] == true)
					pool.push_back(make_pair(parts.first, i));
			}
		}
	}

	for (auto& parts : myPartsTable)
	{
		for (uint32 i = 1; i < parts.second.size(); i++)
		{
			if (parts.second[i] == true)
				pool.push_back(make_pair(parts.first, i));
		}
	}

	Vector<std::pair<Protocol::Parts, uint32>> randomParts;

	auto index = Random::GenerateRandomNumbers(0, (uint32)pool.size() - 1, count);
	for (auto i : index)
	{
		randomParts.push_back(pool[i]);
	}

	return randomParts;
}

Vector<pair<Protocol::Parts, uint32>> Parts::GenRandomPartsGrade(const uint32 grade, const uint32 count)
{
	Vector<Protocol::Parts> pool;

	for (auto& parts : myPartsTable)
	{
		if (parts.second[grade] == true)
			pool.push_back(parts.first);
	}

	Vector<pair<Protocol::Parts, uint32>> randomParts;

	auto index = Random::GenerateRandomNumbers(0, (uint32)pool.size() - 1, count);
	for (auto i : index)
	{
		randomParts.push_back(make_pair(pool[i], grade));
	}

	return randomParts;
}

std::pair<Protocol::Parts, uint32> Parts::GenRandomPartsVGun()
{
	Vector<std::pair<Protocol::Parts, uint32>> pool;

	for (auto& parts : myVGunTable)
	{
		for (uint32 i = 1; i < parts.second.size(); i++)
		{
			if (parts.second[i] = true)
				pool.push_back(make_pair(parts.first, i));
		}
	}

	return pool[Random::GenerateRandomNumber(0, (uint32)pool.size() - 1)];
}

void Parts::ClearShopPartsList()
{
	if (shopPartsList.size() != 3)
	{
		shopPartsList.clear();
		shopPartsList.resize(3);
	}

	for (auto& parts : shopPartsList)
	{
		parts.first = Protocol::PARTS_NONE;
		parts.second = 0;
	}
}


