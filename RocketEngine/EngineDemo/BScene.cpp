#include "BScene.h"
#include "RocketAPILoader.h"

#include "SceneLoadTest.h"
#include "DebugCameraMove.h"

#include "GameManager.h"
#include "LobbyManager.h"

#include "PlayerMove.h"
#include "PlayerInfo.h"

BScene::BScene()
	: scene(),
	_gameManager(GameManager::Instance()),
	_lobbyManager(LobbyManager::Instance())
{

}

void BScene::Initialize()
{
	RocketEngine::LoadSceneFromJSON("..\\SceneData\\MainGame.json");
	RocketEngine::SetMainCamera(RocketEngine::FindObjectByName("MainCamera")->GetComponent<RocketEngine::Camera>());

	Player();
	InGameUI();
}

void BScene::MapObject()
{

}

void BScene::Player()
{
	auto player = RocketEngine::FindObjectByName("player");
	auto pCtrl = player->AddComponent<PlayerMove>();
	auto playerInfo = player->AddComponent<PlayerInfo>();
	_gameManager.AddPlayerInfo(playerInfo);

	auto playerAnimCtrl = player->AddComponent<RocketEngine::AnimationController>();

	// Animator 및 State 생성.
	auto playerAnimator = new RocketEngine::Animator();
	auto playerIdle = playerAnimator->AddNewState("myIdle");

	playerIdle->SetAnimDescName("myIdle");
	playerIdle->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_idle_ani.fbx");
	playerIdle->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_small.fbx");
	playerIdle->SetLoop(true);

	playerAnimator->SetInitState(playerIdle->GetStateName());

	// AnimationController에 Animator 부착.
	playerAnimCtrl->SetAnimator(playerAnimator);

	auto vGun = RocketEngine::FindObjectByName("VGun");
	auto tAnimController = vGun->AddComponent<RocketEngine::AnimationController>();
	auto tSkinAnimator = new RocketEngine::Animator();
	auto tTestState = tSkinAnimator->AddNewState("GunIdle");

	tTestState->SetAnimDescName("GunIdle");
	tTestState->SetAnimDescAnimPath("../3DModels/FirstPersonShooting/fps_modeling.fbx");
	tTestState->SetAnimDescOwnerPath("../3DModels/FirstPersonShooting/fps_modeling.fbx");
	tTestState->SetLoop(true);

	tSkinAnimator->SetInitState(tTestState->GetStateName());

	// AnimationController에 Animator 부착.
	tAnimController->SetAnimator(tSkinAnimator);

	auto pSound = player->AddComponent<RocketEngine::AudioClip>();
	pSound->Add3DAudio("walk01", "../Sound/Walk/footfall_01.wav", EffectSound, 0.5f, 10.0f);
	pSound->Add3DAudio("walk02", "../Sound/Walk/footfall_02.wav", EffectSound, 0.5f, 10.0f);
	pSound->Add3DAudio("jump", "../Sound/Jump&Land/jump.wav", EffectSound, 1.0f, 10.0f);
	pSound->Add3DAudio("landing", "../Sound/Jump&Land/landing.wav", EffectSound, 0.2f, 10.0f);
	pSound->Add3DAudio("shoot", "../Sound/Shoot/Gun_sound.wav", EffectSound, 0.3f, 20.0f);
	pSound->Add3DAudio("BGM", "../Sound/BGM/ingame_bgm_50_50.wav", BackGroundMusic, 1.0f, 100.0f);

	for (int i = 1; i <= 4; ++i)
	{
		auto player = RocketEngine::FindObjectByName("player" + std::to_string(i));
		auto pCtrl = player->AddComponent<PlayerMove>();
		auto playerInfo = player->AddComponent<PlayerInfo>();

		RocketEngine::AnimationController* tAnimController = player->AddComponent<RocketEngine::AnimationController>();

		// Animator 및 State 생성.
		RocketEngine::Animator* tSkinAnimator = new RocketEngine::Animator();
		RocketEngine::State* tTestState = tSkinAnimator->AddNewState("Idle");

		//tTestState->SetAnimDataName("Idle");
		tTestState->SetAnimDescName("Idle");
		tTestState->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_shoot_ani.fbx");
		tTestState->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		tTestState->SetLoop(true);

		tSkinAnimator->SetInitState(tTestState->GetStateName());

		// AnimationController에 Animator 부착.
		tAnimController->SetAnimator(tSkinAnimator);

		_gameManager.AddPlayerInfo(playerInfo);
		player->Disable();
	}
}

void BScene::InGameUI()
{
#pragma region InGameUI
	/// In Game UI
	// InGame Canvas
	RocketEngine::GameObject* inGameCanvas = RocketEngine::CreateObject("inGameCanvas");

	// 캐릭터 킬 현황1
	RocketEngine::GameObject* player1KillObj = RocketEngine::CreateTextBox("player1KillObj");
	RocketEngine::SpriteRenderer* player1KillSprite = player1KillObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* player1KillText = player1KillObj->GetComponent<RocketEngine::TextBox>();
	player1KillObj->transform.SetPosition(623.0f, 81.0f, 0.0f);
	player1KillSprite->SetPath("..\\UI\\InGame\\Character01.png");
	player1KillSprite->SetSize(150.0f, 100.0f);
	player1KillSprite->SetSortOrder(10);
	player1KillText->SetSize(150.0f, 100.0f);
	player1KillText->SetSortOrder(15);
	player1KillText->SetIgnoreFocus(true);
	player1KillText->SetText("0");

	// 캐릭터 킬 현황2
	RocketEngine::GameObject* player2KillObj = RocketEngine::CreateTextBox("player2KillObj");
	RocketEngine::SpriteRenderer* player2KillSprite = player2KillObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* player2KillText = player2KillObj->GetComponent<RocketEngine::TextBox>();
	player2KillObj->transform.SetPosition(784.0f, 81.0f, 0.0f);
	player2KillSprite->SetPath("..\\UI\\InGame\\Character02.png");
	player2KillSprite->SetSize(150.0f, 100.0f);
	player2KillSprite->SetSortOrder(10);
	player2KillText->SetSize(150.0f, 100.0f);
	player2KillText->SetSortOrder(15);
	player2KillText->SetIgnoreFocus(true);
	player2KillText->SetText("0");

	// 캐릭터 킬 현황3
	RocketEngine::GameObject* player3KillObj = RocketEngine::CreateTextBox("player3KillObj");
	RocketEngine::SpriteRenderer* player3KillSprite = player3KillObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* player3KillText = player3KillObj->GetComponent<RocketEngine::TextBox>();
	player3KillObj->transform.SetPosition(1137.0f, 81.0f, 0.0f);
	player3KillSprite->SetPath("..\\UI\\InGame\\Character03.png");
	player3KillSprite->SetSize(150.0f, 100.0f);
	player3KillSprite->SetSortOrder(10);
	player3KillText->SetSize(150.0f, 100.0f);
	player3KillText->SetSortOrder(15);
	player3KillText->SetIgnoreFocus(true);
	player3KillText->SetText("0");

	// 캐릭터 킬 현황4
	RocketEngine::GameObject* player4KillObj = RocketEngine::CreateTextBox("player4KillObj");
	RocketEngine::SpriteRenderer* player4KillSprite = player4KillObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* player4KillText = player4KillObj->GetComponent<RocketEngine::TextBox>();
	player4KillObj->transform.SetPosition(1297.0f, 81.0f, 0.0f);
	player4KillSprite->SetPath("..\\UI\\InGame\\Character04.png");
	player4KillSprite->SetSize(150.0f, 100.0f);
	player4KillSprite->SetSortOrder(10);
	player4KillText->SetSize(150.0f, 100.0f);
	player4KillText->SetSortOrder(15);
	player4KillText->SetIgnoreFocus(true);
	player4KillText->SetText("0");

	// 라운드 시간
	RocketEngine::GameObject* roundTimeObj = RocketEngine::CreateTextBox("roundTimeObj");
	RocketEngine::SpriteRenderer* roundTimeSprite = roundTimeObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* roundTimeText = roundTimeObj->GetComponent<RocketEngine::TextBox>();
	roundTimeObj->transform.SetPosition(960.0f, 81.0f, 0.0f);
	roundTimeSprite->SetPath("..\\UI\\InGame\\Clock.png");
	roundTimeSprite->SetSize(150.0f, 100.0f);
	roundTimeSprite->SetSortOrder(10);
	roundTimeText->SetSize(150.0f, 100.0f);
	roundTimeText->SetSortOrder(15);
	roundTimeText->SetIgnoreFocus(true);
	roundTimeText->SetText("60");

	// 캐릭터 초상화
	RocketEngine::GameObject* playerSpriteObj = RocketEngine::CreateImageBox("playerSpriteObj");
	RocketEngine::SpriteRenderer* playerSprite = playerSpriteObj->GetComponent<RocketEngine::SpriteRenderer>();
	playerSpriteObj->transform.SetPosition(109.0f, 940.0f, 0.0f);
	playerSprite->SetPath("..\\UI\\InGame\\CharacterImage.png");
	playerSprite->SetSize(180.0f, 180.0f);
	playerSprite->SetSortOrder(10);

	// 체력 바 테두리
	RocketEngine::GameObject* hpBarBorderObj = RocketEngine::CreateTextBox("hpBarBorderObj");
	RocketEngine::SpriteRenderer* hpBarBorderSprite = hpBarBorderObj->GetComponent<RocketEngine::SpriteRenderer>();
	hpBarBorderObj->transform.SetPosition(325.0f, 898.0f, 0.0f);
	hpBarBorderSprite->SetPath("..\\UI\\InGame\\Ex.png");
	hpBarBorderSprite->SetSize(230.0f, 60.0f);
	hpBarBorderSprite->SetSortOrder(20);

	// 체력 바 내용물
	RocketEngine::GameObject* hpBarObj = RocketEngine::CreateImageBox("hpBarObj");
	RocketEngine::SpriteRenderer* hpBarSprite = hpBarObj->GetComponent<RocketEngine::SpriteRenderer>();
	hpBarObj->transform.SetPosition(335.0f, 984.0f, 0.0f);
	hpBarSprite->SetPath("..\\UI\\InGame\\HealthBar.png");
	hpBarSprite->SetSize(330.0f, 90.0f);
	hpBarSprite->SetSortOrder(10);

	// 총알 수
	RocketEngine::GameObject* ammoObj = RocketEngine::CreateTextBox("ammoObj");
	RocketEngine::SpriteRenderer* ammoSprite = ammoObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* ammoText = ammoObj->GetComponent<RocketEngine::TextBox>();
	ammoObj->transform.SetPosition(1653.0f, 984.0f, 0.0f);
	ammoSprite->SetPath("..\\UI\\InGame\\Ammo.png");
	ammoSprite->SetSize(90.0f, 90.0f);
	ammoSprite->SetSortOrder(10);
	ammoText->SetSize(90.0f, 90.0f);
	ammoText->SetSortOrder(15);
	ammoText->SetIgnoreFocus(true);
	ammoText->SetText("10");

	// 총 초상화
	RocketEngine::GameObject* gunSpriteObj = RocketEngine::CreateImageBox("gunSpriteObj");
	RocketEngine::SpriteRenderer* gunSprite = gunSpriteObj->GetComponent<RocketEngine::SpriteRenderer>();
	gunSpriteObj->transform.SetPosition(1799.0f, 940.0f, 0.0f);
	gunSprite->SetPath("..\\UI\\InGame\\GunImage.png");
	gunSprite->SetSize(180.0f, 180.0f);
	gunSprite->SetSortOrder(10);

	// Canvas Setting
	player1KillObj->transform.SetParent(inGameCanvas);
	player2KillObj->transform.SetParent(inGameCanvas);
	player3KillObj->transform.SetParent(inGameCanvas);
	player4KillObj->transform.SetParent(inGameCanvas);
	roundTimeObj->transform.SetParent(inGameCanvas);
	playerSpriteObj->transform.SetParent(inGameCanvas);
	hpBarBorderObj->transform.SetParent(inGameCanvas);
	hpBarObj->transform.SetParent(inGameCanvas);
	ammoObj->transform.SetParent(inGameCanvas);
	gunSpriteObj->transform.SetParent(inGameCanvas);

	// Object 기본 상태 설정
	inGameCanvas->Enable();

	_gameManager.AssignInGameCanvas(inGameCanvas);
	_gameManager.AssignPlayer1KillText(player1KillText);
	_gameManager.AssignPlayer2KillText(player2KillText);
	_gameManager.AssignPlayer3KillText(player3KillText);
	_gameManager.AssignPlayer4KillText(player4KillText);
	_gameManager.AssignRoundTimeText(roundTimeText);
	_gameManager.AssignPlayerSprite(playerSprite);
	_gameManager.AssignHPBarSprite(hpBarSprite);
	_gameManager.AssignAmmoText(ammoText);
	_gameManager.AssignGunSprite(gunSprite);
	//////////////////////////////////////////////////////////////////////////

#pragma endregion
}
