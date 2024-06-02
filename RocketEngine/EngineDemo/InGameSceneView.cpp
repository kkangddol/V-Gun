#include "InGameSceneView.h"
#include "RocketAPILoader.h"

#include <cassert>

InGameSceneView::InGameSceneView()
	: _scene()
{

}

void InGameSceneView::Initialize()
{
	_scene = RocketEngine::CreateScene("InGame");
	assert(_scene);

	// Sound 추가
	AddSound();

	// UI 관리
	InGame();

}

void InGameSceneView::InGame()
{
	/// In Game UI
	// InGame Canvas
	RocketEngine::GameObject* inGameCanvas = RocketEngine::CreateObject("inGameCanvas");

	// InGame Board 이미지
	RocketEngine::GameObject* inGameBoardImageObj = RocketEngine::CreateImageBox("inGameBoardImageObj");
	RocketEngine::SpriteRenderer* inGameBoardSprite = inGameBoardImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	inGameBoardImageObj->transform.SetPosition(959.0f, 69.5f, 0.0f);
	inGameBoardSprite->SetPath("..\\UI\\InGame\\ScoreBoard_1138x139.png");
	inGameBoardSprite->SetSize(1138.0f, 139.0f);
	inGameBoardSprite->SetSortOrder(0);

	//// 캐릭터 킬 현황1
	//RocketEngine::GameObject* playerKillInfoImageObj1 = RocketEngine::CreateImageBox("playerKillInfoImageObj1");
	//RocketEngine::SpriteRenderer* playerKillInfoSprite1 = playerKillInfoImageObj1->GetComponent<RocketEngine::SpriteRenderer>();
	//playerKillInfoImageObj1->transform.SetPosition(526.5f, 73.5f, 0.0f);
	//playerKillInfoSprite1->SetPath("..\\UI\\InGame\\Character01.png");
	//playerKillInfoSprite1->SetSize(150.0f, 100.0f);
	//playerKillInfoSprite1->SetSortOrder(10);

	//// 캐릭터 킬 현황2
	//RocketEngine::GameObject* playerKillInfoImageObj2 = RocketEngine::CreateImageBox("playerKillInfoImageObj2");
	//RocketEngine::SpriteRenderer* playerKillInfoSprite2 = playerKillInfoImageObj2->GetComponent<RocketEngine::SpriteRenderer>();
	//playerKillInfoImageObj2->transform.SetPosition(735.5f, 73.5f, 0.0f);
	//playerKillInfoSprite2->SetPath("..\\UI\\InGame\\Character02.png");
	//playerKillInfoSprite2->SetSize(150.0f, 100.0f);
	//playerKillInfoSprite2->SetSortOrder(10);

	//// 캐릭터 킬 현황3
	//RocketEngine::GameObject* playerKillInfoImageObj3 = RocketEngine::CreateImageBox("playerKillInfoImageObj3");
	//RocketEngine::SpriteRenderer* playerKillInfoSprite3 = playerKillInfoImageObj3->GetComponent<RocketEngine::SpriteRenderer>();
	//playerKillInfoImageObj3->transform.SetPosition(1190.5f, 73.5f, 0.0f);
	//playerKillInfoSprite3->SetPath("..\\UI\\InGame\\Character03.png");
	//playerKillInfoSprite3->SetSize(150.0f, 100.0f);
	//playerKillInfoSprite3->SetSortOrder(10);

	//// 캐릭터 킬 현황4
	//RocketEngine::GameObject* playerKillInfoImageObj4 = RocketEngine::CreateImageBox("playerKillInfoImageObj4");
	//RocketEngine::SpriteRenderer* playerKillInfoSprite4 = playerKillInfoImageObj4->GetComponent<RocketEngine::SpriteRenderer>();
	//playerKillInfoImageObj4->transform.SetPosition(1388.5f, 73.5f, 0.0f);
	//playerKillInfoSprite4->SetPath("..\\UI\\InGame\\Character04.png");
	//playerKillInfoSprite4->SetSize(150.0f, 100.0f);
	//playerKillInfoSprite4->SetSortOrder(10);

	//// 라운드 시간
	//RocketEngine::GameObject* roundTimeImageObj = RocketEngine::CreateImageBox("roundTimeImageObj");
	//RocketEngine::SpriteRenderer* roundTimeSprite = roundTimeImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//roundTimeImageObj->transform.SetPosition(960.0f, 81.0f, 0.0f);
	//roundTimeSprite->SetPath("..\\UI\\InGame\\Clock.png");
	//roundTimeSprite->SetSize(150.0f, 100.0f);
	//roundTimeSprite->SetSortOrder(10);

	// Player 상태창
	RocketEngine::GameObject* playerStatusPaneImageObj = RocketEngine::CreateImageBox("playerStatusPaneImageObj");
	RocketEngine::SpriteRenderer* playerStatusPaneSprite = playerStatusPaneImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	playerStatusPaneImageObj->transform.SetPosition(276.5f, 957.0f, 0.0f);
	playerStatusPaneSprite->SetPath("..\\UI\\InGame\\Character_Banana_494x178.png");
	playerStatusPaneSprite->SetSize(494.0f, 178.0f);
	playerStatusPaneSprite->SetSortOrder(10);

	//// 캐릭터 초상화
	//RocketEngine::GameObject* playerImageObj = RocketEngine::CreateImageBox("playerImageObj");
	//RocketEngine::SpriteRenderer* playerSprite = playerImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//playerImageObj->transform.SetPosition(109.0f, 940.0f, 0.0f);
	//playerSprite->SetPath("..\\UI\\InGame\\CharacterImage.png");
	//playerSprite->SetSize(180.0f, 180.0f);
	//playerSprite->SetSortOrder(10);

	//// 남은 체력 수치
	//RocketEngine::GameObject* playerHpImageObj = RocketEngine::CreateTextBox("playerHpImageObj");
	//RocketEngine::SpriteRenderer* playerHpSprite = playerHpImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//playerHpImageObj->transform.SetPosition(325.0f, 898.0f, 0.0f);
	//playerHpSprite->SetPath("..\\UI\\InGame\\Ex.png");
	//playerHpSprite->SetSize(230.0f, 60.0f);
	//playerHpSprite->SetSortOrder(20);

	// 체력 바
	RocketEngine::GameObject* playerhpBarImageObj = RocketEngine::CreateTextBox("playerhpBarImageObj");
	RocketEngine::SpriteRenderer* playerhpBarSprite = playerhpBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerhpBarText = playerhpBarImageObj->GetComponent<RocketEngine::TextBox>();
	playerhpBarImageObj->transform.SetPosition(330.0f, 980.5f, 0.0f);
	playerhpBarSprite->SetPath("..\\UI\\InGame\\HealthBar_250x45.png");
	playerhpBarSprite->SetSize(250.0f, 45.0f);
	playerhpBarSprite->SetSortOrder(10);
	playerhpBarText->SetText("100/100");
	playerhpBarText->SetFontSize(20);
	playerhpBarText->SetSize(250.0f, 45.0f);
	playerhpBarText->SetSortOrder(20);

	// 총알 수 Text
	RocketEngine::GameObject* bulletValTextObj = RocketEngine::CreateTextBox("bulletValTextObj");
	RocketEngine::TextBox* bulletValText = bulletValTextObj->GetComponent<RocketEngine::TextBox>();
	bulletValTextObj->transform.SetPosition(1625.5f, 1012.5f, 0.0f);
	bulletValText->SetText("100/100");
	bulletValText->SetFontSize(18);
	bulletValText->SetSize(230.0f, 60.0f);
	bulletValText->SetSortOrder(10);

	// 총 초상화
	RocketEngine::GameObject* bulletPortraitImageObj = RocketEngine::CreateImageBox("bulletPortraitImageObj");
	RocketEngine::SpriteRenderer* bulletPortraitSprite = bulletPortraitImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	bulletPortraitImageObj->transform.SetPosition(1793.0f, 952.5f, 0.0f);
	bulletPortraitSprite->SetPath("..\\UI\\InGame\\Gun_Watermelon_188x153.png");
	bulletPortraitSprite->SetSize(188.0f, 153.0f);
	bulletPortraitSprite->SetSortOrder(10);
	
	// 총 아래 Bar Image
	RocketEngine::GameObject* bulletUnderBarImageObj = RocketEngine::CreateImageBox("bulletUnderBarImageObj");
	RocketEngine::SpriteRenderer* bulletUnderBarSprite = bulletUnderBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	bulletUnderBarImageObj->transform.SetPosition(1712.5f, 1041.5f, 0.0f);
	bulletUnderBarSprite->SetPath("..\\UI\\InGame\\AmmoGun_UnderBar_399x9.png");
	bulletUnderBarSprite->SetSize(399.9f, 9.0f);
	bulletUnderBarSprite->SetSortOrder(10);

	// Canvas Setting
	inGameBoardImageObj->transform.SetParent(inGameCanvas);
	//playerKillInfoImageObj1->transform.SetParent(inGameCanvas);
	//playerKillInfoImageObj2->transform.SetParent(inGameCanvas);
	//playerKillInfoImageObj3->transform.SetParent(inGameCanvas);
	//playerKillInfoImageObj4->transform.SetParent(inGameCanvas);
	//roundTimeImageObj->transform.SetParent(inGameCanvas);
	playerStatusPaneImageObj->transform.SetParent(inGameCanvas);
	//playerImageObj->transform.SetParent(inGameCanvas);
	//playerHpImageObj->transform.SetParent(inGameCanvas);
	playerhpBarImageObj->transform.SetParent(inGameCanvas);
	//bulletValImageObj->transform.SetParent(inGameCanvas);
	bulletPortraitImageObj->transform.SetParent(inGameCanvas);
	bulletUnderBarImageObj->transform.SetParent(inGameCanvas);

	// Object 기본 상태 설정
	//inGameCanvas->Enable();
}

void InGameSceneView::AddSound()
{

}
