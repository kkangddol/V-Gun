#include "GameLobbySceneView.h"
#include "RocketAPILoader.h"

#include <cassert>

GameLobbySceneView::GameLobbySceneView()
	: scene()
{

}

void GameLobbySceneView::Initialize()
{
	scene = RocketEngine::CreateScene("GameLobby");
	RocketEngine::LoadScene("GameLobby");
	assert(scene);

	//////////////////////////////////////////////////////////////////////////

	/// Game Lobby UI
	// Game Lobby Canvas
	RocketEngine::GameObject* lobbyCanvas = RocketEngine::CreateObject("lobbyCanvas");

	// 게임 시작 버튼
	RocketEngine::GameObject* gameStartButtonObj = RocketEngine::CreateButton("gameStartButtonObj");
	RocketEngine::Button* gameStartButton = gameStartButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* gameStartButtonSprite = gameStartButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	gameStartButtonObj->transform.SetPosition(1717.0f, 961.0f, 0.0f);
	gameStartButton->SetSize(422.0f, 114.0f);
	gameStartButton->SetSortOrder(20);
	gameStartButtonSprite->SetPath("..\\UI\\WaitingRoom\\Gamestart_422x114.png");
	gameStartButtonSprite->SetSize(422.0f, 114.0f);
	gameStartButtonSprite->SetSortOrder(10);

	// 방 설정 버튼
	RocketEngine::GameObject* roomSettingButtonObj = RocketEngine::CreateButton("roomSettingButtonObj");
	RocketEngine::Button* roomSettingButton = roomSettingButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* roomSettingButtonSprite = roomSettingButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	roomSettingButtonObj->transform.SetPosition(115.0f, 986.0f, 0.0f);
	roomSettingButton->SetSize(110.0f, 1140.0f);
	roomSettingButton->SetSortOrder(20);
	roomSettingButtonSprite->SetPath("..\\UI\\WaitingRoom\\Edit_110x114.png");
	roomSettingButtonSprite->SetSize(110.0f, 114.0f);
	roomSettingButtonSprite->SetSortOrder(10);

	// 나가기 버튼
	RocketEngine::GameObject* exitButtonObj = RocketEngine::CreateButton("exitButtonObj");
	RocketEngine::Button* exitButton = exitButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* exitButtonSprite = exitButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	exitButtonObj->transform.SetPosition(232.0f, 986.0f, 0.0f);
	exitButton->SetSize(110.0f, 114.0f);
	exitButton->SetSortOrder(20);
	exitButtonSprite->SetPath("..\\UI\\WaitingRoom\\Quit_110x114.png");
	exitButtonSprite->SetSize(110.0f, 114.0f);
	exitButtonSprite->SetSortOrder(10);

	// Canvas Setting
	gameStartButtonObj->transform.SetParent(lobbyCanvas);
	roomSettingButtonObj->transform.SetParent(lobbyCanvas);
	exitButtonObj->transform.SetParent(lobbyCanvas);

	// Object 기본 상태 설정
	//lobbyCanvas->Enable();

	//////////////////////////////////////////////////////////////////////////
	
	/// Setting UI
	// 방 설정 Image
	RocketEngine::GameObject* roomSettingCanvas = RocketEngine::CreateImageBox("roomSettingCanvas");
	RocketEngine::SpriteRenderer* roomSettingCanvasSprite = roomSettingCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	roomSettingCanvas->transform.SetPosition(960.0f, 540.0f, 0.0f);
	roomSettingCanvasSprite->SetPath("..\\UI\\RoomEdit\\BackGround.png");
	roomSettingCanvasSprite->SetSize(860.0f, 669.0f);
	roomSettingCanvasSprite->SetSortOrder(30);

	// 닫기 버튼
	RocketEngine::GameObject* roomSettingExitButtonObj = RocketEngine::CreateButton("roomSettingExitButtonObj");
	RocketEngine::Button* roomSettingExitButton = roomSettingExitButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* roomSettingExitButtonSprite = roomSettingExitButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	roomSettingExitButtonObj->transform.SetPosition(960.5f, 932.0f, 0.0f);
	roomSettingExitButton->SetSortOrder(60);
	roomSettingExitButton->SetSize(191.0f, 79.0f);
	roomSettingExitButtonSprite->SetPath("..\\UI\\RoomEdit\\close.png");
	roomSettingExitButtonSprite->SetSize(187.0f, 70.0f);
	roomSettingExitButtonSprite->SetSortOrder(40);

	//// 라운드 Text Image
	//RocketEngine::GameObject* roundImageObj = RocketEngine::CreateTextBox("roundImageObj");
	//RocketEngine::SpriteRenderer* roundSprite = roundImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//roundImageObj->transform.SetPosition(1009.0f, 540.0f, 0.0f);
	//roundSprite->SetPath("..\\UI\\RoomEdit\\Round.png");
	//roundSprite->SetSize(180.0f, 70.0f);		// 임시
	//roundSprite->SetSortOrder(50);

	// 라운드 Num
	RocketEngine::GameObject* roundValBoxObj = RocketEngine::CreateImageBox("roundValBoxObj");
	RocketEngine::SpriteRenderer* roundValBoxSprite = roundValBoxObj->GetComponent<RocketEngine::SpriteRenderer>();
	roundValBoxObj->transform.SetPosition(963.0f, 493.0f, 0.0f);
	roundValBoxSprite->SetPath("..\\UI\\RoomEdit\\Round_Number_70x70.png");
	roundValBoxSprite->SetSize(70.0f, 70.0f);
	roundValBoxSprite->SetSortOrder(40);
	
	// 라운드 낮추기 버튼
	RocketEngine::GameObject* roundDownButtonObj = RocketEngine::CreateButton("roundDownButtonObj");
	RocketEngine::Button* roundDownButton = roundDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* roundDownButtonSprite = roundDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	roundDownButtonObj->transform.SetPosition(880.0f, 503.0f, 0.0f);
	roundDownButton->SetSize(71.0f, 66.0f);
	roundDownButton->SetSortOrder(60);
	roundDownButtonSprite->SetPath("..\\UI\\RoomEdit\\Round_LeftButton.png");
	roundDownButtonSprite->SetSize(71.0f, 66.0f);
	roundDownButtonSprite->SetSortOrder(40);

	// 라운드 올리기 버튼
	RocketEngine::GameObject* roundUpButtonObj = RocketEngine::CreateButton("roundUpButtonObj");
	RocketEngine::Button* roundUpButton = roundUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* roundUpButtonSprite = roundUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	roundUpButtonObj->transform.SetPosition(1063.5f, 503.0f, 0.0f);	// 임시
	roundUpButton->SetSize(71.0f, 66.0f);
	roundUpButton->SetSortOrder(60);
	roundUpButtonSprite->SetPath("..\\UI\\RoomEdit\\Round_RightButton.png");
	roundUpButtonSprite->SetSize(71.0f, 66.0f);
	roundUpButtonSprite->SetSortOrder(40);

	//// 시간 Image
	//RocketEngine::GameObject* timeImageObj = RocketEngine::CreateTextBox("timeImageObj");
	//RocketEngine::SpriteRenderer* timeSprite = timeImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//timeImageObj->transform.SetPosition(1009.0f, 718.0f, 0.0f);		// 임시
	//timeSprite->SetPath("..\\UI\\RoomEdit\\Time.png");
	//timeSprite->SetSize(200.0f, 75.0f);	// 임시
	//timeSprite->SetSortOrder(50);

	// 시간 표시 Box
	RocketEngine::GameObject* timeValBoxObj = RocketEngine::CreateImageBox("timeValBoxObj");
	RocketEngine::SpriteRenderer* timeValSprite = timeValBoxObj->GetComponent<RocketEngine::SpriteRenderer>();
	timeValBoxObj->transform.SetPosition(965.5f, 719.0f, 0.0f);
	timeValSprite->SetPath("..\\UI\\RoomEdit\\Time_Edit.png");
	timeValSprite->SetSize(177.0f, 94.0f);
	timeValSprite->SetSortOrder(40);

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

	// Canvas Setting
	roomSettingExitButtonObj->transform.SetParent(roomSettingCanvas);
	//roundImageObj->transform.SetParent(roomSettingCanvas);
	roundValBoxObj->transform.SetParent(roomSettingCanvas);
	roundDownButtonObj->transform.SetParent(roomSettingCanvas);
	roundUpButtonObj->transform.SetParent(roomSettingCanvas);
	//timeImageObj->transform.SetParent(roomSettingCanvas);
	timeValBoxObj->transform.SetParent(roomSettingCanvas);
	timeDownButtonObj->transform.SetParent(roomSettingCanvas);
	timeUpButtonObj->transform.SetParent(roomSettingCanvas);

	// Object 기본 상태 설정
	roomSettingCanvas->Disable();

	//////////////////////////////////////////////////////////////////////////

	/// Button 설정
	// Lobby - 게임 시작 Button (InGame 으로 이동)
	gameStartButton->SetOnClickEvent(
		[]()
		{

		}
	);

	// Lobby - 방 설정 Button (설정 창으로 이동)
	roomSettingButton->SetOnClickEvent(
		[roomSettingCanvas]()
		{
			roomSettingCanvas->Enable();
		}
	);

	// Lobby - 나가기 Button (메인 메뉴로 이동)
	exitButton->SetOnClickEvent(
		[]()
		{
			
		}
	);

	// RoomSetting - 닫기 Button
	roomSettingExitButton->SetOnClickEvent(
		[roomSettingCanvas]()
		{
			roomSettingCanvas->Disable();
		}
	);

	// RoomSetting - Round Down Button
	roundDownButton->SetOnClickEvent(
		[]()
		{

		}
	);

	// RoomSetting - Round Up Button
	roundUpButton->SetOnClickEvent(
		[]()
		{

		}
	);

	// RoomSetting - Time Down Button
	timeDownButton->SetOnClickEvent(
		[]()
		{

		}
	);

	// RoomSetting - Time Up Button
	timeUpButton->SetOnClickEvent(
		[]()
		{

		}
	);
}
