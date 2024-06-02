#include "pch.h"
#include "PlayerInfo.h"
#include "LobbyManager.h"
#include "GameManager.h"
#include "PlayerMove.h"

PlayerInfo::PlayerInfo(RocketEngine::GameObject* owner)
	: RocketEngine::Component(owner)
{

}

void PlayerInfo::Start()
{
	this->Init();
}

void PlayerInfo::Update()
{
	if (!_isGoldChanging)
	{
		return;
	}

	GameManager::Instance().UpdateGoldText(std::to_string(gold));
}

void PlayerInfo::Init()
{
	// this->playerId = 0;
	// this->isHost = false;
	// this->playerIndex = 0;

	::memset(&this->stats, NULL, sizeof(this->stats));

	this->parts.clear();

	this->gold = 0;

	this->kill = 0;
	this->death = 0;

	this->currentHealthPoint = 0;
	this->isDie = false;

	_animCtrl = gameObject->GetComponent<RocketEngine::AnimationController>();

	_isGoldChanging = false;

	currentAmmoCount = 5;

	totalGold = 0;

	isDie = false;

	animState = Protocol::ANIM_IDLE;
}

void PlayerInfo::InitRound()
{
	::memset(&this->stats, NULL, sizeof(this->stats));

	this->parts.clear();

	this->gold = 0;

	this->kill = 0;
	this->death = 0;

	this->currentHealthPoint = 0;
	this->isDie = false;

	_isGoldChanging = false;

	currentAmmoCount = 5;

	totalGold = 0;

	bool isDie = false;

	animState = Protocol::ANIM_IDLE;
}

bool PlayerInfo::IsMine()
{
	return LobbyManager::Instance().GetIndex() == this->playerIndex;
}

void PlayerInfo::ReadPlayerInfo(const Protocol::PlayerInfo& playerInfo)
{
	if (this->playerId != playerInfo.id())
		return;

	if (GameManager::Instance().myId != playerInfo.id())
	{
		return;
	}

	// Todo Stats , parts 
	{
		auto& statsInfo = playerInfo.stats();

		this->stats.vGun = statsInfo.vgun();

		this->stats.damageHead = statsInfo.damagehead();
		this->stats.damageBody = statsInfo.damagebody();
		this->stats.damageLeg = statsInfo.damageleg();

		this->stats.fireRate = statsInfo.firerate();
		this->stats.reloadTime = statsInfo.reloadtime();

		this->stats.moveSpeed = statsInfo.movespeed();
		this->stats.bulletCapacity = statsInfo.bulletcapacity();
		this->stats.recoil = statsInfo.recoil();

		this->stats.damage = statsInfo.damage();

		this->stats.ableJumpCount = statsInfo.extrajumpcount();
		this->stats.healthPoint = statsInfo.healthpoint();
	}

	// Todo parts 는 보여주는 기능이 없던데
	this->currentHealthPoint = playerInfo.currenthealthpoint();
	this->isDie = playerInfo.isdie();

	this->gold = playerInfo.gold();
	this->kill = playerInfo.kill();
	this->death = playerInfo.death();

	this->totalGold = playerInfo.totalgold();

	switch (this->stats.vGun)
	{
		case Protocol::VGUN_WATERMELON_1:
		case Protocol::VGUN_WATERMELON_2:
		case Protocol::VGUN_WATERMELON_3:
			GameManager::Instance().ChangeGunSprite("..\\UI\\InGame\\Gun_Watermelon_188x153.png");
			GameManager::Instance().ChangeGunModeling(0);
			break;
		case Protocol::VGUN_KIWI_1:
		case Protocol::VGUN_KIWI_2:
		case Protocol::VGUN_KIWI_3:
			GameManager::Instance().ChangeGunSprite("..\\UI\\InGame\\Gun_Kiw_188x153.png");
			GameManager::Instance().ChangeGunModeling(1);
			break;
		case Protocol::VGUN_CARROT_1:
		case Protocol::VGUN_CARROT_2:
		case Protocol::VGUN_CARROT_3:
			GameManager::Instance().ChangeGunSprite("..\\UI\\InGame\\Gun_Carrot_188x153.png");
			GameManager::Instance().ChangeGunModeling(2);
			break;
		case Protocol::VGUN_BANANA_1:
		case Protocol::VGUN_BANANA_2:
		case Protocol::VGUN_BANANA_3:
			GameManager::Instance().ChangeGunSprite("..\\UI\\InGame\\Gun_Banana_188x153.png");
			GameManager::Instance().ChangeGunModeling(3);
			break;
		default:
			break;
	}

}

void PlayerInfo::SetAnimState(Protocol::AnimState state)
{
	if (_animCtrl == nullptr)
	{
		return;
	}

	switch (state)
	{
		case Protocol::ANIM_NONE:
			break;
		case Protocol::ANIM_IDLE:
			_animCtrl->SetCurrentState("Idle");
			break;
		case Protocol::ANIM_DIE:
			_animCtrl->SetCurrentState("Die");
			break;
		case Protocol::ANIM_JUMP:
			_animCtrl->SetCurrentState("Jump");
			break;
		case Protocol::ANIM_MOVE_FORWARD:
			_animCtrl->SetCurrentState("MoveForward");
			break;
		case Protocol::ANIM_MOVE_LEFT:
			_animCtrl->SetCurrentState("MoveLeft");
			break;
		case Protocol::ANIM_MOVE_RIGHT:
			_animCtrl->SetCurrentState("MoveRight");
			break;
		case Protocol::ANIM_RELOAD:
			_animCtrl->SetCurrentState("Reload");
			break;
		case Protocol::ANIM_SHOOT:
			_animCtrl->SetCurrentState("Shoot");
			break;
		case Protocol::AnimState_INT_MIN_SENTINEL_DO_NOT_USE_:
			break;
		case Protocol::AnimState_INT_MAX_SENTINEL_DO_NOT_USE_:
			break;
		default:
			break;
	}

	animState = state;
}

void PlayerInfo::SetCurrentHP(int newHP)
{
	currentHealthPoint = newHP;

	auto gameManager = GameManager::Instance();
	if (gameManager.myId == playerId)
	{
		gameManager.SetPlayerHpBar(newHP);
	}
}

void PlayerInfo::OnHit()
{
	if (GameManager::Instance().myId == playerId)
	{
		RocketEngine::PlayAudioOnceNested("../Sound/Hit/hit_water.wav", 1.0f);
		// 카메라 흔들기
		gameObject->GetComponent<PlayerMove>()->GetPlayerCamera()->SetIsCameraShakeOnHit(true);
	}
}

void PlayerInfo::SetCurrentGold(int g)
{
	_isGoldChanging = true;
	auto tween = RocketEngine::CreateTween();
	tween->GetData(&gold)
		.DoMove(g,1.0f)
		.Delay(0.2f)
		.SetEase(RocketEngine::LINEAR)
		.OnComplete([this]() {this->_isGoldChanging = false; });
}

int PlayerInfo::GetGunIndex()
{
	switch (this->stats.vGun)
	{
		case Protocol::VGUN_WATERMELON_1:
		case Protocol::VGUN_WATERMELON_2:
		case Protocol::VGUN_WATERMELON_3:
			return 0;
		case Protocol::VGUN_KIWI_1:
		case Protocol::VGUN_KIWI_2:
		case Protocol::VGUN_KIWI_3:
			return 1;
			break;
		case Protocol::VGUN_CARROT_1:
		case Protocol::VGUN_CARROT_2:
		case Protocol::VGUN_CARROT_3:
			return 2;
			break;
		case Protocol::VGUN_BANANA_1:
		case Protocol::VGUN_BANANA_2:
		case Protocol::VGUN_BANANA_3:
			return 3;
			break;
		default:
			return 0;
			break;
	}
}
