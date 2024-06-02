#include "pch.h"
#include "Player.h"
#include "Room.h"
#include "VGun.h"

Player::Player()
{
	this->stats.Init();
}

void Player::CalculateTotalStats()
{
	WRITE_LOCK;
	this->stats.Init();

	for (auto parts : this->parts.myPartsList)
	{
		this->parts.GetCalcStats(parts)(this->stats);
	}
}

void Player::RespawnInit()
{
	WRITE_LOCK;
	this->isDie = false;
	this->currentHealthPoint = this->stats.healthPoint;
	this->parts.ClearShopPartsList();
	this->CalculateTotalStats();
}

bool Player::BuyParts(uint32 selectIndex)
{
	WRITE_LOCK;

	if (this->isDie != true)
		return false;

	auto price = this->parts.GetPartsPrice(this->parts.shopPartsList[selectIndex]);

	if (this->gold < price)
		return false;

	if (this->parts.SelectParts(selectIndex) == false)
		return false;

	this->gold -= price;

	return true;
}

bool Player::RerollParts(int32 price /*= REROLL_PRICE*/)
{
	WRITE_LOCK;

	if (this->gold < price)
		return false;

	this->gold -= price;

	this->parts.GenerateShopPartsList(1);

	return true;
}

void Player::PlayerInit()
{
	// Todo parts, stats, transform
	this->stats.Init();
	this->parts.InitParts();

	this->transform.Init();

	this->gold = 0;

	this->kill = 0;
	this->death = 0;

	this->roundRank = 0;

	this->currentHealthPoint = stats.healthPoint;
	this->isDie = false;
	// Todo 죽었을때 시간 + spawnTime 더하기
	this->respawnTime = Time::GetNowTime();
	this->totalGold = 0;
}

void Player::WritePlayerId(Protocol::Player* playerId)
{
	READ_LOCK;
	playerId->set_id(this->playerId);
	playerId->set_index(this->playerIndex);
}

void Player::WritePlayerInfo(Protocol::PlayerInfo* playerInfo)
{
	READ_LOCK;
	playerInfo->set_id(this->playerId);
	playerInfo->set_index(this->playerIndex);

	{
		auto stats = playerInfo->mutable_stats();
		auto& myStats = this->stats;
		stats->set_vgun(myStats.vGun);
		stats->set_damagehead(myStats.damageHead);
		stats->set_damagebody(myStats.damageBody);
		stats->set_damageleg(myStats.damageLeg);
		stats->set_firerate(myStats.fireRate);
		stats->set_reloadtime(myStats.reloadTime);
		stats->set_movespeed(myStats.moveSpeed);
		stats->set_bulletcapacity(myStats.bulletCapacity);
		stats->set_recoil(myStats.recoil);
		stats->set_damage(myStats.damage);
		stats->set_extrajumpcount(myStats.extraJumpCount);
		stats->set_healthpoint(myStats.healthPoint);
	}

	{
		for (auto& part : this->parts.myPartsList)
		{
			//auto partsList = playerInfo.mutable_partslist();

			auto partsInfo = playerInfo->mutable_partslist()->add_parts();

			partsInfo->set_parts(part.first);
			partsInfo->set_grade(part.second);
		}
	}

	playerInfo->set_currenthealthpoint(this->currentHealthPoint);
	playerInfo->set_isdie(this->isDie);

	playerInfo->set_gold(this->gold);
	playerInfo->set_kill(this->kill);
	playerInfo->set_death(this->death);

	playerInfo->set_roundrank(this->roundRank);
	playerInfo->set_totalgold(this->totalGold);
}

