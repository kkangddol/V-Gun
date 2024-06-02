#include "Client.h"
#include "RocketAPILoader.h"

#include "MainMenuSceneView.h"
#include "MainGameSceneView.h"
#include "OffsetTest.h"
#include "InGameSceneView.h"

#include "LobbyManager.h"
#include "NetworkManager.h"
#include "GameManager.h"

//[TW]
#include "TestTransform.h"

void Client::Initialize()
{
	LobbyManager::Instance();
	NetworkManager::Instance();
	GameManager::Instance();

// TestTransform 사용시엔 주석처리
#define MAINMEMU

#ifdef MAINMEMU
	MainMenuSceneView mainMenu;
	mainMenu.Initialize();

	MainGameSceneView mainGame;
	mainGame.Initialize();

	RocketEngine::LoadScene("MainMenu");
	//RocketEngine::LoadScene("MainGame");

	/// 여기서부터는 테스트

// 	OffsetTest test;
// 	test.Initialize();
// 	RocketEngine::LoadScene("OffsetTest");

// 	InGameSceneView ingame;
// 	ingame.Initialize();
// 	RocketEngine::LoadScene("InGame");

#else TESTTRANSFORM
	TestTransform testTransform;
	testTransform.Initialize();
	RocketEngine::LoadScene("TESTTRANSFORM");

#endif
}
