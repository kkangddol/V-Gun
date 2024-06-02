#include "MainMenuSceneView.h"
#include "RocketAPILoader.h"

#include <cassert>
#include <map>
#include <string>

#include <fstream>
#include <sstream>

#include "LobbyManager.h"
#include "DebugCameraMove.h"
#include "LobbyRandomAnim.h"

#pragma region CSVPath
const std::string GAPAN02FILE = "../MapCoord/CSV/Gapan02.csv";
const std::string LARGEGAPAN01FILE = "../MapCoord/CSV/LargeGapan01.csv";
const std::string BOX01FILE = "../MapCoord/CSV/Box01.csv";
const std::string BOX02FILE = "../MapCoord/CSV/Box02.csv";
const std::string OAKFILE = "../MapCoord/CSV/Oak.csv";
const std::string CARTFILE = "../MapCoord/CSV/Cart.csv";
const std::string FRUITBOX01FILE = "../MapCoord/CSV/FruitBox01.csv";
const std::string FRUITBOX02FILE = "../MapCoord/CSV/FruitBox02.csv";
const std::string WATERMELONFILE = "../MapCoord/CSV/Watermelon.csv";
const std::string KIWIFILE = "../MapCoord/CSV/Kiwi.csv";
const std::string CARROTFILE = "../MapCoord/CSV/Carrot.csv";
const std::string BANANAFILE = "../MapCoord/CSV/Banana.csv";
const std::string DESK01FILE = "../MapCoord/CSV/Desk01.csv";
const std::string DESK02FILE = "../MapCoord/CSV/Desk02.csv";
const std::string DESK03FILE = "../MapCoord/CSV/Desk03.csv";
const std::string LAMPFILE = "../MapCoord/CSV/Lamp.csv";
const std::string CHAIRFILE = "../MapCoord/CSV/Chair.csv";
const std::string WOODWALLFILE = "../MapCoord/CSV/WoodWall.csv";
const std::string BOARD01FILE = "../MapCoord/CSV/Board01.csv";
const std::string BOARD02FILE = "../MapCoord/CSV/Board02.csv";
const std::string BOARD03FILE = "../MapCoord/CSV/Board03.csv";
const std::string BOARD04FILE = "../MapCoord/CSV/Board04.csv";
const std::string BUILDING01FILE = "../MapCoord/CSV/Building01.csv";
const std::string BUILDING02FILE = "../MapCoord/CSV/Building02.csv";
const std::string BUILDING03FILE = "../MapCoord/CSV/Building03.csv";
#pragma endregion

/// 추후 비율 조정을 위한 값들
float posRatio = 0.01f;
float scaleRatio = 0.8f;
float defaultHeight = 0.0f;
RocketEngine::RMFLOAT3 tempPos = { 0.0f,0.0f,0.0f };

MainMenuSceneView::MainMenuSceneView()
	: _scene(),
	_lobbyManager(LobbyManager::Instance())
{

}

/// <summary>
/// 게임 시작 화면 View (Main Menu Scene)
/// 게임 메인 화면에 UI를 때려박는다.
/// </summary>
void MainMenuSceneView::Initialize()
{
	// 이게 있어야함
	_scene = RocketEngine::CreateScene("MainMenu");
	RocketEngine::Camera* mainCam = RocketEngine::GetMainCamera();
	mainCam->gameObject->AddComponent<DebugCameraMove>();

	assert(_scene);

	// Sound 추가 
	AddSound();

	// UI 관리
	MainMenu();
	Lobby();

	LobbyObject();
	LobbyObject2();
	LobbyPlayerSlot();
}

void MainMenuSceneView::MainMenu()
{
	/// 기본 화면
	// 게임 배경화면 (배경 이미지)
	RocketEngine::GameObject* mainCanvas = RocketEngine::CreateImageBox("mainCanvas");
	RocketEngine::SpriteRenderer* backgoundSprite = mainCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	mainCanvas->transform.SetPosition(960.0f, 540.0f, 0.0f);
	backgoundSprite->SetPath("..\\UI\\MainMenu\\Room_lobbybackground.png");
	backgoundSprite->SetSize(1920.0f, 1080.0f);
	backgoundSprite->SetSortOrder(0);

	// 방 생성 Button
	RocketEngine::GameObject* createRoomButtonObj = RocketEngine::CreateButton("createRoomButtonObj");
	RocketEngine::Button* createRoomButton = createRoomButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* createRoomButtonSprite = createRoomButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	createRoomButtonObj->transform.SetPosition(959.0f, 580.0f, 0.0f);
	createRoomButton->SetSize(356.0f, 148.0f);
	createRoomButton->SetSortOrder(40);
	createRoomButtonSprite->SetPath("..\\UI\\MainMenu\\Room_create_356x148.png");
	createRoomButtonSprite->SetSize(356.0f, 148.0f);
	createRoomButtonSprite->SetSortOrder(10);

	createRoomButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().CreateRoom();
		});

	// 방 입장 Button
	RocketEngine::GameObject* joinRoomButtonObj = RocketEngine::CreateButton("joinRoomButtonObj");
	RocketEngine::Button* joinRoomButton = joinRoomButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* joinRoomButtonSprite = joinRoomButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	joinRoomButtonObj->transform.SetPosition(959.0f, 750.0f, 0.0f);
	joinRoomButton->SetSize(355.0f, 148.0f);
	joinRoomButton->SetSortOrder(40);
	joinRoomButtonSprite->SetPath("..\\UI\\MainMenu\\Room_entrance_355x148.png");
	joinRoomButtonSprite->SetSize(355.0f, 148.0f);
	joinRoomButtonSprite->SetSortOrder(10);

	joinRoomButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().JoinRoom();
		});

	// 게임 방법 Button
	RocketEngine::GameObject* tutorialButtonObj = RocketEngine::CreateButton("tutorialButtonObj");
	RocketEngine::Button* tutorialButton = tutorialButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* tutorialButtonSprite = tutorialButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	tutorialButtonObj->transform.SetPosition(959.0f, 921.0f, 0.0f);
	tutorialButton->SetSize(356.0f, 148.0f);
	tutorialButton->SetSortOrder(40);
	tutorialButtonSprite->SetPath("..\\UI\\MainMenu\\Howtoplay_356x148.png");
	tutorialButtonSprite->SetSize(356.0f, 148.0f);
	tutorialButtonSprite->SetSortOrder(10);

	tutorialButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().Tutorial();
		});

	// 종료 Button
	RocketEngine::GameObject* exitGameButtonObj = RocketEngine::CreateButton("exitButtonObj");
	RocketEngine::Button* exitGameButton = exitGameButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* exitGameButtonSprite = exitGameButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	exitGameButtonObj->transform.SetPosition(90.0f, 987.0f, 0.0f);
	exitGameButton->SetSize(213.0f, 211.0f);
	exitGameButton->SetSortOrder(40);
	exitGameButtonSprite->SetPath("..\\UI\\MainMenu\\Exit_213x211.png");
	exitGameButtonSprite->SetSize(213.0f, 211.0f);
	exitGameButtonSprite->SetSortOrder(10);

	exitGameButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().QuitGame();
		});

	//// 설정 Button
	//RocketEngine::GameObject* settingButtonObj = RocketEngine::CreateButton("settingButtonObj");
	//RocketEngine::Button* settingButton = settingButtonObj->GetComponent<RocketEngine::Button>();
	//RocketEngine::SpriteRenderer* settingButtonSprite = settingButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	//settingButtonObj->transform.SetPosition(1855.0f, 1012.0f, 0.0f);
	//settingButton->SetSize(188.0f, 189.0f);
	//settingButton->SetSortOrder(40);
	//settingButtonSprite->SetPath("..\\UI\\WaitingRoom\\Edit_188x189.png");
	//settingButtonSprite->SetSize(188.0f, 189.0f);
	//settingButtonSprite->SetSortOrder(10);

	//settingButton->SetOnClickEvent(
	//	[]()
	//	{
	//		RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
	//		LobbyManager::Instance().Setting();
	//	});

	/// 게임 방법 창
	RocketEngine::GameObject* tutorialObj = RocketEngine::CreateButton("tutorialObj");
	RocketEngine::Button* tutorialExitButton = tutorialObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* tutorialSprite = tutorialObj->GetComponent<RocketEngine::SpriteRenderer>();
	tutorialObj->transform.SetPosition(960.0f, 540.0f, 0.0f);
	tutorialSprite->SetSize(1920.0f, 1080.0f);
	tutorialExitButton->SetSortOrder(50);
	tutorialSprite->SetPath("..\\UI\\Howtoplay\\howtoplay_game.png");
	tutorialExitButton->SetSize(1920.0f, 1080.0f);
	tutorialSprite->SetSortOrder(30);

	tutorialExitButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().ExitTutorial();
		});

	/// 방 입장
	// 방 입장 이미지
	RocketEngine::GameObject* joinRoomCanvas = RocketEngine::CreateImageBox("joinRoomCanvas");
	RocketEngine::SpriteRenderer* joinRoomCanvasSprite = joinRoomCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	joinRoomCanvas->transform.SetPosition(960.0f, 540.5f, 0.0f);
	joinRoomCanvasSprite->SetPath("..\\UI\\Entrance\\Background.png");
	joinRoomCanvasSprite->SetSize(728.0f, 365.0f);
	joinRoomCanvasSprite->SetSortOrder(51);

	// 방 입장 Close Button
	RocketEngine::GameObject* joinRoomExitButtonObj = RocketEngine::CreateButton("joinRoomExitButtonObj");
	RocketEngine::Button* joinRoomExitButton = joinRoomExitButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* joinRoomExitButtonSprite = joinRoomExitButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	joinRoomExitButtonObj->transform.SetPosition(960.5f, 778.0f, 0.0f);
	joinRoomExitButton->SetSize(187.0f, 70.0f);
	joinRoomExitButton->SetSortOrder(55);
	joinRoomExitButtonSprite->SetPath("..\\UI\\Entrance\\close.png");
	joinRoomExitButtonSprite->SetSize(187.0f, 70.0f);
	joinRoomExitButtonSprite->SetSortOrder(51);

	joinRoomExitButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().ExitJoinRoom();
		});

	// 코드 입력
	RocketEngine::GameObject* codeInputObj = RocketEngine::CreateTextBox("codeInputObj");
	RocketEngine::TextBox* codeInputTextBox = codeInputObj->GetComponent<RocketEngine::TextBox>();
	RocketEngine::SpriteRenderer* codeInputSprite = codeInputObj->GetComponent<RocketEngine::SpriteRenderer>();
	codeInputObj->transform.SetPosition(960.0f, 570.0f, 0.0f);
	codeInputTextBox->SetText("");
	codeInputTextBox->SetSize(530.0f, 90.0f);
	codeInputTextBox->SetFontSize(40);
	codeInputTextBox->SetSortOrder(53);
	codeInputSprite->SetPath("..\\UI\\Entrance\\Code.png");
	codeInputSprite->SetSize(530.0f, 90.0f);
	codeInputSprite->SetSortOrder(51);

	// 방 입장 버튼
	RocketEngine::GameObject* roomEnterButtonObj = RocketEngine::CreateButton("roomEnterButtonObj");
	RocketEngine::Button* roomEnterButton = roomEnterButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* roomEnterButtonSprite = roomEnterButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	roomEnterButtonObj->transform.SetPosition(1151.0f, 570.0f, 0.0f);
	roomEnterButton->SetSize(120.0f, 60.0f);
	roomEnterButton->SetSortOrder(55);
	roomEnterButtonSprite->SetPath("..\\UI\\Entrance\\Entrance.png");
	roomEnterButtonSprite->SetSize(120.0f, 60.0f);
	roomEnterButtonSprite->SetSortOrder(52);

	roomEnterButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().JoinWithCode();
		}
	);

	/// 게임 설정 창 (1안)

	// 설정창 이미지
	RocketEngine::GameObject* settingCanvas = RocketEngine::CreateImageBox("settingCanvas");
	RocketEngine::SpriteRenderer* settingCanvasSprite = settingCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	settingCanvas->transform.SetPosition(960.0f, 540.0f, 0.0f);
	settingCanvasSprite->SetPath("..\\UI\\Edit\\BackGround.png");
	settingCanvasSprite->SetSize(862.0f, 670.0f);
	settingCanvasSprite->SetSortOrder(20);

	// 닫기 버튼
	RocketEngine::GameObject* optionCloseButtonObj = RocketEngine::CreateButton("optionCloseButtonObj");
	RocketEngine::Button* optionCloseButton = optionCloseButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* optionCloseButtonSprite = optionCloseButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	optionCloseButtonObj->transform.SetPosition(960.5f, 930.0f, 0.0f);
	optionCloseButton->SetSize(187.0f, 70.0f);
	optionCloseButton->SetSortOrder(51);
	optionCloseButtonSprite->SetPath("..\\UI\\Edit\\Close.png");
	optionCloseButtonSprite->SetSize(187.0f, 70.0f);
	optionCloseButtonSprite->SetSortOrder(50);

	optionCloseButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().ExitSetting();
		});

	// 민감도 Down Button (에임 감도)
	RocketEngine::GameObject* aimSensitivityDownButtonObj = RocketEngine::CreateButton("aimSensitivityDownButtonObj");
	RocketEngine::Button* aimSensitivityDownButton = aimSensitivityDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* aimSensitivityDownButtonSprite = aimSensitivityDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	aimSensitivityDownButtonObj->transform.SetPosition(815.5f, 451.0f, 0.0f);
	aimSensitivityDownButton->SetSize(71.0f, 66.0f);
	aimSensitivityDownButton->SetSortOrder(50);
	aimSensitivityDownButtonSprite->SetPath("..\\UI\\Edit\\Left.png");
	aimSensitivityDownButtonSprite->SetSize(71.0f, 66.0f);
	aimSensitivityDownButtonSprite->SetSortOrder(20);

	aimSensitivityDownButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().DownSensitivity();
		});

	// 민감도 Up Button (에임 감도)
	RocketEngine::GameObject* aimSensitivityUpButtonObj = RocketEngine::CreateButton("aimSensitivityUpButtonObj");
	RocketEngine::Button* aimSensitivityUpButton = aimSensitivityUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* aimSensitivityUpButtonSprite = aimSensitivityUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	aimSensitivityUpButtonObj->transform.SetPosition(1298.5f, 451.0f, 0.0f);
	aimSensitivityUpButton->SetSize(71.0f, 66.0f);
	aimSensitivityUpButton->SetSortOrder(50);
	aimSensitivityUpButtonSprite->SetPath("..\\UI\\Edit\\Right.png");
	aimSensitivityUpButtonSprite->SetSize(71.0f, 66.0f);
	aimSensitivityUpButtonSprite->SetSortOrder(20);

	aimSensitivityUpButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().UpSensitivity();
		});

	// 민감도 Bar 이미지
	RocketEngine::GameObject* aimSensitivityBarImageObj = RocketEngine::CreateImageBox("aimSensitivityBarImageObj");
	RocketEngine::SpriteRenderer* aimSensitivityBarSprite = aimSensitivityBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	aimSensitivityBarImageObj->transform.SetPosition(1057.0f, 451.0f, 0.0f);
	aimSensitivityBarSprite->SetPath("..\\UI\\Edit\\Bar.png");
	aimSensitivityBarSprite->SetSize(396.0f, 28.0f);
	aimSensitivityBarSprite->SetSortOrder(20);

	// 민감도 슬라이드 Bar Button
	RocketEngine::GameObject* aimSensitivitySlideBarButtonObj = RocketEngine::CreateButton("aimSensitivitySlideBarButtonObj");
	RocketEngine::Button* aimSensitivitySlideBarButton = aimSensitivitySlideBarButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* aimSensitivitySlideBarButtonSprite = aimSensitivitySlideBarButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	aimSensitivitySlideBarButtonObj->transform.SetPosition(1024.0f, 450.5f, 0.0f);
	aimSensitivitySlideBarButton->SetSize(36.0f, 77.0f);
	aimSensitivitySlideBarButton->SetSortOrder(50);
	aimSensitivitySlideBarButtonSprite->SetPath("..\\UI\\Edit\\MiniBar.png");
	aimSensitivitySlideBarButtonSprite->SetSize(36.0f, 77.0f);
	aimSensitivitySlideBarButtonSprite->SetSortOrder(20);

	aimSensitivitySlideBarButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
		}
	);

	// 배경음 Down Button
	RocketEngine::GameObject* backGroundSoundDownButtonObj = RocketEngine::CreateButton("backGroundSoundDownButtonObj");
	RocketEngine::Button* backGroundSoundDownButton = backGroundSoundDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* backGroundSoundDownButtonSprite = backGroundSoundDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	backGroundSoundDownButtonObj->transform.SetPosition(815.5f, 570.0f, 0.0f);
	backGroundSoundDownButton->SetSize(71.0f, 66.0f);
	backGroundSoundDownButton->SetSortOrder(50);
	backGroundSoundDownButtonSprite->SetPath("..\\UI\\Edit\\Left.png");
	backGroundSoundDownButtonSprite->SetSize(71.0f, 66.0f);
	backGroundSoundDownButtonSprite->SetSortOrder(20);

	backGroundSoundDownButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().DownBGM();
		}
	);

	// 배경음 Up Button
	RocketEngine::GameObject* backGroundSoundUpButtonObj = RocketEngine::CreateButton("backGroundSoundUpButtonObj");
	RocketEngine::Button* backGroundSoundUpButton = backGroundSoundUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* backGroundSoundUpButtonSprite = backGroundSoundUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	backGroundSoundUpButtonObj->transform.SetPosition(1298.5f, 570.0f, 0.0f);
	backGroundSoundUpButton->SetSize(71.0f, 66.0f);
	backGroundSoundUpButton->SetSortOrder(50);
	backGroundSoundUpButtonSprite->SetPath("..\\UI\\Edit\\Right.png");
	backGroundSoundUpButtonSprite->SetSize(71.0f, 66.0f);
	backGroundSoundUpButtonSprite->SetSortOrder(20);

	backGroundSoundUpButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().UpBGM();
		}
	);

	// 배경음 Bar 이미지
	RocketEngine::GameObject* backGroundSoundBarImageObj = RocketEngine::CreateImageBox("backGroundSoundBarImageObj");
	RocketEngine::SpriteRenderer* backGroundSoundBarSprite = backGroundSoundBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	backGroundSoundBarImageObj->transform.SetPosition(1057.0f, 570.0f, 0.0f);
	backGroundSoundBarSprite->SetPath("..\\UI\\Edit\\Bar.png");
	backGroundSoundBarSprite->SetSize(396.0f, 28.0f);
	backGroundSoundBarSprite->SetSortOrder(20);

	// 배경음 슬라이드 Bar Button
	RocketEngine::GameObject* backGroundSoundSlideBarButtonObj = RocketEngine::CreateButton("backGroundSoundSlideBarButtonObj");
	RocketEngine::Button* backGroundSoundSlideBarButton = backGroundSoundSlideBarButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* backGroundSoundSlideBarButtonSprite = backGroundSoundSlideBarButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	backGroundSoundSlideBarButtonObj->transform.SetPosition(999.0f, 569.5f, 0.0f);
	backGroundSoundSlideBarButton->SetSize(36.0f, 77.0f);
	backGroundSoundSlideBarButton->SetSortOrder(50);
	backGroundSoundSlideBarButtonSprite->SetPath("..\\UI\\Edit\\MiniBar.png");
	backGroundSoundSlideBarButtonSprite->SetSize(36.0f, 77.0f);
	backGroundSoundSlideBarButtonSprite->SetSortOrder(20);

	backGroundSoundSlideBarButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
		}
	);

	// 효과음 Down Button
	RocketEngine::GameObject* effectSoundDownButtonObj = RocketEngine::CreateButton("effectSoundDownButtonObj");
	RocketEngine::Button* effectSoundDownButton = effectSoundDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* effectSoundDownButtonSprite = effectSoundDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	effectSoundDownButtonObj->transform.SetPosition(815.5f, 691.0f, 0.0f);
	effectSoundDownButton->SetSize(71.0f, 66.0f);
	effectSoundDownButton->SetSortOrder(50);
	effectSoundDownButtonSprite->SetPath("..\\UI\\Edit\\Left.png");
	effectSoundDownButtonSprite->SetSize(71.0f, 66.0f);
	effectSoundDownButtonSprite->SetSortOrder(20);

	effectSoundDownButton->SetOnClickEvent(
		[]() 
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().DownEffectSound();
		}
	);

	// 효과음 Up Button
	RocketEngine::GameObject* effectSoundUpButtonObj = RocketEngine::CreateButton("effectSoundUpButtonObj");
	RocketEngine::Button* effectSoundUpButton = effectSoundUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* effectSoundUpButtonSprite = effectSoundUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	effectSoundUpButtonObj->transform.SetPosition(1298.5f, 691.0f, 0.0f);
	effectSoundUpButtonSprite->SetPath("..\\UI\\Edit\\Right.png");
	effectSoundUpButton->SetSize(71.0f, 66.0f);
	effectSoundUpButton->SetSortOrder(50);
	effectSoundUpButtonSprite->SetSize(71.0f, 66.0f);
	effectSoundUpButtonSprite->SetSortOrder(20);

	effectSoundUpButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().UpEffectSound();
		}
	);

	// 효과음 Bar 이미지
	RocketEngine::GameObject* effectSoundBarImageObj = RocketEngine::CreateImageBox("effectSoundBarImageObj");
	RocketEngine::SpriteRenderer* effectSoundBarSprite = effectSoundBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	effectSoundBarImageObj->transform.SetPosition(1057.0f, 691.0f, 0.0f);
	effectSoundBarSprite->SetPath("..\\UI\\Edit\\Bar.png");
	effectSoundBarSprite->SetSize(396.0f, 28.0f);
	effectSoundBarSprite->SetSortOrder(20);

	// 효과음 슬라이드 Bar Button
	RocketEngine::GameObject* effectSoundSlideBarButtonObj = RocketEngine::CreateButton("effectSoundSlideBarButtonObj");
	RocketEngine::Button* effectSoundSlideBarButton = effectSoundSlideBarButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* effectSoundSlideBarButtonSprite = effectSoundSlideBarButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	effectSoundSlideBarButtonObj->transform.SetPosition(1178.5f, 691.0f, 0.0f);
	effectSoundSlideBarButton->SetSize(36.0f, 77.0f);
	effectSoundSlideBarButton->SetSortOrder(50);
	effectSoundSlideBarButtonSprite->SetPath("..\\UI\\Edit\\MiniBar.png");
	effectSoundSlideBarButtonSprite->SetSortOrder(20);
	effectSoundSlideBarButtonSprite->SetSize(36.0f, 77.0f);

	effectSoundSlideBarButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
		}
	);

	//////////////////////////////////////////////////////////////////////////

	/// Canvas 묶기
	// Main backgound (mainCanvas)
	createRoomButtonObj->transform.SetParent(mainCanvas);
	joinRoomButtonObj->transform.SetParent(mainCanvas);
	tutorialButtonObj->transform.SetParent(mainCanvas);
	exitGameButtonObj->transform.SetParent(mainCanvas);
	//settingButtonObj->transform.SetParent(mainCanvas);

	// enteringRoom Obj (부모 : joinRoomCanvas)
	joinRoomExitButtonObj->transform.SetParent(joinRoomCanvas);
	codeInputObj->transform.SetParent(joinRoomCanvas);
	roomEnterButtonObj->transform.SetParent(joinRoomCanvas);

	// GameHelp Obj
	// 한개라 제외 tutorialObj

	// Option Obj (부모 : optionCanvas)
	optionCloseButtonObj->transform.SetParent(settingCanvas);
	aimSensitivityDownButtonObj->transform.SetParent(settingCanvas);
	aimSensitivityUpButtonObj->transform.SetParent(settingCanvas);
	aimSensitivityBarImageObj->transform.SetParent(settingCanvas);
	aimSensitivitySlideBarButtonObj->transform.SetParent(settingCanvas);
	backGroundSoundDownButtonObj->transform.SetParent(settingCanvas);
	backGroundSoundUpButtonObj->transform.SetParent(settingCanvas);
	backGroundSoundBarImageObj->transform.SetParent(settingCanvas);
	backGroundSoundSlideBarButtonObj->transform.SetParent(settingCanvas);
	effectSoundDownButtonObj->transform.SetParent(settingCanvas);
	effectSoundUpButtonObj->transform.SetParent(settingCanvas);
	effectSoundBarImageObj->transform.SetParent(settingCanvas);
	effectSoundSlideBarButtonObj->transform.SetParent(settingCanvas);

	//////////////////////////////////////////////////////////////////////////

	// 메인화면에 필요없어서 일단 Disable
	settingCanvas->Disable();

	_lobbyManager = LobbyManager::Instance();
	_lobbyManager.SetMainCanvas(mainCanvas);
	_lobbyManager.SetJoinCanvas(joinRoomCanvas);
	_lobbyManager.SetTutorialCanvas(tutorialObj);
	_lobbyManager.SetSettingCanvas(settingCanvas);
	_lobbyManager.SetCodeInputTextBox(codeInputTextBox);
}

void MainMenuSceneView::Lobby()
{
	/// Game Lobby UI
	// Game Lobby Canvas
	RocketEngine::GameObject* lobbyCanvas = RocketEngine::CreateObject("lobbyCanvas");

	// 게임 시작 버튼
	RocketEngine::GameObject* gameStartButtonObj = RocketEngine::CreateButton("gameStartButtonObj");
	RocketEngine::Button* gameStartButton = gameStartButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* gameStartButtonSprite = gameStartButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	gameStartButtonObj->transform.SetPosition(1717.0f, 961.0f, 0.0f);
	gameStartButton->SetSize(438.0f, 126.0f);
	gameStartButton->SetSortOrder(20);
	gameStartButtonSprite->SetPath("..\\UI\\WaitingRoom\\Gamestart_438x126.png");
	gameStartButtonSprite->SetSize(438.0f, 126.0f);
	gameStartButtonSprite->SetSortOrder(10);

	// Lobby - 게임 시작 Button (InGame 으로 이동)
	gameStartButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().OnClickGameStartButton();
		}
	);

	// 방 코드
	RocketEngine::GameObject* roomCodeObj = RocketEngine::CreateTextBox("roomCodeObj");
	RocketEngine::TextBox* roomCodeTextBox = roomCodeObj->GetComponent<RocketEngine::TextBox>();
	RocketEngine::SpriteRenderer* roomCodeSprite = roomCodeObj->GetComponent<RocketEngine::SpriteRenderer>();
	roomCodeObj->transform.SetPosition(960.0f, 960.0f, 0.0f);
	roomCodeTextBox->SetIgnoreFocus(true);
	//roomCodeTextBox->SetSize(350.0f, 150.0f);
	//roomCodeTextBox->SetSize(177.0f, 94.0f);
	roomCodeTextBox->SetSize(248.0f, 128.0f);
	roomCodeTextBox->SetSortOrder(20);
	roomCodeTextBox->SetFontSize(40);
	//roomCodeSprite->SetPath("..\\UI\\Entrance\\Code.png");
	//roomCodeSprite->SetPath("..\\UI\\RoomEdit\\Time_Edit.png");
	roomCodeSprite->SetPath("..\\UI\\Card\\ClockBackground.png");
	//roomCodeSprite->SetSize(350.0f, 150.0f);
	//roomCodeSprite->SetSize(177.0f, 94.0f);
	roomCodeSprite->SetSize(396.0f, 166.0f);
	roomCodeSprite->SetSortOrder(10);


	// 방 설정 버튼
	RocketEngine::GameObject* roomSettingButtonObj = RocketEngine::CreateButton("roomSettingButtonObj");
	RocketEngine::Button* roomSettingButton = roomSettingButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* roomSettingButtonSprite = roomSettingButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	roomSettingButtonObj->transform.SetPosition(115.0f, 986.0f, 0.0f);
	roomSettingButton->SetSize(100.0f, 100.0f);
	roomSettingButton->SetSortOrder(20);
	roomSettingButtonSprite->SetPath("..\\UI\\WaitingRoom\\Edit_188x189.png");
	roomSettingButtonSprite->SetSize(188.0f, 187.0f);
	roomSettingButtonSprite->SetSortOrder(10);

	// Lobby - 방 설정 Button (설정 창으로 이동)
	roomSettingButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			if (LobbyManager::Instance().GetRoomSettingCanvas()->IsActive())
			{
				LobbyManager::Instance().ExitRoomSetting();
			}
			else
			{
				LobbyManager::Instance().RoomSetting();
			}
		}
	);

	// 나가기 버튼
	RocketEngine::GameObject* exitRoomButtonObj = RocketEngine::CreateButton("exitButtonObj");
	RocketEngine::Button* exitRoomButton = exitRoomButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* exitRoomButtonSprite = exitRoomButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	exitRoomButtonObj->transform.SetPosition(232.0f, 986.0f, 0.0f);
	exitRoomButton->SetSize(100.0f, 100.0f);
	exitRoomButton->SetSortOrder(20);
	exitRoomButtonSprite->SetPath("..\\UI\\WaitingRoom\\Quit_188x189.png");
	exitRoomButtonSprite->SetSize(188.0f, 189.0f);
	exitRoomButtonSprite->SetSortOrder(10);

	// Lobby - 나가기 Button (메인 메뉴로 이동)
	exitRoomButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().ExitRoom();
		}
	);

	// Canvas Setting
	gameStartButtonObj->transform.SetParent(lobbyCanvas);
	roomSettingButtonObj->transform.SetParent(lobbyCanvas);
	exitRoomButtonObj->transform.SetParent(lobbyCanvas);
	roomCodeObj->transform.SetParent(lobbyCanvas);

	// Object 기본 상태 설정
	//lobbyCanvas->Enable();

	//////////////////////////////////////////////////////////////////////////

	/// Setting UI
	// 방 설정 Image
	RocketEngine::GameObject* roomSettingCanvas = RocketEngine::CreateImageBox("roomSettingCanvas");
	RocketEngine::SpriteRenderer* roomSettingCanvasSprite = roomSettingCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	roomSettingCanvas->transform.SetPosition(960.0f, 540.0f, 0.0f);
	roomSettingCanvasSprite->SetPath("..\\UI\\RoomEdit\\BackGround.png");
	roomSettingCanvasSprite->SetSize(862.0f, 670.0f);
	roomSettingCanvasSprite->SetSortOrder(30);

	// 닫기 버튼
	RocketEngine::GameObject* roomSettingExitButtonObj = RocketEngine::CreateButton("roomSettingExitButtonObj");
	RocketEngine::Button* roomSettingExitButton = roomSettingExitButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* roomSettingExitButtonSprite = roomSettingExitButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	roomSettingExitButtonObj->transform.SetPosition(960.5f, 855.0f, 0.0f);
	roomSettingExitButton->SetSortOrder(60);
	roomSettingExitButton->SetSize(187.0f, 70.0f);
	roomSettingExitButtonSprite->SetPath("..\\UI\\RoomEdit\\close.png");
	roomSettingExitButtonSprite->SetSize(187.0f, 70.0f);
	roomSettingExitButtonSprite->SetSortOrder(40);

	// RoomSetting - 닫기 Button
	roomSettingExitButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().ExitRoomSetting();
		}
	);

	// 목표킬수 Text Box
	RocketEngine::GameObject* targetKillNumObj = RocketEngine::CreateTextBox("targetKillNumObj");
	RocketEngine::SpriteRenderer* targetKillNumSprite = targetKillNumObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* targetKillNumText = targetKillNumObj->AddComponent<RocketEngine::TextBox>();
	targetKillNumObj->transform.SetPosition(973.0f, 503.0f, 0.0f);
	targetKillNumSprite->SetPath("..\\UI\\RoomEdit\\Round_Number_70x70.png");
	targetKillNumSprite->SetSize(94.0f, 94.0f);
	targetKillNumSprite->SetSortOrder(40);
	targetKillNumText->SetFontSize(20.0f);
	targetKillNumText->SetSize(94.0f, 94.0f);
	targetKillNumText->SetSortOrder(50);
	targetKillNumText->SetText("40");
	targetKillNumText->SetIgnoreFocus(true);

	// 라운드 낮추기 버튼
	RocketEngine::GameObject* targetKillDownButtonObj = RocketEngine::CreateButton("targetKillDownButtonObj");
	RocketEngine::Button* targetKillDownButton = targetKillDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* targetKillDownButtonSprite = targetKillDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	targetKillDownButtonObj->transform.SetPosition(880.0f, 503.0f, 0.0f);
	targetKillDownButton->SetSize(71.0f, 66.0f);
	targetKillDownButton->SetSortOrder(60);
	targetKillDownButtonSprite->SetPath("..\\UI\\RoomEdit\\Round_LeftButton.png");
	targetKillDownButtonSprite->SetSize(71.0f, 66.0f);
	targetKillDownButtonSprite->SetSortOrder(40);

	// RoomSetting - Round Down Button
	targetKillDownButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().DownRoundKill();
		}
	);

	// 라운드 올리기 버튼
	RocketEngine::GameObject* targetKillUpButtonObj = RocketEngine::CreateButton("targetKillUpButtonObj");
	RocketEngine::Button* targetKillUpButton = targetKillUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* targetKillUpButtonSprite = targetKillUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	targetKillUpButtonObj->transform.SetPosition(1063.5f, 503.0f, 0.0f);	// 임시
	targetKillUpButton->SetSize(71.0f, 66.0f);
	targetKillUpButton->SetSortOrder(60);
	targetKillUpButtonSprite->SetPath("..\\UI\\RoomEdit\\Round_RightButton.png");
	targetKillUpButtonSprite->SetSize(71.0f, 66.0f);
	targetKillUpButtonSprite->SetSortOrder(40);

	// RoomSetting - Round Up Button
	targetKillUpButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().UpRoundKill();
		}
	);

	// 시간 표시 Box
	RocketEngine::GameObject* roundTimeObj = RocketEngine::CreateTextBox("roundTimeObj");
	RocketEngine::SpriteRenderer* roundTimeSprite = roundTimeObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* roundTimeText = roundTimeObj->GetComponent<RocketEngine::TextBox>();
	roundTimeObj->transform.SetPosition(965.5f, 719.0f, 0.0f);
	roundTimeSprite->SetPath("..\\UI\\RoomEdit\\Time_Edit.png");
	roundTimeSprite->SetSize(177.0f, 94.0f);
	roundTimeSprite->SetSortOrder(40);
	roundTimeText->SetFontSize(60.0f);
	roundTimeText->SetSize(177.0f, 94.0f);
	roundTimeText->SetSortOrder(50);
	roundTimeText->SetText("120");
	roundTimeText->SetIgnoreFocus(true);

	// 시간 Down 버튼
	RocketEngine::GameObject* timeDownButtonObj = RocketEngine::CreateButton("timeDownButtonObj");
	RocketEngine::Button* timeDownButton = timeDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* timeDownButtonSprite = timeDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	timeDownButtonObj->transform.SetPosition(817.5f, 715.0f, 0.0f);
	timeDownButton->SetSize(71.0f, 66.0f);
	timeDownButton->SetSortOrder(60);
	timeDownButtonSprite->SetPath("..\\UI\\RoomEdit\\Round_LeftButton.png");
	timeDownButtonSprite->SetSize(71.0f, 66.0f);
	timeDownButtonSprite->SetSortOrder(40);

	// RoomSetting - Time Down Button
	timeDownButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().DownRoundTime();
		}
	);

	// 시간 Up 버튼
	RocketEngine::GameObject* timeUpButtonObj = RocketEngine::CreateButton("timeUpButtonObj");
	RocketEngine::Button* timeUpButton = timeUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* timeUpButtonSprite = timeUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	timeUpButtonObj->transform.SetPosition(1107.5f, 715.0f, 0.0f);
	timeUpButton->SetSize(71.0f, 66.0f);
	timeUpButton->SetSortOrder(60);
	timeUpButtonSprite->SetPath("..\\UI\\RoomEdit\\Round_RightButton.png");
	timeUpButtonSprite->SetSize(71.0f, 66.0f);
	timeUpButtonSprite->SetSortOrder(40);

	// RoomSetting - Time Up Button
	timeUpButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().UpRoundTime();
		}
	);

	// Canvas Setting
	roomSettingExitButtonObj->transform.SetParent(roomSettingCanvas);
	targetKillNumObj->transform.SetParent(roomSettingCanvas);
	targetKillDownButtonObj->transform.SetParent(roomSettingCanvas);
	targetKillUpButtonObj->transform.SetParent(roomSettingCanvas);
	roundTimeObj->transform.SetParent(roomSettingCanvas);
	timeDownButtonObj->transform.SetParent(roomSettingCanvas);
	timeUpButtonObj->transform.SetParent(roomSettingCanvas);

	//////////////////////////////////////////////////////////////////////////

	_lobbyManager.SetLobbyCanvas(lobbyCanvas);
	_lobbyManager.SetRoomSettingCanvas(roomSettingCanvas);
	_lobbyManager.SetRoomCodeTextBox(roomCodeTextBox);
	_lobbyManager.SetGameStartButton(gameStartButtonObj);
	_lobbyManager.AssignKillCountTextBox(targetKillNumText);
	_lobbyManager.AssignRoundTimeTextBox(roundTimeText);
	_lobbyManager.AssignRoomSettingObj(roomSettingButtonObj);
}

void MainMenuSceneView::LobbyObject()
{
	RocketEngine::GameObject* floor_01 = RocketEngine::CreateObject("floor_01");
	tempPos = RocketEngine::RMFLOAT3{ -1500.0f, defaultHeight, -1500.0f } *posRatio;
	floor_01->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_01Renderer = floor_01->AddComponent<RocketEngine::MeshRenderer>();
	floor_01Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	//floor_01Renderer->SetShaderPath("../FX/Default/PosNormalTex.cso");
	floor_01Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	//floor_01Renderer->SetDiffusePath("../3DModels/Floor/Substance_graph_basecolor.png");
	//floor_01Renderer->SetNormalPath("../3DModels/Floor/Substance_graph_normal.png");
	floor_01Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_01Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_01Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_01Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_01Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_01Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_01Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_01Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_02 = RocketEngine::CreateObject("floor_02");
	tempPos = RocketEngine::RMFLOAT3{ -500.0f, defaultHeight, -1500.0f } *posRatio;
	floor_02->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_02Renderer = floor_02->AddComponent<RocketEngine::MeshRenderer>();
	floor_02Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	//floor_02Renderer->SetShaderPath("../FX/Default/PosNormalTex.cso");
	floor_02Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	//floor_02Renderer->SetDiffusePath("../3DModels/Floor/Substance_graph_basecolor.png");
	//floor_02Renderer->SetNormalPath("../3DModels/Floor/Substance_graph_normal.png");
	floor_02Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_02Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_02Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_02Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_02Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_02Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_02Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_02Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_03 = RocketEngine::CreateObject("floor_03");
	tempPos = RocketEngine::RMFLOAT3{ 500.0f, defaultHeight, -1500.0f } *posRatio;
	floor_03->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_03Renderer = floor_03->AddComponent<RocketEngine::MeshRenderer>();
	floor_03Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	//floor_03Renderer->SetShaderPath("../FX/Default/PosNormalTex.cso");
	floor_03Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_03Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_03Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_03Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_03Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_03Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_03Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_03Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_03Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_04 = RocketEngine::CreateObject("floor_04");
	tempPos = RocketEngine::RMFLOAT3{ 1500.0f, defaultHeight, -1500.0f } *posRatio;
	floor_04->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_04Renderer = floor_04->AddComponent<RocketEngine::MeshRenderer>();
	floor_04Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_04Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_04Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_04Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_04Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_04Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_04Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_04Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_04Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_04Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_05 = RocketEngine::CreateObject("floor_05");
	tempPos = RocketEngine::RMFLOAT3{ -1500.0f, defaultHeight, -500.0f } *posRatio;
	floor_05->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_05Renderer = floor_05->AddComponent<RocketEngine::MeshRenderer>();
	floor_05Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_05Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_05Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_05Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_05Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_05Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_05Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_05Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_05Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_05Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_06 = RocketEngine::CreateObject("floor_06");
	tempPos = RocketEngine::RMFLOAT3{ -500.0f, defaultHeight, -500.0f } *posRatio;
	floor_06->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_06Renderer = floor_06->AddComponent<RocketEngine::MeshRenderer>();
	floor_06Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_06Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_06Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_06Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_06Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_06Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_06Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_06Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_06Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_06Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_07 = RocketEngine::CreateObject("floor_07");
	tempPos = RocketEngine::RMFLOAT3{ 500.0f, defaultHeight, -500.0f } *posRatio;
	floor_07->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_07Renderer = floor_07->AddComponent<RocketEngine::MeshRenderer>();
	floor_07Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_07Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_07Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_07Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_07Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_07Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_07Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_07Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_07Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_07Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_08 = RocketEngine::CreateObject("floor_08");
	tempPos = RocketEngine::RMFLOAT3{ 1500.0f, defaultHeight, -500.0f } *posRatio;
	floor_08->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_08Renderer = floor_08->AddComponent<RocketEngine::MeshRenderer>();
	floor_08Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_08Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_08Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_08Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_08Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_08Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_08Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_08Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_08Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_08Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_09 = RocketEngine::CreateObject("floor_09");
	tempPos = RocketEngine::RMFLOAT3{ -1500.0f, defaultHeight, 500.0f } *posRatio;
	floor_09->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_09Renderer = floor_09->AddComponent<RocketEngine::MeshRenderer>();
	floor_09Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_09Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_09Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_09Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_09Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_09Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_09Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_09Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_09Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_09Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_10 = RocketEngine::CreateObject("floor_10");
	tempPos = RocketEngine::RMFLOAT3{ -500.0f, defaultHeight, 500.0f } *posRatio;
	floor_10->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_10Renderer = floor_10->AddComponent<RocketEngine::MeshRenderer>();
	floor_10Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_10Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_10Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_10Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_10Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_10Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_10Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_10Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_10Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_10Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_11 = RocketEngine::CreateObject("floor_11");
	tempPos = RocketEngine::RMFLOAT3{ 500.0f, defaultHeight, 500.0f } *posRatio;
	floor_11->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_11Renderer = floor_11->AddComponent<RocketEngine::MeshRenderer>();
	floor_11Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_11Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_11Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_11Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_11Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_11Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_11Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_11Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_11Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_11Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_12 = RocketEngine::CreateObject("floor_12");
	tempPos = RocketEngine::RMFLOAT3{ 1500.0f, defaultHeight, 500.0f } *posRatio;
	floor_12->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_12Renderer = floor_12->AddComponent<RocketEngine::MeshRenderer>();
	floor_12Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_12Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_12Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_12Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_12Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_12Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_12Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_12Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_12Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_12Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_13 = RocketEngine::CreateObject("floor_13");
	tempPos = RocketEngine::RMFLOAT3{ -1500.0f, defaultHeight, 1500.0f } *posRatio;
	floor_13->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_13Renderer = floor_13->AddComponent<RocketEngine::MeshRenderer>();
	floor_13Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_13Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_13Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_13Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_13Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_13Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_13Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_13Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_13Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_13Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_14 = RocketEngine::CreateObject("floor_14");
	tempPos = RocketEngine::RMFLOAT3{ -500.0f, defaultHeight, 1500.0f } *posRatio;
	floor_14->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_14Renderer = floor_14->AddComponent<RocketEngine::MeshRenderer>();
	floor_14Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_14Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_14Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_14Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_14Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_14Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_14Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_14Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_14Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_14Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_15 = RocketEngine::CreateObject("floor_15");
	tempPos = RocketEngine::RMFLOAT3{ 500.0f, defaultHeight, 1500.0f } *posRatio;
	floor_15->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_15Renderer = floor_15->AddComponent<RocketEngine::MeshRenderer>();
	floor_15Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_15Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_15Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_15Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_15Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_15Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_15Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_15Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_15Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_15Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	RocketEngine::GameObject* floor_16 = RocketEngine::CreateObject("floor_16");
	tempPos = RocketEngine::RMFLOAT3{ 1500.0f, defaultHeight, 1500.0f } *posRatio;
	floor_16->transform.SetLocalPosition(tempPos);
	RocketEngine::MeshRenderer* floor_16Renderer = floor_16->AddComponent<RocketEngine::MeshRenderer>();
	floor_16Renderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
	floor_16Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	floor_16Renderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
	floor_16Renderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
	floor_16Renderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
	floor_16Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_16Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	floor_16Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_16Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	floor_16Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
}

void MainMenuSceneView::LobbyObject2()
{
	/// 추후 비율 조정을 위한 값들
	float posRatio = 0.01f;
	float scaleRatio = 0.8f;
	float defaultHeight = 0.0f;
	RocketEngine::RMFLOAT3 tempPos = { 0.0f,0.0f,0.0f };

	// Floor
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			int index = i * 10 + j + 1;
			RocketEngine::GameObject* floor = RocketEngine::CreateObject("floor_" + std::to_string(index));
			tempPos = RocketEngine::RMFLOAT3{ -4500.0f + 1000.0f * i, defaultHeight, -4500.0f + 1000.0f * j } *posRatio;
			floor->transform.SetLocalPosition(tempPos.z, tempPos.y, tempPos.x);
			RocketEngine::MeshRenderer* floorRenderer = floor->AddComponent<RocketEngine::MeshRenderer>();
			floorRenderer->SetMeshPath("../3DModels/Floor/Floor.fbx");
			floorRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			floorRenderer->SetDiffusePath("../3DModels/Floor/ARM/Floor_Basecolor.png");
			floorRenderer->SetNormalPath("../3DModels/Floor/ARM/Floor_Normal.png");
			floorRenderer->SetARMPath("../3DModels/Floor/ARM/Floor_ARM.png");
			floorRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			floorRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			floorRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			floorRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			floorRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}
	}

	// Gapan02
	std::ifstream inputFileGapan02(GAPAN02FILE);

	if (inputFileGapan02.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileGapan02, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* gapan02 = RocketEngine::CreateObject("gapan02" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			gapan02->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			gapan02->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* gapan02Renderer = gapan02->AddComponent<RocketEngine::MeshRenderer>();
			gapan02Renderer->SetMeshPath("../3DModels/Gapan02/Gapan02.fbx");
			gapan02Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			gapan02Renderer->SetDiffusePath("../3DModels/Gapan02/ARM/Gapan02_Basecolor.png");
			gapan02Renderer->SetNormalPath("../3DModels/Gapan02/ARM/Gapan02_Normal.png");
			gapan02Renderer->SetARMPath("../3DModels/Gapan02/ARM/Gapan02_ARM.png");
			gapan02Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			gapan02Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			gapan02Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			gapan02Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			gapan02Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileGapan02.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// LargeGapan01
	std::ifstream inputFileLargeGapan01(LARGEGAPAN01FILE);

	if (inputFileLargeGapan01.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileLargeGapan01, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* largeGapan01 = RocketEngine::CreateObject("largeGapan01" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			largeGapan01->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			largeGapan01->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* largeGapan01Renderer = largeGapan01->AddComponent<RocketEngine::MeshRenderer>();
			largeGapan01Renderer->SetMeshPath("../3DModels/Gapan/LargeGapan01.fbx");
			largeGapan01Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			largeGapan01Renderer->SetDiffusePath("../3DModels/Gapan/ARM/Gapan_Basecolor.png");
			largeGapan01Renderer->SetNormalPath("../3DModels/Gapan/ARM/Gapan_Normal.png");
			largeGapan01Renderer->SetARMPath("../3DModels/Gapan/ARM/Gapan_ARM.png");
			largeGapan01Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			largeGapan01Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			largeGapan01Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			largeGapan01Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			largeGapan01Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileLargeGapan01.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Box01

	std::ifstream inputFileBox01(BOX01FILE);

	if (inputFileBox01.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBox01, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* box01 = RocketEngine::CreateObject("box01" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			box01->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			box01->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* box01Renderer = box01->AddComponent<RocketEngine::MeshRenderer>();
			box01Renderer->SetMeshPath("../3DModels/Box/Box01.fbx");
			box01Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			box01Renderer->SetDiffusePath("../3DModels/Box/Box0102_BaseColor.png");
			box01Renderer->SetNormalPath("../3DModels/Box/Box0102_Normal.png");
			box01Renderer->SetARMPath("../3DModels/Box/Box0102_ARM.png");
			box01Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			box01Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			box01Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			box01Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			box01Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBox01.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Box02
	std::ifstream inputFileBox02(BOX02FILE);

	if (inputFileBox02.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBox02, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* box02 = RocketEngine::CreateObject("box02" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			box02->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			box02->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* box02Renderer = box02->AddComponent<RocketEngine::MeshRenderer>();
			box02Renderer->SetMeshPath("../3DModels/Box/Box02.fbx");
			box02Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			box02Renderer->SetDiffusePath("../3DModels/Box/Box0102_BaseColor.png");
			box02Renderer->SetNormalPath("../3DModels/Box/Box0102_Normal.png");
			box02Renderer->SetARMPath("../3DModels/Box/Box0102_ARM.png");
			box02Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			box02Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			box02Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			box02Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			box02Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBox02.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Oak
	std::ifstream inputFileOak(OAKFILE);

	if (inputFileOak.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileOak, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* oak = RocketEngine::CreateObject("oak" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			oak->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			oak->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* oakRenderer = oak->AddComponent<RocketEngine::MeshRenderer>();
			oakRenderer->SetMeshPath("../3DModels/Oak&Cart/Oak.fbx");
			oakRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			oakRenderer->SetDiffusePath("../3DModels/Oak&Cart/OakCart_BaseColor.png");
			oakRenderer->SetNormalPath("../3DModels/Oak&Cart/OakCart_Normal.png");
			oakRenderer->SetARMPath("../3DModels/Oak&Cart/OakCart_ARM.png");
			oakRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			oakRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			oakRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			oakRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			oakRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileOak.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Cart
	std::ifstream inputFileCart(CARTFILE);

	if (inputFileCart.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileCart, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* cart = RocketEngine::CreateObject("cart" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			cart->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			cart->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* cartRenderer = cart->AddComponent<RocketEngine::MeshRenderer>();
			cartRenderer->SetMeshPath("../3DModels/Oak&Cart/Cart.fbx");
			cartRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			cartRenderer->SetDiffusePath("../3DModels/Oak&Cart/OakCart_BaseColor.png");
			cartRenderer->SetNormalPath("../3DModels/Oak&Cart/OakCart_Normal.png");
			cartRenderer->SetARMPath("../3DModels/Oak&Cart/OakCart_ARM.png");
			cartRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			cartRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			cartRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			cartRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			cartRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileCart.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// FruitBox01
	std::ifstream inputFileFruitBox01(FRUITBOX01FILE);

	if (inputFileFruitBox01.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileFruitBox01, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* fruitBox01 = RocketEngine::CreateObject("fruitBox01" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			fruitBox01->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			fruitBox01->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* fruitBox01Renderer = fruitBox01->AddComponent<RocketEngine::MeshRenderer>();
			fruitBox01Renderer->SetMeshPath("../3DModels/FruitBox/FruitBox01.fbx");
			fruitBox01Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			fruitBox01Renderer->SetDiffusePath("../3DModels/FruitBox/ARM/FruitBox_BaseColor.png");
			fruitBox01Renderer->SetNormalPath("../3DModels/FruitBox/ARM/FruitBox_Normal.png");
			fruitBox01Renderer->SetARMPath("../3DModels/FruitBox/ARM/FruitBox_ARM.png");
			fruitBox01Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			fruitBox01Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			fruitBox01Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			fruitBox01Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			fruitBox01Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileFruitBox01.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// FruitBox02
	std::ifstream inputFileFruitBox02(FRUITBOX02FILE);

	if (inputFileFruitBox02.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileFruitBox02, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* fruitBox02 = RocketEngine::CreateObject("fruitBox02" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			fruitBox02->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			fruitBox02->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* fruitBox02Renderer = fruitBox02->AddComponent<RocketEngine::MeshRenderer>();
			fruitBox02Renderer->SetMeshPath("../3DModels/FruitBox/FruitBox02.fbx");
			fruitBox02Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			fruitBox02Renderer->SetDiffusePath("../3DModels/FruitBox/ARM/FruitBox_BaseColor.png");
			fruitBox02Renderer->SetNormalPath("../3DModels/FruitBox/ARM/FruitBox_Normal.png");
			fruitBox02Renderer->SetARMPath("../3DModels/FruitBox/ARM/FruitBox_ARM.png");
			fruitBox02Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			fruitBox02Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			fruitBox02Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			fruitBox02Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			fruitBox02Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileFruitBox02.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Watermelon
	std::ifstream inputFileWatermelon(WATERMELONFILE);

	if (inputFileWatermelon.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileWatermelon, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			//RocketEngine::GameObject* watermelon = RocketEngine::CreateObject("watermelon" + token);
			RocketEngine::GameObject* watermelon = RocketEngine::CreateObject("watermelon");

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			watermelon->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			watermelon->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* watermelonRenderer = watermelon->AddComponent<RocketEngine::MeshRenderer>();
			watermelonRenderer->SetMeshPath("../3DModels/Fruit/Watermelon.fbx");
			watermelonRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			watermelonRenderer->SetDiffusePath("../3DModels/Fruit/ARM/Fruit_BaseColor.png");
			watermelonRenderer->SetNormalPath("../3DModels/Fruit/ARM/Fruit_Normal.png");
			watermelonRenderer->SetARMPath("../3DModels/Fruit/ARM/Fruit_ARM.png");
			watermelonRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			watermelonRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			watermelonRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			watermelonRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			watermelonRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileWatermelon.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Kiwi
	std::ifstream inputFileKiwi(KIWIFILE);

	if (inputFileKiwi.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileKiwi, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* kiwi = RocketEngine::CreateObject("kiwi" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			kiwi->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			kiwi->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* kiwiRenderer = kiwi->AddComponent<RocketEngine::MeshRenderer>();
			kiwiRenderer->SetMeshPath("../3DModels/Fruit/Kiwi.fbx");
			kiwiRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			kiwiRenderer->SetDiffusePath("../3DModels/Fruit/ARM/Fruit_BaseColor.png");
			kiwiRenderer->SetNormalPath("../3DModels/Fruit/ARM/Fruit_Normal.png");
			kiwiRenderer->SetARMPath("../3DModels/Fruit/ARM/Fruit_ARM.png");
			kiwiRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			kiwiRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			kiwiRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			kiwiRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			kiwiRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileKiwi.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Carrot
	std::ifstream inputFileCarrot(CARROTFILE);

	if (inputFileCarrot.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileCarrot, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* carrot = RocketEngine::CreateObject("carrot" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			carrot->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			carrot->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* carrotRenderer = carrot->AddComponent<RocketEngine::MeshRenderer>();
			carrotRenderer->SetMeshPath("../3DModels/Fruit/Carrot.fbx");
			carrotRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			carrotRenderer->SetDiffusePath("../3DModels/Fruit/ARM/Fruit_BaseColor.png");
			carrotRenderer->SetNormalPath("../3DModels/Fruit/ARM/Fruit_Normal.png");
			carrotRenderer->SetARMPath("../3DModels/Fruit/ARM/Fruit_ARM.png");
			carrotRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			carrotRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			carrotRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			carrotRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			carrotRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileCarrot.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Banana
	std::ifstream inputFileBanana(BANANAFILE);

	if (inputFileBanana.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBanana, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* banana = RocketEngine::CreateObject("banana" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			banana->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			banana->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* bananaRenderer = banana->AddComponent<RocketEngine::MeshRenderer>();
			bananaRenderer->SetMeshPath("../3DModels/Fruit/Banana.fbx");
			bananaRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			bananaRenderer->SetDiffusePath("../3DModels/Fruit/ARM/Fruit_BaseColor.png");
			bananaRenderer->SetNormalPath("../3DModels/Fruit/ARM/Fruit_Normal.png");
			bananaRenderer->SetARMPath("../3DModels/Fruit/ARM/Fruit_ARM.png");
			bananaRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			bananaRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			bananaRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			bananaRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			bananaRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBanana.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Desk01
	std::ifstream inputFileDesk01(DESK01FILE);

	if (inputFileDesk01.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileDesk01, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* desk01 = RocketEngine::CreateObject("desk01" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			desk01->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			desk01->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* desk01Renderer = desk01->AddComponent<RocketEngine::MeshRenderer>();
			desk01Renderer->SetMeshPath("../3DModels/Gapan/Desk01.fbx");
			desk01Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			desk01Renderer->SetDiffusePath("../3DModels/Gapan/ARM/Gapan_BaseColor.png");
			desk01Renderer->SetNormalPath("../3DModels/Gapan/ARM/Gapan_Normal.png");
			desk01Renderer->SetARMPath("../3DModels/Gapan/ARM/Gapan_ARM.png");
			desk01Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			desk01Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			desk01Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			desk01Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			desk01Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileDesk01.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Desk02
	std::ifstream inputFileDesk02(DESK02FILE);

	if (inputFileDesk02.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileDesk02, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* desk02 = RocketEngine::CreateObject("desk02" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			desk02->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			desk02->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* desk02Renderer = desk02->AddComponent<RocketEngine::MeshRenderer>();
			desk02Renderer->SetMeshPath("../3DModels/Gapan/Desk02.fbx");
			desk02Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			desk02Renderer->SetDiffusePath("../3DModels/Gapan/ARM/Gapan_BaseColor.png");
			desk02Renderer->SetNormalPath("../3DModels/Gapan/ARM/Gapan_Normal.png");
			desk02Renderer->SetARMPath("../3DModels/Gapan/ARM/Gapan_ARM.png");
			desk02Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			desk02Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			desk02Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			desk02Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			desk02Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileDesk02.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Desk03
	std::ifstream inputFileDesk03(DESK03FILE);

	if (inputFileDesk03.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileDesk03, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* desk03 = RocketEngine::CreateObject("desk03" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			desk03->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			desk03->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* desk03Renderer = desk03->AddComponent<RocketEngine::MeshRenderer>();
			desk03Renderer->SetMeshPath("../3DModels/Gapan02/Desk03.fbx");
			desk03Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			desk03Renderer->SetDiffusePath("../3DModels/Gapan02/ARM/Gapan02_Basecolor.png");
			desk03Renderer->SetNormalPath("../3DModels/Gapan02/ARM/Gapan02_Normal.png");
			desk03Renderer->SetARMPath("../3DModels/Gapan02/ARM/Gapan02_ARM.png");
			desk03Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			desk03Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			desk03Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			desk03Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			desk03Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileDesk03.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Lamp
	std::ifstream inputFileLamp(LAMPFILE);

	if (inputFileLamp.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileLamp, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* lamp = RocketEngine::CreateObject("lamp" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			lamp->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			lamp->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* lampRenderer = lamp->AddComponent<RocketEngine::MeshRenderer>();
			lampRenderer->SetMeshPath("../3DModels/Gapan/Lamp.fbx");
			lampRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			lampRenderer->SetDiffusePath("../3DModels/Gapan/ARM/Gapan_BaseColor.png");
			lampRenderer->SetNormalPath("../3DModels/Gapan/ARM/Gapan_Normal.png");
			lampRenderer->SetARMPath("../3DModels/Gapan/ARM/Gapan_ARM.png");
			lampRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			lampRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			lampRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			lampRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			lampRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileLamp.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Chair
	std::ifstream inputFileChair(CHAIRFILE);

	if (inputFileChair.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileChair, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* chair = RocketEngine::CreateObject("chair" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			chair->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			chair->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* chairRenderer = chair->AddComponent<RocketEngine::MeshRenderer>();
			chairRenderer->SetMeshPath("../3DModels/Gapan/Chair.fbx");
			chairRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			chairRenderer->SetDiffusePath("../3DModels/Gapan/ARM/Gapan_BaseColor.png");
			chairRenderer->SetNormalPath("../3DModels/Gapan/ARM/Gapan_Normal.png");
			chairRenderer->SetARMPath("../3DModels/Gapan/ARM/Gapan_ARM.png");
			chairRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			chairRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			chairRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			chairRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			chairRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileChair.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// WoodWall
	std::ifstream inputFileWoodWall(WOODWALLFILE);

	if (inputFileWoodWall.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileWoodWall, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* woodWall = RocketEngine::CreateObject("woodWall" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			woodWall->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			woodWall->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* woodWallRenderer = woodWall->AddComponent<RocketEngine::MeshRenderer>();
			woodWallRenderer->SetMeshPath("../3DModels/Gapan/WoodWall.fbx");
			woodWallRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			woodWallRenderer->SetDiffusePath("../3DModels/Gapan/ARM/Gapan_BaseColor.png");
			woodWallRenderer->SetNormalPath("../3DModels/Gapan/ARM/Gapan_Normal.png");
			woodWallRenderer->SetARMPath("../3DModels/Gapan/ARM/Gapan_ARM.png");
			woodWallRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			woodWallRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			woodWallRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			woodWallRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			woodWallRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileWoodWall.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Board01
	std::ifstream inputFileBoard01(BOARD01FILE);

	if (inputFileBoard01.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBoard01, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* board01 = RocketEngine::CreateObject("board01" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			board01->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			board01->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* board01Renderer = board01->AddComponent<RocketEngine::MeshRenderer>();
			board01Renderer->SetMeshPath("../3DModels/Board/Board01.fbx");
			board01Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			board01Renderer->SetDiffusePath("../3DModels/Board/ARM/Board_Basecolor.png");
			board01Renderer->SetNormalPath("../3DModels/Board/ARM/Board_Normal.png");
			board01Renderer->SetARMPath("../3DModels/Board/ARM/Board_ARM.png");
			board01Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			board01Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			board01Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			board01Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			board01Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBoard01.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Board2
	std::ifstream inputFileBoard02(BOARD02FILE);

	if (inputFileBoard02.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBoard02, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* board02 = RocketEngine::CreateObject("board02" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			board02->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			board02->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* board02Renderer = board02->AddComponent<RocketEngine::MeshRenderer>();
			board02Renderer->SetMeshPath("../3DModels/Board/Board02.fbx");
			board02Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			board02Renderer->SetDiffusePath("../3DModels/Board/ARM/Board_Basecolor.png");
			board02Renderer->SetNormalPath("../3DModels/Board/ARM/Board_Normal.png");
			board02Renderer->SetARMPath("../3DModels/Board/ARM/Board_ARM.png");
			board02Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			board02Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			board02Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			board02Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			board02Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBoard02.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Board3
	std::ifstream inputFileBoard03(BOARD03FILE);

	if (inputFileBoard03.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBoard03, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* board03 = RocketEngine::CreateObject("board03" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			board03->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			board03->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* board03Renderer = board03->AddComponent<RocketEngine::MeshRenderer>();
			board03Renderer->SetMeshPath("../3DModels/Board/Board03.fbx");
			board03Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			board03Renderer->SetDiffusePath("../3DModels/Board/ARM/Board_Basecolor.png");
			board03Renderer->SetNormalPath("../3DModels/Board/ARM/Board_Normal.png");
			board03Renderer->SetARMPath("../3DModels/Board/ARM/Board_ARM.png");
			board03Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			board03Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			board03Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			board03Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			board03Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBoard03.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Board4
	std::ifstream inputFileBoard04(BOARD04FILE);

	if (inputFileBoard04.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBoard04, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* board04 = RocketEngine::CreateObject("board04" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			board04->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			board04->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* board04Renderer = board04->AddComponent<RocketEngine::MeshRenderer>();
			board04Renderer->SetMeshPath("../3DModels/Board/Board04.fbx");
			board04Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			board04Renderer->SetDiffusePath("../3DModels/Board/ARM/Board_Basecolor.png");
			board04Renderer->SetNormalPath("../3DModels/Board/ARM/Board_Normal.png");
			board04Renderer->SetARMPath("../3DModels/Board/ARM/Board_ARM.png");
			board04Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			board04Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			board04Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			board04Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			board04Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBoard04.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// Building01
	std::ifstream inputFileBuilding01(BUILDING01FILE);

	if (inputFileBuilding01.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBuilding01, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* building01 = RocketEngine::CreateObject("building01" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			building01->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			building01->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* building01Renderer = building01->AddComponent<RocketEngine::MeshRenderer>();
			building01Renderer->SetMeshPath("../3DModels/Building/Building01/Building01.fbx");
			building01Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			building01Renderer->SetDiffusePath("../3DModels/Building/Building01/Building01_BaseColor.png");
			building01Renderer->SetNormalPath("../3DModels/Building/Building01/Building01_Normal.png");
			building01Renderer->SetARMPath("../3DModels/Building/Building01/Building01_ARM.png");
			building01Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			building01Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			building01Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			building01Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			building01Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBuilding01.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// building02
	std::ifstream inputFileBuilding02(BUILDING02FILE);

	if (inputFileBuilding02.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBuilding02, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* building02 = RocketEngine::CreateObject("building02" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			building02->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			building02->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* building02Renderer = building02->AddComponent<RocketEngine::MeshRenderer>();
			building02Renderer->SetMeshPath("../3DModels/Building/Building01/Building01.fbx");
			building02Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			building02Renderer->SetDiffusePath("../3DModels/Building/Building01/Building01_02_BaseColor.png");
			building02Renderer->SetNormalPath("../3DModels/Building/Building01/Building01_02_Normal.png");
			building02Renderer->SetARMPath("../3DModels/Building/Building01/Building01_02_ARM.png");
			building02Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			building02Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			building02Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			building02Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			building02Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBuilding02.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

	// building02
	std::ifstream inputFileBuilding03(BUILDING03FILE);

	if (inputFileBuilding03.is_open())
	{
		// 파일로부터 문자열 읽기
		std::string line;
		while (std::getline(inputFileBuilding03, line))
		{
			std::istringstream iss(line);
			std::string token;

			// token은 현재 읽은 각 요소입니다.
			std::getline(iss, token, ',');
			RocketEngine::GameObject* building03 = RocketEngine::CreateObject("building03" + token);

			RocketEngine::RMFLOAT3 tempPos;
			std::getline(iss, token, ',');
			tempPos.z = stoi(token);
			std::getline(iss, token, ',');
			tempPos.x = stoi(token);
			std::getline(iss, token, ',');
			tempPos.y = stoi(token) + defaultHeight;

			building03->transform.SetLocalPosition(tempPos * posRatio);

			RocketEngine::RMFLOAT3 tempRot;
			std::getline(iss, token, ',');
			tempRot.z = stoi(token);
			std::getline(iss, token, ',');
			tempRot.x = stoi(token);
			std::getline(iss, token, ',');
			tempRot.y = stoi(token) + 90.0f;

			building03->transform.SetLocalRotationEuler(tempRot);

			RocketEngine::MeshRenderer* building03Renderer = building03->AddComponent<RocketEngine::MeshRenderer>();
			building03Renderer->SetMeshPath("../3DModels/Building/Building01/Building01.fbx");
			building03Renderer->SetShaderPath("../FX/Default/BasicPBR.cso");
			building03Renderer->SetDiffusePath("../3DModels/Building/Building01/Building01_03_BaseColor.png");
			building03Renderer->SetNormalPath("../3DModels/Building/Building01/Building01_03_Normal.png");
			building03Renderer->SetARMPath("../3DModels/Building/Building01/Building01_03_ARM.png");
			building03Renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
			building03Renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
			building03Renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
			building03Renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
			building03Renderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
		}

		// 파일 닫기
		inputFileBuilding03.close();
	}
	else
	{
		std::cout << "파일을 열 수 없습니다." << std::endl;
	}

}


void MainMenuSceneView::LobbyPlayerSlot()
{
	for (int i = 1; i <= 4; ++i)
	{
		std::string diffusePath = "";
		std::string normalPath = "";
		std::string ARMPath = "";

		switch (i)
		{
			case 1:
				diffusePath = "../3DModels/Player/Vgun_character_texture3/banana/BaseColor.png";
				normalPath = "../3DModels/Player/Vgun_character_texture3/banana/Normal.png";
				ARMPath = "../3DModels/Player/Vgun_character_texture3/banana/ARM.png";
				break;
			case 2:
				diffusePath = "../3DModels/Player/Vgun_character_texture3/carrot/BaseColor.png";
				normalPath = "../3DModels/Player/Vgun_character_texture3/carrot/Normal.png";
				ARMPath = "../3DModels/Player/Vgun_character_texture3/carrot/ARM.png";
				break;
			case 3:
				diffusePath = "../3DModels/Player/Vgun_character_texture3/kiwi/BaseColor.png";
				normalPath = "../3DModels/Player/Vgun_character_texture3/kiwi/Normal.png";
				ARMPath = "../3DModels/Player/Vgun_character_texture3/kiwi/ARM.png";
				break;
			case 4:
				diffusePath = "../3DModels/Player/Vgun_character_texture3/watermelon/BaseColor.png";
				normalPath = "../3DModels/Player/Vgun_character_texture3/watermelon/Normal.png";
				ARMPath = "../3DModels/Player/Vgun_character_texture3/watermelon/ARM.png";
				break;

			default:
				break;
		}

		RocketEngine::GameObject* player = RocketEngine::CreateObject("playerLobby" + std::to_string(i));
		player->transform.SetPosition({ 2.01f - (1.34f * (i-1)), 0.0f, 5.2f });

		RocketEngine::SkinnedMeshRenderer* playerRenderer = player->AddComponent<RocketEngine::SkinnedMeshRenderer>();
		playerRenderer->SetMeshPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		//playerRenderer->SetShaderPath("../FX/Default/SkinnedMeshTex.cso");
		playerRenderer->SetShaderPath("../FX/Default/BasicPBRSkinned.cso");
		playerRenderer->SetDiffusePath(diffusePath);
		playerRenderer->SetNormalPath(normalPath);
		playerRenderer->SetARMPath(ARMPath);

		playerRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
		playerRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
		playerRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
		playerRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
		playerRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR_SKINNED);

		RocketEngine::AnimationController* tAnimController = player->AddComponent<RocketEngine::AnimationController>();
		player->AddComponent<LobbyRandomAnim>();

		// Animator 및 State 생성.
		RocketEngine::Animator* tSkinAnimator = new RocketEngine::Animator();
		RocketEngine::State* tTestState = tSkinAnimator->AddNewState("lobbyIdle");

		//tTestState->SetAnimDataName("lobbyIdle");
		tTestState->SetAnimDescName("lobbyIdle");
		tTestState->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_idle_ani.fbx");
		tTestState->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		tTestState->SetLoop(true);

		tSkinAnimator->SetInitState(tTestState->GetStateName());

		// AnimationController에 Animator 부착.
		tAnimController->SetAnimator(tSkinAnimator);

		// TODO : 애니메이션도 붙여서 로비에서 아이들 뜨게끔 해야함

		_lobbyManager.SetPlayerSlot(i, player);
	}
}

// Sound 추가
void MainMenuSceneView::AddSound()
{
	// Button 클릭음
	RocketEngine::AddAudio("..\\Sound\\ButtonClick\\button_push.wav", eSoundGroup::EffectSound);
}
