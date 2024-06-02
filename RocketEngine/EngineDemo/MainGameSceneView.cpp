#include "MainGameSceneView.h"
#include "RocketAPILoader.h"
#include "PlayerMove.h"
#include "PlayerInfo.h"
#include "CameraMove.h"
#include "GameManager.h"
#include "LobbyManager.h"
#include <vector>
#include <array>

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

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

MainGameSceneView::MainGameSceneView()
	: scene(),
	_gameManager(GameManager::Instance()),
	_lobbyManager(LobbyManager::Instance())
{

}

void MainGameSceneView::Initialize()
{
	scene = RocketEngine::CreateScene("MainGame");

	RocketEngine::GetMainCamera()->gameObject->AddComponent<CameraMove>();
	RocketEngine::GetMainCamera()->gameObject->transform.SetPosition(0.0f, 0.0f, 0.0f);
	RocketEngine::GetMainCamera()->gameObject->transform.SetRotation(1.0f,0.0f, 0.0f, 0.0f);

	// Light
//  RocketEngine::GameObject* dirLightObj = RocketEngine::CreateObject("dirLight");
//  RocketEngine::DirectionalLight* dirLight = dirLightObj->AddComponent<RocketEngine::DirectionalLight>();
//  dirLightObj->transform.Rotate(90.0f, 0.0f, 30.0f);

	// Audio 추가
	RocketEngine::AddAudio("../Sound/BGM/ingame_bgm2_50_50.wav", BackGroundMusic);
	RocketEngine::AddAudio("../Sound/RoundStart&End/round_gamestart.wav", SoundGroupChannel2);
	RocketEngine::AddAudio("../Sound/RoundStart&End/round_end.wav", SoundGroupChannel2);
	RocketEngine::AddAudio("../Sound/ResultBGM/result_bgm.wav", SoundGroupChannel2);
	RocketEngine::AddAudio("../Sound/PartsBuy/parts_buy.wav", eSoundGroup::EffectSound);
	RocketEngine::AddAudio("../Sound/Hit/hit_water.wav", eSoundGroup::EffectSound);

	MapObject();
	Player();
	InGameUI();
	DeathUI();
	SettingUI();
	RoundEnd();
	TotalResultUI();

}

void MainGameSceneView::MapObject()
{
	/// 물체들?
	// StaticPlane 생성
	RocketEngine::GameObject* plane = RocketEngine::CreateObject("plane");
	RocketEngine::PlaneCollider* planeCol = plane->AddComponent<RocketEngine::PlaneCollider>();
	plane->transform.Rotate(0.0f, 0.0f, 90.0f);
	RocketEngine::GameObject* wall_1 = RocketEngine::CreateObject("wall_1");
	RocketEngine::PlaneCollider* wall_1Col = wall_1->AddComponent<RocketEngine::PlaneCollider>();
	wall_1->transform.SetLocalPosition(20.0f, 20.0f, 0.0f);
	wall_1->transform.Rotate(0.0f, 0.0f, 180.0f);
	RocketEngine::GameObject* wall_2 = RocketEngine::CreateObject("wall_2");
	RocketEngine::PlaneCollider* wall_2Col = wall_2->AddComponent<RocketEngine::PlaneCollider>();
	wall_2->transform.SetLocalPosition(-20.0f, 20.0f, 0.0f);
	wall_2->transform.Rotate(0.0f, 0.0f, 0.0f);
	RocketEngine::GameObject* wall_3 = RocketEngine::CreateObject("wall_3");
	RocketEngine::PlaneCollider* wall_3Col = wall_3->AddComponent<RocketEngine::PlaneCollider>();
	wall_3->transform.SetLocalPosition(0.0f, 20.0f, 20.0f);
	wall_3->transform.Rotate(0.0f, 90.0f, 0.0f);
	RocketEngine::GameObject* wall_4 = RocketEngine::CreateObject("wall_4");
	RocketEngine::PlaneCollider* wall_4Col = wall_4->AddComponent<RocketEngine::PlaneCollider>();
	wall_4->transform.SetLocalPosition(0.0f, 20.0f, -20.0f);
	wall_4->transform.Rotate(0.0f, 90.0f, 180.0f);

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

			//RocketEngine::StaticBoxCollider* gapan02Col = gapan02->AddComponent<RocketEngine::StaticBoxCollider>();
			//gapan02Col->SetPositionOffset({ 0.0f, 4.0f * scaleRatio, 0.0f });
			//gapan02Col->SetVolume(6.0f * scaleRatio, 0.5f * scaleRatio, 4.6f * scaleRatio);

			RocketEngine::StaticBoxCollider* gapan02Roof_1Col = gapan02->AddComponent<RocketEngine::StaticBoxCollider>();
			gapan02Roof_1Col->SetPositionOffset({ 0.0f, 3.9f * scaleRatio, 1.1f });
			gapan02Roof_1Col->SetRotationOffset(15.0f, 0.0f, 0.0f);
			gapan02Roof_1Col->SetVolume(5.7f * scaleRatio, 0.4f * scaleRatio, 2.4f * scaleRatio);

			RocketEngine::StaticBoxCollider* gapan02Roof_2Col = gapan02->AddComponent<RocketEngine::StaticBoxCollider>();
			gapan02Roof_2Col->SetPositionOffset({ 0.0f, 4.0f * scaleRatio, -0.9f });
			gapan02Roof_2Col->SetRotationOffset(-12.0f, 0.0f, 0.0f);
			gapan02Roof_2Col->SetVolume(5.7f * scaleRatio, 0.2f * scaleRatio, 2.9f * scaleRatio);

			RocketEngine::StaticBoxCollider* gapan02Roof_3Col = gapan02->AddComponent<RocketEngine::StaticBoxCollider>();
			gapan02Roof_3Col->SetPositionOffset({ 0.0f, 2.9f * scaleRatio, -2.0f });
			gapan02Roof_3Col->SetVolume(5.7f * scaleRatio, 1.8f * scaleRatio, 0.1f * scaleRatio);

			RocketEngine::StaticBoxCollider* gapan02Leg_1Col = gapan02->AddComponent<RocketEngine::StaticBoxCollider>();
			gapan02Leg_1Col->SetPositionOffset({ -2.4f, 2.95f * scaleRatio, +0.2f });
			gapan02Leg_1Col->SetVolume(0.3f * scaleRatio, 5.9f * scaleRatio, 0.3f * scaleRatio);

			RocketEngine::StaticBoxCollider* gapan02Leg_2Col = gapan02->AddComponent<RocketEngine::StaticBoxCollider>();
			gapan02Leg_2Col->SetPositionOffset({ +2.3f, 2.95f * scaleRatio, +0.2f });
			gapan02Leg_2Col->SetVolume(0.3f * scaleRatio, 5.9f * scaleRatio, 0.3f * scaleRatio);
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

			RocketEngine::StaticBoxCollider* largeGapan01Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Col->SetPositionOffset({ 0.0f, 4.7f * scaleRatio, 0.0f });
			largeGapan01Col->SetRotationOffset(11.0f, 0.0f, 0.0f);
			largeGapan01Col->SetVolume(7.2f * scaleRatio, 0.1f * scaleRatio, 5.2f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_1Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_1Col->SetPositionOffset({ +2.98f, 2.85f * scaleRatio, -1.98f });
			largeGapan01Leg_1Col->SetVolume(0.3f * scaleRatio, 5.7f * scaleRatio, 0.3f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_2Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_2Col->SetPositionOffset({ -2.96f, 2.85f * scaleRatio, -1.98f });
			largeGapan01Leg_2Col->SetVolume(0.3f * scaleRatio, 5.7f * scaleRatio, 0.3f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_3Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_3Col->SetPositionOffset({ 0.8f, 2.9f * scaleRatio, -1.98f });
			largeGapan01Leg_3Col->SetVolume(0.3f * scaleRatio, 5.8f * scaleRatio, 0.3f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_4Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_4Col->SetPositionOffset({ -2.96f, 2.4f * scaleRatio, +1.98f });
			largeGapan01Leg_4Col->SetVolume(0.3f * scaleRatio, 4.8f * scaleRatio, 0.3f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_5Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_5Col->SetPositionOffset({ +2.96f, 2.4f * scaleRatio, +1.98f });
			largeGapan01Leg_5Col->SetVolume(0.3f * scaleRatio, 4.8f * scaleRatio, 0.3f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_6Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_6Col->SetPositionOffset({ +0.8f, 2.4f * scaleRatio, +1.98f });
			largeGapan01Leg_6Col->SetVolume(0.3f * scaleRatio, 4.8f * scaleRatio, 0.3f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_7Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_7Col->SetPositionOffset({ +2.96f, 3.4f * scaleRatio, 0.0f });
			largeGapan01Leg_7Col->SetVolume(0.1f * scaleRatio, 0.1f * scaleRatio, 4.8f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_8Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_8Col->SetPositionOffset({ -2.96f, 3.6f * scaleRatio, 0.0f });
			largeGapan01Leg_8Col->SetVolume(0.1f * scaleRatio, 0.1f * scaleRatio, 4.8f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_9Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_9Col->SetPositionOffset({ -1.1f, 3.5f * scaleRatio, 1.98f });
			largeGapan01Leg_9Col->SetVolume(5.0f * scaleRatio, 0.1f * scaleRatio, 0.1f * scaleRatio);

			RocketEngine::StaticBoxCollider* largeGapan01Leg_10Col = largeGapan01->AddComponent<RocketEngine::StaticBoxCollider>();
			largeGapan01Leg_10Col->SetPositionOffset({ -1.1f, 3.4f * scaleRatio, -1.98f });
			largeGapan01Leg_10Col->SetVolume(5.0f * scaleRatio, 0.1f * scaleRatio, 0.1f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* box01Col = box01->AddComponent<RocketEngine::StaticBoxCollider>();
			box01Col->SetPositionOffset({ 0.0f, 0.61f * scaleRatio, 0.0f });
			box01Col->SetVolume(1.2f * scaleRatio, 1.22f * scaleRatio, 1.2f * scaleRatio);
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

			RocketEngine::StaticBoxCollider* box02Col = box02->AddComponent<RocketEngine::StaticBoxCollider>();
			box02Col->SetPositionOffset({ 0.0f, 0.6f * scaleRatio, 0.0f });
			box02Col->SetVolume(1.2f * scaleRatio, 1.2f * scaleRatio, 1.2f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* oakCol = oak->AddComponent<RocketEngine::StaticBoxCollider>();
			oakCol->SetPositionOffset({ 0.0f, 0.75f * scaleRatio, 0.0f });
			oakCol->SetVolume(1.11f * scaleRatio, 1.5f * scaleRatio, 1.11f * scaleRatio);
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

			RocketEngine::StaticBoxCollider* cartCol = cart->AddComponent<RocketEngine::StaticBoxCollider>();
			cartCol->SetPositionOffset({ -0.7f, 0.8f * scaleRatio, 0.0f });
			cartCol->SetVolume(4.7f * scaleRatio, 0.6f * scaleRatio, 2.38f * scaleRatio);

			RocketEngine::StaticBoxCollider* cartHandle_1Col = cart->AddComponent<RocketEngine::StaticBoxCollider>();
			cartHandle_1Col->SetPositionOffset({ 1.5f, 1.0f * scaleRatio, 0.7f });
			cartHandle_1Col->SetVolume(2.8f * scaleRatio, 0.2f * scaleRatio, 0.2f * scaleRatio);

			RocketEngine::StaticBoxCollider* cartHandle_2Col = cart->AddComponent<RocketEngine::StaticBoxCollider>();
			cartHandle_2Col->SetPositionOffset({ 1.5f, 1.0f * scaleRatio, -0.7f });
			cartHandle_2Col->SetVolume(2.8f * scaleRatio, 0.2f * scaleRatio, 0.2f * scaleRatio);

			RocketEngine::StaticBoxCollider* cartSide_1Col = cart->AddComponent<RocketEngine::StaticBoxCollider>();
			cartSide_1Col->SetPositionOffset({ 0.4f, 1.3f * scaleRatio, 0.0f });
			cartSide_1Col->SetVolume(0.1f * scaleRatio, 0.5f * scaleRatio, 2.38f * scaleRatio);

			RocketEngine::StaticBoxCollider* cartSide_2Col = cart->AddComponent<RocketEngine::StaticBoxCollider>();
			cartSide_2Col->SetPositionOffset({ -2.5f, 1.3f * scaleRatio, 0.0f });
			cartSide_2Col->SetVolume(0.1f * scaleRatio, 0.5f * scaleRatio, 2.38f * scaleRatio);

			RocketEngine::StaticBoxCollider* cartSide_3Col = cart->AddComponent<RocketEngine::StaticBoxCollider>();
			cartSide_3Col->SetPositionOffset({ -1.0f, 1.6f * scaleRatio, -0.9f });
			cartSide_3Col->SetVolume(3.7f * scaleRatio, 1.2f * scaleRatio, 0.1f * scaleRatio);

			RocketEngine::StaticBoxCollider* cartSide_4Col = cart->AddComponent<RocketEngine::StaticBoxCollider>();
			cartSide_4Col->SetPositionOffset({ -1.0f, 1.6f * scaleRatio, 0.9f });
			cartSide_4Col->SetVolume(3.7f * scaleRatio, 1.2f * scaleRatio, 0.1f * scaleRatio);
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

			RocketEngine::StaticBoxCollider* fruitBox01Col = fruitBox01->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox01Col->SetPositionOffset({ 0.0f, -0.03f * scaleRatio, 0.0f });
			fruitBox01Col->SetVolume(1.0f * scaleRatio, 0.05f * scaleRatio, 1.2f * scaleRatio);

			RocketEngine::StaticBoxCollider* fruitBox01Side_1Col = fruitBox01->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox01Side_1Col->SetPositionOffset({ 0.36f, 0.25f * scaleRatio, 0.0f });
			fruitBox01Side_1Col->SetVolume(0.02f * scaleRatio, 0.5f * scaleRatio, 1.2f * scaleRatio);

			RocketEngine::StaticBoxCollider* fruitBox01Side_2Col = fruitBox01->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox01Side_2Col->SetPositionOffset({ -0.36f, 0.25f * scaleRatio, 0.0f });
			fruitBox01Side_2Col->SetVolume(0.02f * scaleRatio, 0.5f * scaleRatio, 1.2f * scaleRatio);

			RocketEngine::StaticBoxCollider* fruitBox01Side_3Col = fruitBox01->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox01Side_3Col->SetPositionOffset({ 0.0f, 0.25f * scaleRatio, 0.45f });
			fruitBox01Side_3Col->SetVolume(1.0f * scaleRatio, 0.5f * scaleRatio, 0.02f * scaleRatio);

			RocketEngine::StaticBoxCollider* fruitBox01Side_4Col = fruitBox01->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox01Side_4Col->SetPositionOffset({ 0.0f, 0.25f * scaleRatio, -0.45f });
			fruitBox01Side_4Col->SetVolume(1.0f * scaleRatio, 0.5f * scaleRatio, 0.02f * scaleRatio);
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

			RocketEngine::StaticBoxCollider* fruitBox02Col = fruitBox02->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox02Col->SetPositionOffset({ 0.05f, -0.03f * scaleRatio, -0.05f });
			fruitBox02Col->SetVolume(0.8f * scaleRatio, 0.05f * scaleRatio, 0.8f * scaleRatio);

			RocketEngine::StaticBoxCollider* fruitBox02Side_1Col = fruitBox02->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox02Side_1Col->SetPositionOffset({ 0.35f, 0.2f * scaleRatio, -0.05f });
			fruitBox02Side_1Col->SetVolume(0.02f * scaleRatio, 0.5f * scaleRatio, 0.8f * scaleRatio);

			RocketEngine::StaticBoxCollider* fruitBox02Side_2Col = fruitBox02->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox02Side_2Col->SetPositionOffset({ -0.25f, 0.2f * scaleRatio, -0.05f });
			fruitBox02Side_2Col->SetVolume(0.02f * scaleRatio, 0.5f * scaleRatio, 0.8f * scaleRatio);

			RocketEngine::StaticBoxCollider* fruitBox02Side_3Col = fruitBox02->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox02Side_3Col->SetPositionOffset({ 0.05f, 0.2f * scaleRatio, 0.25f });
			fruitBox02Side_3Col->SetVolume(0.8f * scaleRatio, 0.5f * scaleRatio, 0.02f * scaleRatio);

			RocketEngine::StaticBoxCollider* fruitBox02Side_4Col = fruitBox02->AddComponent<RocketEngine::StaticBoxCollider>();
			fruitBox02Side_4Col->SetPositionOffset({ 0.05f, 0.2f * scaleRatio, -0.35f });
			fruitBox02Side_4Col->SetVolume(0.8f * scaleRatio, 0.5f * scaleRatio, 0.02f * scaleRatio);
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
			RocketEngine::CapsuleCollider* watermelonCol = watermelon->AddComponent<RocketEngine::CapsuleCollider>();
			//RocketEngine::BoxCollider* watermelonCol = watermelon->AddComponent<RocketEngine::BoxCollider>();
			//watermelonCol->SetPositionOffset({ 0.0f, 0.4125f * scaleRatio, 0.0f });
			watermelonCol->SetPositionOffset({ 0.0f, 0.45f * scaleRatio, 0.0f });
			watermelonCol->SetCapsuleInfo(0.33f, 0.005f);
			//watermelonCol->SetVolume (0.661f * scaleRatio, 0.825f * scaleRatio, 0.671f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* kiwiCol = kiwi->AddComponent<RocketEngine::StaticBoxCollider>();
			kiwiCol->SetPositionOffset({ 0.0f, 0.112f * scaleRatio, 0.0f });
			kiwiCol->SetVolume(0.146f * scaleRatio, 0.224f * scaleRatio, 0.123f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* carrotCol = carrot->AddComponent<RocketEngine::StaticBoxCollider>();
			carrotCol->SetPositionOffset({ 0.0f, 0.1665f * scaleRatio, 0.0f });
			carrotCol->SetVolume(0.133f * scaleRatio, 0.333f * scaleRatio, 0.131f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* bananaCol = banana->AddComponent<RocketEngine::StaticBoxCollider>();
			bananaCol->SetPositionOffset({ 0.0f, 0.085f * scaleRatio, 0.0f });
			bananaCol->SetVolume(0.449f * scaleRatio, 0.17f * scaleRatio, 0.0863f * scaleRatio);
			//RocketEngine::CapsuleCollider* bananaCol = banana->AddComponent<RocketEngine::CapsuleCollider>();
			//bananaCol->SetPositionOffset({ 0.0f, 0.085f * scaleRatio, 0.0f });
			//bananaCol->SetRotationOffset(0.0f,0.0f,-90.0f);
			//bananaCol->SetCapsuleInfo(0.06f,0.16f);
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

			RocketEngine::StaticBoxCollider* desk01Col = desk01->AddComponent<RocketEngine::StaticBoxCollider>();
			desk01Col->SetPositionOffset({ 0.0f, 0.45f * scaleRatio, 0.0f });
			desk01Col->SetVolume(4.45f * scaleRatio, 0.9f * scaleRatio, 1.4f * scaleRatio);

			RocketEngine::StaticBoxCollider* desk01Upper_1Col = desk01->AddComponent<RocketEngine::StaticBoxCollider>();
			desk01Upper_1Col->SetPositionOffset({ 0.0f, 1.1f * scaleRatio, -0.35f });
			desk01Upper_1Col->SetRotationOffset(25.0f, 0.0f, 0.0f);
			desk01Upper_1Col->SetVolume(4.45f * scaleRatio, 0.2f * scaleRatio, 0.6f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* desk02Col = desk02->AddComponent<RocketEngine::StaticBoxCollider>();
			desk02Col->SetPositionOffset({ 0.0f, 0.05f * scaleRatio, 0.0f });
			desk02Col->SetVolume(1.15f * scaleRatio, 0.1f * scaleRatio, 3.5f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* desk03Col = desk03->AddComponent<RocketEngine::StaticBoxCollider>();
			desk03Col->SetPositionOffset({ 0.0f, 0.55f * scaleRatio, 0.0f });
			desk03Col->SetVolume(3.8f * scaleRatio, 1.1f * scaleRatio, 1.48f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* lampCol = lamp->AddComponent<RocketEngine::StaticBoxCollider>();
			lampCol->SetPositionOffset({ 0.0f, -1.05f * scaleRatio, 0.0f });
			lampCol->SetVolume(0.392f * scaleRatio, 0.85f * scaleRatio, 0.392f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* chairCol = chair->AddComponent<RocketEngine::StaticBoxCollider>();
			chairCol->SetPositionOffset({ 0.0f, 0.5f * scaleRatio, 0.0f });
			chairCol->SetVolume(0.691f * scaleRatio, 1.0f * scaleRatio, 0.691f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* woodWallCol = woodWall->AddComponent<RocketEngine::StaticBoxCollider>();
			woodWallCol->SetPositionOffset({ 0.0f, 1.1f * scaleRatio, 0.0f });
			woodWallCol->SetVolume(0.183f * scaleRatio, 2.2f * scaleRatio, 4.8f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* board01Col = board01->AddComponent<RocketEngine::StaticBoxCollider>();
			board01Col->SetPositionOffset({ 0.0f, 0.93f * scaleRatio, 0.0f });
			board01Col->SetVolume(5.5f * scaleRatio, 1.86f * scaleRatio, 0.12f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* board02Col = board02->AddComponent<RocketEngine::StaticBoxCollider>();
			board02Col->SetPositionOffset({ 0.0f, 1.0f * scaleRatio, 0.0f });
			board02Col->SetVolume(5.59f * scaleRatio, 2.0f * scaleRatio, 0.1f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* board03Col = board03->AddComponent<RocketEngine::StaticBoxCollider>();
			board03Col->SetPositionOffset({ 0.0f, 1.25f * scaleRatio, 0.0f });
			board03Col->SetVolume(2.4f * scaleRatio, 2.4f * scaleRatio, 0.12f * scaleRatio);
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
			RocketEngine::StaticBoxCollider* board04Col = board04->AddComponent<RocketEngine::StaticBoxCollider>();
			board04Col->SetPositionOffset({ 0.0f, 1.2f * scaleRatio, 0.0f });
			board04Col->SetVolume(4.2f * scaleRatio, 2.45f * scaleRatio, 0.12f * scaleRatio);
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

void MainGameSceneView::Player()
{
	/// Player 오브젝트
	// Player 나
	RocketEngine::GameObject* player = RocketEngine::CreateObject("player");
	PlayerMove* pCtrl = player->AddComponent<PlayerMove>();
	auto playerInfo = player->AddComponent<PlayerInfo>();
	GameManager::Instance().AddPlayerInfo(playerInfo);

	auto playerRenderer = player->AddComponent<RocketEngine::SkinnedMeshRenderer>();
	playerRenderer->SetMeshPath("../3DModels/Player/Cube.fbx");
	//playerRenderer->SetShaderPath("../FX/Default/SkinnedMeshTex.cso");
	playerRenderer->SetShaderPath("../FX/Default/BasicPBRSkinned.cso");
	std::string diffusePath = "../3DModels/Player/Vgun_character_texture3/banana/BaseColor.png";
	std::string normalPath = "../3DModels/Player/Vgun_character_texture3/banana/Normal.png";
	std::string ARMPath = "../3DModels/Player/Vgun_character_texture3/banana/ARM.png";
	playerRenderer->SetDiffusePath(diffusePath);
	playerRenderer->SetNormalPath(normalPath);
	playerRenderer->SetARMPath(ARMPath);
	playerRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR_SKINNED);

	RocketEngine::AnimationController* playerAnimCtrl = player->AddComponent<RocketEngine::AnimationController>();

#pragma region MyAnimator
	// Animator 및 State 생성.
	RocketEngine::Animator* playerAnimator = new RocketEngine::Animator();

	RocketEngine::State* playerIdle = playerAnimator->AddNewState("Idle");
	playerIdle->SetAnimDescName("MyIdle");
	playerIdle->SetAnimDescAnimPath("../3DModels/Player/Cube.fbx");
	playerIdle->SetAnimDescOwnerPath("../3DModels/Player/Cube.fbx");
	playerIdle->SetLoop(true);

	RocketEngine::State* playerMoveForward = playerAnimator->AddNewState("MoveForward");
	playerMoveForward->SetAnimDescName("MyMoveForward");
	playerMoveForward->SetAnimDescAnimPath("../3DModels/Player/Cube.fbx");
	playerMoveForward->SetAnimDescOwnerPath("../3DModels/Player/Cube.fbx");
	playerMoveForward->SetLoop(false);

	RocketEngine::State* playerJump = playerAnimator->AddNewState("Jump");
	playerJump->SetAnimDescName("MyJump");
	playerJump->SetAnimDescAnimPath("../3DModels/Player/Cube.fbx");
	playerJump->SetAnimDescOwnerPath("../3DModels/Player/Cube.fbx");
	playerJump->SetLoop(false);

	RocketEngine::State* playerDie = playerAnimator->AddNewState("Die");
	playerDie->SetAnimDescName("MyDie");
	playerDie->SetAnimDescAnimPath("../3DModels/Player/Cube.fbx");
	playerDie->SetAnimDescOwnerPath("../3DModels/Player/Cube.fbx");
	playerDie->SetLoop(false);

	RocketEngine::State* playerReload = playerAnimator->AddNewState("Reload");
	playerReload->SetAnimDescName("MyReload");
	playerReload->SetAnimDescAnimPath("../3DModels/Player/Cube.fbx");
	playerReload->SetAnimDescOwnerPath("../3DModels/Player/Cube.fbx");
	playerReload->SetLoop(false);

	RocketEngine::State* playerShoot = playerAnimator->AddNewState("MyShoot");
	playerShoot->SetAnimDescName("MyShoot");
	playerShoot->SetAnimDescAnimPath("../3DModels/Player/Cube.fbx");
	playerShoot->SetAnimDescOwnerPath("../3DModels/Player/Cube.fbx");
	playerShoot->SetLoop(false);

	RocketEngine::State* playerMoveRight = playerAnimator->AddNewState("MoveRight");
	playerMoveRight->SetAnimDescName("MyMoveRight");
	playerMoveRight->SetAnimDescAnimPath("../3DModels/Player/Cube.fbx");
	playerMoveRight->SetAnimDescOwnerPath("../3DModels/Player/Cube.fbx");
	playerMoveRight->SetLoop(false);

	RocketEngine::State* playerMoveLeft = playerAnimator->AddNewState("MoveLeft");
	playerMoveLeft->SetAnimDescName("MyMoveLeft");
	playerMoveLeft->SetAnimDescAnimPath("../3DModels/Player/Cube.fbx");
	playerMoveLeft->SetAnimDescOwnerPath("../3DModels/Player/Cube.fbx");
	playerMoveLeft->SetLoop(false);

	playerAnimator->SetInitState(playerIdle->GetStateName());

	// AnimationController에 Animator 부착.
	playerAnimCtrl->SetAnimator(playerAnimator);

#pragma endregion MyAnimator



	RocketEngine::BoxCollider* playerCol = player->AddComponent<RocketEngine::BoxCollider>();
	playerCol->SetPositionOffset({ 0.0f, 1.25f, 0.0f });
	playerCol->SetVolume(0.001f, 0.001f, 0.001f);

	// Player Body
	RocketEngine::GameObject* playerBody = RocketEngine::CreateObject("playerBody");
	playerBody->transform.SetParent(player);
	playerBody->transform.SetLocalPosition(0.0f, 0.57f, 0.0f);
	playerBody->transform.SetLocalRotationEuler(0.0f, 0.0f, 90.0f);
	RocketEngine::CapsuleCollider* bodyCol = playerBody->AddComponent<RocketEngine::CapsuleCollider>();
	bodyCol->SetCapsuleInfo(0.25f, 0.32f);

	// Player Head
	RocketEngine::GameObject* playerHead = RocketEngine::CreateObject("playerHead");
	playerHead->transform.SetParent(player);
	playerHead->transform.SetLocalPosition(0.0f, 1.5f, 0.0f);
	RocketEngine::SphereCollider* headCol = playerHead->AddComponent<RocketEngine::SphereCollider>();
	//headCol->SetVolume(0.6f, 0.55f, 0.6f);
	headCol->SetRadius(0.3f);

	// 계층구조와 fixedJoint 만드는 부분.
	playerCol->SetFixedJoint(bodyCol);
	playerCol->SetFixedJoint(headCol);


	// Camera, 1인칭 총모양
	RocketEngine::GameObject* mainCam = RocketEngine::GetMainCamera()->gameObject;
	mainCam->transform.SetParent(player);
	mainCam->transform.SetLocalPosition(0.0f, 1.5f, 0.0f);
	mainCam->transform.SetLocalRotation(1.0f, 0.0f, 0.0f, 0.0f);

	RocketEngine::GameObject* bulletCarrot = RocketEngine::CreateObject("bulletCarrot");
	bulletCarrot->transform.SetParent(mainCam);
	bulletCarrot->transform.SetLocalPosition(0.1f, -0.1f, 1.1f);
	bulletCarrot->transform.SetLocalRotationEuler(-100.0f,-5.0f,-20.0f);
	bulletCarrot->transform.SetLocalScale(0.6f, 0.6f, 0.6f);

	RocketEngine::MeshRenderer* bulletCarrotRenderer = bulletCarrot->AddComponent<RocketEngine::MeshRenderer>();
	bulletCarrotRenderer->SetMeshPath("../3DModels/Fruit/Carrot.fbx");
	bulletCarrotRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	bulletCarrotRenderer->SetDiffusePath("../3DModels/Fruit/ARM/Fruit_BaseColor.png");
	bulletCarrotRenderer->SetNormalPath("../3DModels/Fruit/ARM/Fruit_Normal.png");
	bulletCarrotRenderer->SetARMPath("../3DModels/Fruit/ARM/Fruit_ARM.png");
	bulletCarrotRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	bulletCarrotRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	bulletCarrotRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	bulletCarrotRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	bulletCarrotRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);


	for (int i = 0; i < 4; ++i)
	{
		std::string diffuse = "";
		std::string normal = "";
		std::string arm = "";
		switch (i)
		{
			case 0:
				//watermelon
				diffuse = "../3DModels/FirstPersonShooting/the_first_person_texture/watermelon/basecolor.png";
				normal = "../3DModels/FirstPersonShooting/the_first_person_texture/watermelon/nomal.png";
				arm = "../3DModels/FirstPersonShooting/the_first_person_texture/watermelon/arm.png";
				break;
			case 1:
				//kiwi
				diffuse = "../3DModels/FirstPersonShooting/the_first_person_texture/kiwi/basecolor.png";
				normal = "../3DModels/FirstPersonShooting/the_first_person_texture/kiwi/nomal.png";
				arm = "../3DModels/FirstPersonShooting/the_first_person_texture/kiwi/arm.png";
				break;
			case 2:
				//carrot
				diffuse = "../3DModels/FirstPersonShooting/the_first_person_texture/carrot/basecolor.png";
				normal = "../3DModels/FirstPersonShooting/the_first_person_texture/carrot/nomal.png";
				arm = "../3DModels/FirstPersonShooting/the_first_person_texture/carrot/arm.png";
				break;
			case 3:
				//banana
				diffuse = "../3DModels/FirstPersonShooting/the_first_person_texture/banana/basecolor.png";
				normal = "../3DModels/FirstPersonShooting/the_first_person_texture/banana/nomal.png";
				arm = "../3DModels/FirstPersonShooting/the_first_person_texture/banana/arm.png";
				break;
			default:
				break;
		}

		RocketEngine::GameObject* vgunObject = RocketEngine::CreateObject("VGun"+std::to_string(i));
		vgunObject->transform.SetParent(mainCam);
		//vgunObject->transform.SetLocalPosition(0.25f, -0.38f, 0.75f);
		//vgunObject->transform.SetRotationEuler(2.0f, 135.0f, 5.0f);
		vgunObject->transform.SetLocalPosition(0.28f, -0.35f, 0.7f);
		vgunObject->transform.SetRotationEuler(0.0f, -50.0f, -5.0f);

		RocketEngine::SkinnedMeshRenderer* gunRenderer = vgunObject->AddComponent<RocketEngine::SkinnedMeshRenderer>();
		gunRenderer->SetMeshPath("../3DModels/FirstPersonShooting/fps_modelong_shooting_ani.fbx");
		//gunRenderer->SetShaderPath("../FX/Default/SkinnedMeshTex.cso");
		gunRenderer->SetShaderPath("../FX/Default/BasicPBRSkinned.cso");
		gunRenderer->SetDiffusePath(diffuse);
		gunRenderer->SetNormalPath(normal);
		gunRenderer->SetARMPath(arm);
		gunRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
		gunRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
		gunRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
		gunRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
		gunRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR_SKINNED);

		RocketEngine::AnimationController* tAnimController = vgunObject->AddComponent<RocketEngine::AnimationController>();

		// Animator 및 State 생성.
		RocketEngine::Animator* gunAnimator = new RocketEngine::Animator();

	// 	RocketEngine::State* gunIdle = gunAnimator->AddNewState("Idle");
	// 	gunIdle->SetAnimDescName("GunIdle");
	// 	gunIdle->SetAnimDescAnimPath("../3DModels/FirstPersonShooting/fps_modeling.fbx");
	// 	gunIdle->SetAnimDescOwnerPath("../3DModels/FirstPersonShooting/fps_modeling.fbx");
	// 	gunIdle->SetLoop(true);

		RocketEngine::State* gunShoot = gunAnimator->AddNewState("Shoot");
		gunShoot->SetAnimDescName("GunShoot");
		gunShoot->SetAnimDescAnimPath("../3DModels/FirstPersonShooting/fps_modelong_shooting_ani.fbx");
		gunShoot->SetAnimDescOwnerPath("../3DModels/FirstPersonShooting/fps_modelong_shooting_ani.fbx");
		gunShoot->SetLoop(false);


		gunAnimator->SetInitState(gunShoot->GetStateName());

		// AnimationController에 Animator 부착.
		tAnimController->SetAnimator(gunAnimator);

		pCtrl->AssignGunAnim(i,tAnimController);

		GameManager::Instance().AssignGunModeling(vgunObject, i);
		vgunObject->Disable();
	}



	// Audio 등록
	RocketEngine::AudioClip* pSound = player->AddComponent<RocketEngine::AudioClip>();
	pSound->Add3DAudio("walk01", "../Sound/Walk/footfall_01.wav", EffectSound, 0.5f, 10.0f);
	pSound->Add3DAudio("walk02", "../Sound/Walk/footfall_02.wav", EffectSound, 0.5f, 10.0f);
	pSound->Add3DAudio("jump", "../Sound/Jump&Land/jump.wav", EffectSound, 1.0f, 10.0f);
	pSound->Add3DAudio("landing", "../Sound/Jump&Land/landing.wav", EffectSound, 0.3f, 10.0f);
	pSound->Add3DAudio("shoot", "../Sound/Shoot/Gun_sound.wav", EffectSound, 0.4f, 20.0f);
	pSound->Add3DAudio("reload", "../Sound/GunReload/gun_reload.wav", EffectSound, 0.3f, 5.0f);

	for (int i = 1; i <= 4; ++i)
	{
		RocketEngine::GameObject* otherPlayer = RocketEngine::CreateObject("player" + std::to_string(i));
		PlayerMove* otherMoveCtrl = otherPlayer->AddComponent<PlayerMove>();
		auto otherInfo = otherPlayer->AddComponent<PlayerInfo>();

		std::string diffusePath = "";
		std::string normalPath = "";
		std::string ARMPath = "";

		switch (i)
		{
			case 1:
				diffusePath = "../3DModels/Player/Vgun_character_texture3/watermelon/BaseColor.png";
				normalPath = "../3DModels/Player/Vgun_character_texture3/watermelon/Normal.png";
				ARMPath = "../3DModels/Player/Vgun_character_texture3/watermelon/ARM.png";
				break;
			case 2:
				diffusePath = "../3DModels/Player/Vgun_character_texture3/kiwi/BaseColor.png";
				normalPath = "../3DModels/Player/Vgun_character_texture3/kiwi/Normal.png";
				ARMPath = "../3DModels/Player/Vgun_character_texture3/kiwi/ARM.png";
				break;
			case 3:
				diffusePath = "../3DModels/Player/Vgun_character_texture3/carrot/BaseColor.png";
				normalPath = "../3DModels/Player/Vgun_character_texture3/carrot/Normal.png";
				ARMPath = "../3DModels/Player/Vgun_character_texture3/carrot/ARM.png";
				break;
			case 4:
				diffusePath = "../3DModels/Player/Vgun_character_texture3/banana/BaseColor.png";
				normalPath = "../3DModels/Player/Vgun_character_texture3/banana/Normal.png";
				ARMPath = "../3DModels/Player/Vgun_character_texture3/banana/ARM.png";
				break;

			default:
				break;
		}

		RocketEngine::SkinnedMeshRenderer* otherRenderer = otherPlayer->AddComponent<RocketEngine::SkinnedMeshRenderer>();
		otherRenderer->SetMeshPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		//otherRenderer->SetShaderPath("../FX/Default/SkinnedMeshTex.cso");
		otherRenderer->SetShaderPath("../FX/Default/BasicPBRSkinned.cso");
		otherRenderer->SetDiffusePath(diffusePath);
		otherRenderer->SetNormalPath(normalPath);
		otherRenderer->SetARMPath(ARMPath);

		otherRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
		otherRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
		otherRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
		otherRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
		otherRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR_SKINNED);

		RocketEngine::AnimationController* otherAnimController = otherPlayer->AddComponent<RocketEngine::AnimationController>();

		// Animator 및 State 생성.
		RocketEngine::Animator* otherAnimator = new RocketEngine::Animator();

		RocketEngine::State* otherIdle = otherAnimator->AddNewState("Idle");
		otherIdle->SetAnimDescName("Idle");
		otherIdle->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_idle_ani.fbx");
		otherIdle->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		otherIdle->SetLoop(true);

		RocketEngine::State* otherMoveForward = otherAnimator->AddNewState("MoveForward");
		otherMoveForward->SetAnimDescName("MoveForward");
		otherMoveForward->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_move_ani.fbx");
		otherMoveForward->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		otherMoveForward->SetLoop(false);

		RocketEngine::State* otherJump = otherAnimator->AddNewState("Jump");
		otherJump->SetAnimDescName("Jump");
		otherJump->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_jump_ani.fbx");
		otherJump->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		otherJump->SetLoop(false);

		RocketEngine::State* otherDie = otherAnimator->AddNewState("Die");
		otherDie->SetAnimDescName("Die");
		otherDie->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_die_ani.fbx");
		otherDie->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		otherDie->SetLoop(false);

		RocketEngine::State* otherReload = otherAnimator->AddNewState("Reload");
		otherReload->SetAnimDescName("Reload");
		otherReload->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_reload_ani.fbx");
		otherReload->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		otherReload->SetLoop(false);

		RocketEngine::State* otherShoot = otherAnimator->AddNewState("Shoot");
		otherShoot->SetAnimDescName("Shoot");
		otherShoot->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_shoot_ani.fbx");
		otherShoot->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		otherShoot->SetLoop(false);

		RocketEngine::State* otherMoveRight = otherAnimator->AddNewState("MoveRight");
		otherMoveRight->SetAnimDescName("MoveRight");
		otherMoveRight->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_sidestep_reverse.fbx");
		otherMoveRight->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		otherMoveRight->SetLoop(false);

		RocketEngine::State* otherMoveLeft = otherAnimator->AddNewState("MoveLeft");
		otherMoveLeft->SetAnimDescName("MoveLeft");
		otherMoveLeft->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_sidestep_ani.fbx");
		otherMoveLeft->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
		otherMoveLeft->SetLoop(false);

		otherAnimator->SetInitState(otherIdle->GetStateName());

		// AnimationController에 Animator 부착.
		otherAnimController->SetAnimator(otherAnimator);


		/////////////////




		RocketEngine::BoxCollider* playerCol = otherPlayer->AddComponent<RocketEngine::BoxCollider>();
		playerCol->SetPositionOffset({ 0.0f, 1.25f, 0.0f });
		playerCol->SetVolume(0.001f, 0.001f, 0.001f);
		//playerCol->DisableSimulation();

		// Player Body
		RocketEngine::GameObject* playerBody = RocketEngine::CreateObject("playerBody" + std::to_string(i));
		playerBody->transform.SetParent(otherPlayer);
		playerBody->transform.SetLocalPosition(0.0f, 0.57f, 0.0f);
		playerBody->transform.SetLocalRotationEuler(0.0f, 0.0f, 90.0f);
		RocketEngine::CapsuleCollider* bodyCol = playerBody->AddComponent<RocketEngine::CapsuleCollider>();
		bodyCol->SetCapsuleInfo(0.25f, 0.32f);

		// Player Head
		RocketEngine::GameObject* playerHead = RocketEngine::CreateObject("playerHead" + std::to_string(i));
		playerHead->transform.SetParent(otherPlayer);
		playerHead->transform.SetLocalPosition(0.0f, 1.5f, 0.0f);
		RocketEngine::SphereCollider* headCol = playerHead->AddComponent<RocketEngine::SphereCollider>();
		//headCol->SetVolume(0.6f, 0.55f, 0.6f);
		headCol->SetRadius(0.3f);

		// 계층구조와 fixedJoint 만드는 부분.
		playerCol->SetFixedJoint(bodyCol);
		playerCol->SetFixedJoint(headCol);


		RocketEngine::AudioClip* pSound = otherPlayer->AddComponent<RocketEngine::AudioClip>();
		pSound->Add3DAudio("walk01", "../Sound/Walk/footfall_01.wav", EffectSound, 0.5f, 10.0f);
		pSound->Add3DAudio("walk02", "../Sound/Walk/footfall_02.wav", EffectSound, 0.5f, 10.0f);
		pSound->Add3DAudio("jump", "../Sound/Jump&Land/jump.wav", EffectSound, 1.0f, 10.0f);
		pSound->Add3DAudio("landing", "../Sound/Jump&Land/landing.wav", EffectSound, 0.2f, 10.0f);
		pSound->Add3DAudio("shoot", "../Sound/Shoot/Gun_sound.wav", EffectSound, 0.3f, 20.0f);


		_gameManager.AddPlayerInfo(otherInfo);
		otherPlayer->FlushDisable();
	}
}

void MainGameSceneView::InGameUI()
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
	RocketEngine::GameObject* player1KillObj = RocketEngine::CreateObject("player1KillObj");
	RocketEngine::TextBox* player1KillText = player1KillObj->AddComponent<RocketEngine::TextBox>();
	player1KillObj->transform.SetPosition(526.5f, 73.5f, 0.0f);
	player1KillText->SetSize(30.0f, 30.0f);
	player1KillText->SetFontSize(20);
	player1KillText->SetSortOrder(15);
	player1KillText->SetIgnoreFocus(true);
	player1KillText->SetText("0");

	//// 캐릭터 킬 현황2
	RocketEngine::GameObject* player2KillObj = RocketEngine::CreateObject("player2KillObj");
	RocketEngine::TextBox* player2KillText = player2KillObj->AddComponent<RocketEngine::TextBox>();
	player2KillObj->transform.SetPosition(732.0f, 73.5f, 0.0f);
	player2KillText->SetSize(30.0f, 30.0f);
	player2KillText->SetFontSize(20);
	player2KillText->SetSortOrder(15);
	player2KillText->SetIgnoreFocus(true);
	player2KillText->SetText("0");

	//// 캐릭터 킬 현황3
	RocketEngine::GameObject* player3KillObj = RocketEngine::CreateObject("player3KillObj");
	RocketEngine::TextBox* player3KillText = player3KillObj->AddComponent<RocketEngine::TextBox>();
	player3KillObj->transform.SetPosition(1187.0f, 73.5f, 0.0f);
	player3KillText->SetSize(30.0f, 30.0f);
	player3KillText->SetFontSize(20);
	player3KillText->SetSortOrder(15);
	player3KillText->SetIgnoreFocus(true);
	player3KillText->SetText("0");

	//// 캐릭터 킬 현황4
	RocketEngine::GameObject* player4KillObj = RocketEngine::CreateObject("player4KillObj");
	RocketEngine::TextBox* player4KillText = player4KillObj->AddComponent<RocketEngine::TextBox>();
	player4KillObj->transform.SetPosition(1385.0f, 73.0f, 0.0f);
	player4KillText->SetSize(30.0f, 30.0f);
	player4KillText->SetFontSize(20);
	player4KillText->SetSortOrder(15);
	player4KillText->SetIgnoreFocus(true);
	player4KillText->SetText("0");

	//// 라운드 시간
	RocketEngine::GameObject* roundTimeObj = RocketEngine::CreateObject("roundTimeObj");
	RocketEngine::TextBox* roundTimeText = roundTimeObj->AddComponent<RocketEngine::TextBox>();
	roundTimeObj->transform.SetPosition(960.0f, 60.0f, 0.0f);
	roundTimeText->SetSize(150.f, 100.0f);
	roundTimeText->SetSortOrder(15);
	roundTimeText->SetIgnoreFocus(true);

	// Player 상태창
	RocketEngine::GameObject* playerStatusPaneImageObj = RocketEngine::CreateImageBox("playerStatusPaneImageObj");
	RocketEngine::SpriteRenderer* playerStatusPaneSprite = playerStatusPaneImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	playerStatusPaneImageObj->transform.SetPosition(276.5f, 957.0f, 0.0f);
	playerStatusPaneSprite->SetPath("..\\UI\\InGame\\Character_Watermelon_494x178.png");
	playerStatusPaneSprite->SetSize(494.0f, 178.0f);
	playerStatusPaneSprite->SetSortOrder(10);

	// 체력 바
	RocketEngine::GameObject* playerhpBarImageObj = RocketEngine::CreateImageBox("playerhpBarImageObj");
	RocketEngine::SpriteRenderer* playerhpBarSprite = playerhpBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();;
	playerhpBarImageObj->transform.SetPosition(330.0f, 980.5f, 0.0f);
	playerhpBarSprite->SetPath("..\\UI\\InGame\\HealthBar_250x45.png");
	playerhpBarSprite->SetSize(250.0f, 45.0f);
	playerhpBarSprite->SetSortOrder(10);

	// 총알 수
	RocketEngine::GameObject* ammoObj = RocketEngine::CreateObject("ammoObj");
	RocketEngine::TextBox* ammoText = ammoObj->AddComponent<RocketEngine::TextBox>();
	ammoObj->transform.SetPosition(1653.0f, 984.0f, 0.0f);
	ammoText->SetFontSize(40);
	ammoText->SetSize(150.0f, 90.0f);
	ammoText->SetSortOrder(10);
	ammoText->SetIgnoreFocus(true);
	ammoText->SetText("100/100");

	// 총 초상화
	RocketEngine::GameObject* gunSpriteObj = RocketEngine::CreateImageBox("gunSpriteObj");
	RocketEngine::SpriteRenderer* gunSprite = gunSpriteObj->GetComponent<RocketEngine::SpriteRenderer>();
	gunSpriteObj->transform.SetPosition(1799.0f, 940.0f, 0.0f);
	gunSprite->SetPath("..\\UI\\InGame\\Gun_Watermelon_188x153.png");
	gunSprite->SetSize(188.0f, 153.0f);
	gunSprite->SetSortOrder(10);

	// 총 아래 Bar Image
	RocketEngine::GameObject* bulletUnderBarImageObj = RocketEngine::CreateImageBox("bulletUnderBarImageObj");
	RocketEngine::SpriteRenderer* bulletUnderBarSprite = bulletUnderBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	bulletUnderBarImageObj->transform.SetPosition(1712.5f, 1041.5f, 0.0f);
	bulletUnderBarSprite->SetPath("..\\UI\\InGame\\AmmoGun_UnderBar_399x9.png");
	bulletUnderBarSprite->SetSize(399.0f, 9.0f);
	bulletUnderBarSprite->SetSortOrder(10);

	// Cross Hair (Aim)
	RocketEngine::GameObject* crossHairImageObj = RocketEngine::CreateImageBox("crossHairImageObj");
	RocketEngine::SpriteRenderer* crossHairSprite = crossHairImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	crossHairImageObj->transform.SetPosition(961.0f, 540.0f, 0.0f);
	crossHairSprite->SetPath("..\\UI\\Aim\\Aim.png");
	crossHairSprite->SetSize(100.0f, 100.0f);
	crossHairSprite->SetSortOrder(0);

	// Canvas Setting
	inGameBoardImageObj->transform.SetParent(inGameCanvas);
	player1KillObj->transform.SetParent(inGameCanvas);
	player2KillObj->transform.SetParent(inGameCanvas);
	player3KillObj->transform.SetParent(inGameCanvas);
	player4KillObj->transform.SetParent(inGameCanvas);
	roundTimeObj->transform.SetParent(inGameCanvas);
	playerStatusPaneImageObj->transform.SetParent(inGameCanvas);
	playerhpBarImageObj->transform.SetParent(inGameCanvas);
	ammoObj->transform.SetParent(inGameCanvas);
	gunSpriteObj->transform.SetParent(inGameCanvas);
	bulletUnderBarImageObj->transform.SetParent(inGameCanvas);
	crossHairImageObj->transform.SetParent(inGameCanvas);

	// Object 기본 상태 설정
	//inGameCanvas->Enable();

	_gameManager.AssignInGameCanvas(inGameCanvas);
	_gameManager.AssignPlayer1KillText(player1KillText);
	_gameManager.AssignPlayer2KillText(player2KillText);
	_gameManager.AssignPlayer3KillText(player3KillText);
	_gameManager.AssignPlayer4KillText(player4KillText);
	_gameManager.AssignRoundTimeText(roundTimeText);
	_gameManager.AssignPlayerSprite(playerStatusPaneSprite);
	_gameManager.AssignHPBarSprite(playerhpBarSprite);
	_gameManager.AssignAmmoText(ammoText);
	_gameManager.AssignGunSprite(gunSprite);

	//////////////////////////////////////////////////////////////////////////
}

void MainGameSceneView::DeathUI()
{
	/// Death UI
	RocketEngine::GameObject* deadCanvas = RocketEngine::CreateObject("deadCanvas");

	/// 파츠강화
	// 시간 제한 Text
	RocketEngine::GameObject* timeRemainObj = RocketEngine::CreateTextBox("timeRemainObj");
	RocketEngine::SpriteRenderer* timeRemainSprite = timeRemainObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* timeRemainText = timeRemainObj->GetComponent<RocketEngine::TextBox>();
	timeRemainObj->transform.SetLocalPosition(884.0f, 103.0f, 0.0f);
	timeRemainSprite->SetPath("..\\UI\\Card\\ClockBackground.png");
	timeRemainSprite->SetSize(396.0f, 166.0f);
	timeRemainSprite->SetSortOrder(10);
	timeRemainText->SetSize(396.0f, 166.0f);
	timeRemainText->SetSortOrder(15);
	timeRemainText->SetIgnoreFocus(true);
	timeRemainText->SetFontSize(50);
	timeRemainText->SetText("10");

	// 골드량 Image Text
	RocketEngine::GameObject* coinNumImageObj = RocketEngine::CreateImageBox("coinNumImageObj");
	RocketEngine::SpriteRenderer* coinNumSprite = coinNumImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	coinNumImageObj->transform.SetLocalPosition(1654.0f, 75.0f, 0.0f);
	coinNumSprite->SetPath("..\\UI\\Card\\CoinQuantity.png");
	coinNumSprite->SetSize(446.0f, 110.0f);
	coinNumSprite->SetSortOrder(10);

	// 골드량 Text (위치 조절이 자유롭지 않아 따로 뺌)
	RocketEngine::GameObject* coinNumTextObj = RocketEngine::CreateObject("coinNumTextObj");
	RocketEngine::TextBox* coinNumText = coinNumTextObj->AddComponent<RocketEngine::TextBox>();
	coinNumTextObj->transform.SetPosition(1804.0f, 75.0f, 0.0f);
	coinNumText->SetSize(200.0f, 110.0f);
	coinNumText->SetSortOrder(15);
	coinNumText->SetIgnoreFocus(true);
	coinNumText->SetFontSize(40);
	coinNumText->SetText("999");

	/// Card UI
	// Card UI Canvas
	RocketEngine::GameObject* cardCanvas = RocketEngine::CreateObject("cardCanvas");

	// 파츠 내역서 박스1
	RocketEngine::GameObject* partsObj1 = RocketEngine::CreateButton("partsObj1");
	RocketEngine::SpriteRenderer* partsBackSprite1 = partsObj1->GetComponent<RocketEngine::SpriteRenderer>();
	partsObj1->transform.SetLocalPosition(491.5f, 568.5f, 0.0f);
	partsBackSprite1->SetPath("..\\UI\\Card\\CardBackGround.png");
	partsBackSprite1->SetSize(523.0f, 787.0f);
	partsBackSprite1->SetSortOrder(35);

	// 박스1 자체가 버튼이 되도록
	RocketEngine::Button* partsButton1 = partsObj1->GetComponent<RocketEngine::Button>();
	partsButton1->SetSize(523.0f, 730.0f);
	partsButton1->SetSortOrder(60);

	// 파츠 아이콘 Image1
	RocketEngine::GameObject* partsIconObj1 = RocketEngine::CreateImageBox("partsIconObj1");
	RocketEngine::SpriteRenderer* partsIconSprite1 = partsIconObj1->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconObj1->transform.SetLocalPosition(494.0f, 509.0f, 0.0f);
	partsIconSprite1->SetPath("..\\UI\\Card\\PartsIcons\\Jumpx2.png");
	partsIconSprite1->SetSize(300.0f, 300.0f);
	partsIconSprite1->SetSortOrder(40);

	// 파츠 설명 박스1
	RocketEngine::GameObject* partsTextObj1 = RocketEngine::CreateObject("partsTextObj1");
	RocketEngine::TextBox* partsText1 = partsTextObj1->AddComponent<RocketEngine::TextBox>();
	partsTextObj1->transform.SetPosition(487.5f, 762.5f, 0.0f);
	partsText1->SetFontSize(18);
	partsText1->SetIgnoreFocus(true);
	partsText1->SetSize(441.0f, 65.0f);
	partsText1->SetSortOrder(40);

	// 파츠 내역서 박스2
	RocketEngine::GameObject* partsObj2 = RocketEngine::CreateButton("partsObj2");
	RocketEngine::SpriteRenderer* partsBackSprite2 = partsObj2->GetComponent<RocketEngine::SpriteRenderer>();
	partsObj2->transform.SetLocalPosition(985.5f, 568.5f, 0.0f);
	partsBackSprite2->SetPath("..\\UI\\Card\\CardBackGround.png");
	partsBackSprite2->SetSize(523.0f, 787.0f);
	partsBackSprite2->SetSortOrder(35);

	// 박스2 자체가 버튼이 되도록
	RocketEngine::Button* partsButton2 = partsObj2->GetComponent<RocketEngine::Button>();
	partsButton2->SetSize(523.0f, 730.0f);
	partsButton2->SetSortOrder(60);

	// 파츠 아이콘 Image2
	RocketEngine::GameObject* partsIconObj2 = RocketEngine::CreateImageBox("partsIconImageObj2");
	RocketEngine::SpriteRenderer* partsIconSprite2 = partsIconObj2->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconObj2->transform.SetLocalPosition(988.0f, 509.0f, 0.0f);
	partsIconSprite2->SetSize(300.0f, 300.0f);
	partsIconSprite2->SetSortOrder(40);

	// 파츠 설명 박스2
	RocketEngine::GameObject* partsTextObj2 = RocketEngine::CreateObject("partsTextObj2");
	RocketEngine::TextBox* partsText2 = partsTextObj2->AddComponent<RocketEngine::TextBox>();
	partsTextObj2->transform.SetPosition(983.5f, 762.5f, 0.0f);
	partsText2->SetFontSize(18);
	partsText2->SetIgnoreFocus(true);
	partsText2->SetSize(341.0f, 65.0f);
	partsText2->SetSortOrder(40);

	// 파츠 내역서 박스3
	RocketEngine::GameObject* partsObj3 = RocketEngine::CreateButton("partsObj3");
	RocketEngine::SpriteRenderer* partsBackSprite3 = partsObj3->GetComponent<RocketEngine::SpriteRenderer>();
	partsObj3->transform.SetLocalPosition(1479.5f, 568.5f, 0.0f);
	partsBackSprite3->SetPath("..\\UI\\Card\\CardBackGround.png");
	partsBackSprite3->SetSize(523.0f, 787.0f);
	partsBackSprite3->SetSortOrder(35);

	// 박스3 자체가 버튼이 되도록
	RocketEngine::Button* partsButton3 = partsObj3->GetComponent<RocketEngine::Button>();
	partsButton3->SetSize(523.0f, 730.0f);
	partsButton3->SetSortOrder(60);

	// 파츠 아이콘 Image3
	RocketEngine::GameObject* partsIconObj3 = RocketEngine::CreateImageBox("partsIconImageObj3");
	RocketEngine::SpriteRenderer* partsIconSprite3 = partsIconObj3->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconObj3->transform.SetLocalPosition(1482.0f, 509.0f, 0.0f);
	partsIconSprite3->SetSize(300.0f, 300.0f);
	partsIconSprite3->SetSortOrder(40);

	// 파츠 설명 박스3
	RocketEngine::GameObject* partsTextObj3 = RocketEngine::CreateObject("partsTextObj3");
	RocketEngine::TextBox* partsText3 = partsTextObj3->AddComponent<RocketEngine::TextBox>();
	partsTextObj3->transform.SetPosition(1481.5f, 762.5f, 0.0f);
	partsText3->SetFontSize(18);
	partsText3->SetSize(341.0f, 65.0f);
	partsText3->SetSortOrder(40);

	// 부활 버튼
	RocketEngine::GameObject* reviveBtnObj = RocketEngine::CreateButton("reviveBtnObj");
	RocketEngine::SpriteRenderer* reviveSprite = reviveBtnObj->GetComponent<RocketEngine::SpriteRenderer>();
	reviveBtnObj->transform.SetLocalPosition(989.0f, 994.5f, 0.0f);
	reviveSprite->SetPath("..\\UI\\Card\\Revival_Button.png");	// 즉시부활 문구 들어있는 버튼으로 변경 필요
	reviveSprite->SetSize(198.0f, 149.0f);
	reviveSprite->SetSortOrder(35);

	RocketEngine::Button* reviveButton = reviveBtnObj->GetComponent<RocketEngine::Button>();
	reviveButton->SetSize(198.0f, 100.0f);
	reviveButton->SetSortOrder(60);

	// 리롤 버튼
	RocketEngine::GameObject* rerollBtnObj = RocketEngine::CreateButton("reviveBtnObj");
	RocketEngine::SpriteRenderer* rerollSprite = rerollBtnObj->GetComponent<RocketEngine::SpriteRenderer>();
	rerollBtnObj->transform.SetLocalPosition(1300.0f, 1000.0f, 0.0f);
	rerollSprite->SetPath("..\\UI\\Card\\Re-roll_Button.png");	// 즉시부활 문구 들어있는 버튼으로 변경 필요
	rerollSprite->SetSize(176.0f, 125.0f);
	rerollSprite->SetSortOrder(35);

	RocketEngine::Button* rerollButton = rerollBtnObj->GetComponent<RocketEngine::Button>();
	rerollButton->SetSize(176.0f, 125.0f);
	rerollButton->SetSortOrder(60);

	// Canvas Setting
	partsIconObj1->transform.SetParent(partsObj1);
	partsTextObj1->transform.SetParent(partsObj1);
	partsButton1->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			GameManager::Instance().OnClickParts(0);
		}
	);

	partsIconObj2->transform.SetParent(partsObj2);
	partsTextObj2->transform.SetParent(partsObj2);
	partsButton2->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			GameManager::Instance().OnClickParts(1);
		}
	);
	partsIconObj3->transform.SetParent(partsObj3);
	partsTextObj3->transform.SetParent(partsObj3);
	partsButton3->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			GameManager::Instance().OnClickParts(2);
		}
	);

	reviveButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			GameManager::Instance().OnRevive();
		}
	);

	rerollButton->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			GameManager::Instance().OnReroll();
		}
	);

	// Canvas Setting
	timeRemainObj->transform.SetParent(deadCanvas);
	coinNumImageObj->transform.SetParent(deadCanvas);
	coinNumTextObj->transform.SetParent(deadCanvas);

	cardCanvas->transform.SetParent(deadCanvas);
	reviveBtnObj->transform.SetParent(deadCanvas);
	rerollBtnObj->transform.SetParent(deadCanvas);

	partsObj1->transform.SetParent(cardCanvas);
	partsObj2->transform.SetParent(cardCanvas);
	partsObj3->transform.SetParent(cardCanvas);
// 	partsTextObj1->transform.SetParent(cardCanvas);
// 	partsTextObj2->transform.SetParent(cardCanvas);
// 	partsTextObj3->transform.SetParent(cardCanvas);

	// 데이터 설정
	_gameManager.AssignDeadCanvas(deadCanvas);
	_gameManager.AssignPartsSpriteInfo(partsIconSprite1, 0);
	_gameManager.AssignPartsSpriteInfo(partsIconSprite2, 1);
	_gameManager.AssignPartsSpriteInfo(partsIconSprite3, 2);
	_gameManager.AssignPartsTextInfo(partsText1, 0);
	_gameManager.AssignPartsTextInfo(partsText2, 1);
	_gameManager.AssignPartsTextInfo(partsText3, 2);

	_gameManager.AssignReviveTimeText(timeRemainText);
	_gameManager.AssignGoldText(coinNumText);


	// Object 기본 상태 설정
	deadCanvas->FlushDisable();
}

void MainGameSceneView::SettingUI()
{
	/// 게임 설정 창 (1안)

// 설정창 이미지
	RocketEngine::GameObject* settingCanvas = RocketEngine::CreateImageBox("settingCanvasInGame");
	RocketEngine::SpriteRenderer* settingCanvasSprite = settingCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	settingCanvas->transform.SetPosition(960.0f, 540.0f, 0.0f);
	settingCanvasSprite->SetPath("..\\UI\\Edit\\BackGround.png");
	settingCanvasSprite->SetSize(862.0f, 670.0f);
	settingCanvasSprite->SetSortOrder(20);

	// 닫기 버튼 삭제함

	//// 민감도 이미지
	//RocketEngine::GameObject* aimSensitivityImageObj = RocketEngine::CreateImageBox("aimSensitivityImageObj");
	//RocketEngine::SpriteRenderer* aimSensitivitySprite = aimSensitivityImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//aimSensitivityImageObj->transform.SetPosition(740.0f, 439.0f, 0.0f);	// 임의값
	//aimSensitivitySprite->SetPath("..\\UI\\Edit\\MinGamDo.png");
	//aimSensitivitySprite->SetSize(191.0f, 70.0f);
	//aimSensitivitySprite->SetSortOrder(20);

	// 민감도 Bar 이미지
	RocketEngine::GameObject* aimSensitivityBarImageObj = RocketEngine::CreateImageBox("aimSensitivityBarImageObjInGame");
	RocketEngine::SpriteRenderer* aimSensitivityBarSprite = aimSensitivityBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	aimSensitivityBarImageObj->transform.SetPosition(1057.0f, 451.0f, 0.0f);
	aimSensitivityBarSprite->SetPath("..\\UI\\Edit\\Bar.png");
	aimSensitivityBarSprite->SetSize(396.0f, 28.0f);
	aimSensitivityBarSprite->SetSortOrder(20);

	// 민감도 슬라이드 Bar Button
	RocketEngine::GameObject* aimSensitivitySlideBarButtonObj = RocketEngine::CreateButton("aimSensitivitySlideBarButtonObjInGame");
	RocketEngine::Button* aimSensitivitySlideBarButton = aimSensitivitySlideBarButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* aimSensitivitySlideBarButtonSprite = aimSensitivitySlideBarButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	aimSensitivitySlideBarButtonObj->transform.SetPosition(1073.5f, 450.5f, 0.0f);
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

	// 민감도 Down Button (에임 감도)
	RocketEngine::GameObject* aimSensitivityDownButtonObj = RocketEngine::CreateButton("aimSensitivityDownButtonObjInGame");
	RocketEngine::Button* aimSensitivityDownButton = aimSensitivityDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* aimSensitivityDownButtonSprite = aimSensitivityDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	aimSensitivityDownButtonObj->transform.SetPosition(815.5f, 451.0f, 0.0f);
	aimSensitivityDownButton->SetSize(71.0f, 66.0f);
	aimSensitivityDownButton->SetSortOrder(50);
	aimSensitivityDownButtonSprite->SetPath("..\\UI\\Edit\\Left.png");
	aimSensitivityDownButtonSprite->SetSize(71.0f, 66.0f);
	aimSensitivityDownButtonSprite->SetSortOrder(20);

	aimSensitivityDownButton->SetOnClickEvent(
		[aimSensitivitySlideBarButtonObj]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().DownSensitivity();
			if (aimSensitivitySlideBarButtonObj->transform.GetPosition().x > 888.0f)
			{
				aimSensitivitySlideBarButtonObj->transform.Translate(-18.5f, 0.0f, 0.0f);
			}
		});

	// 민감도 Up Button (에임 감도)
	RocketEngine::GameObject* aimSensitivityUpButtonObj = RocketEngine::CreateButton("aimSensitivityUpButtonObjInGame");
	RocketEngine::Button* aimSensitivityUpButton = aimSensitivityUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* aimSensitivityUpButtonSprite = aimSensitivityUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	aimSensitivityUpButtonObj->transform.SetPosition(1298.5f, 451.0f, 0.0f);
	aimSensitivityUpButton->SetSize(71.0f, 66.0f);
	aimSensitivityUpButton->SetSortOrder(50);
	aimSensitivityUpButtonSprite->SetPath("..\\UI\\Edit\\Right.png");
	aimSensitivityUpButtonSprite->SetSize(71.0f, 66.0f);
	aimSensitivityUpButtonSprite->SetSortOrder(20);

	aimSensitivityUpButton->SetOnClickEvent(
		[aimSensitivitySlideBarButtonObj]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().UpSensitivity();
			if (aimSensitivitySlideBarButtonObj->transform.GetPosition().x < 1222.0f)
			{
				aimSensitivitySlideBarButtonObj->transform.Translate(18.5f, 0.0f, 0.0f);
			}
		});



	//// 배경음 Image
	//RocketEngine::GameObject* backGroundSoundImageObj = RocketEngine::CreateTextBox("backGroundSoundImageObj");
	//RocketEngine::SpriteRenderer* backGroundSoundSprite = backGroundSoundImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//backGroundSoundImageObj->transform.SetPosition(740.0f, 607.0f, 0.0f);		// 임의값
	//backGroundSoundSprite->SetPath("..\\UI\\Edit\\BaeGyeonguem.png");
	//backGroundSoundSprite->SetSize(191.0f, 70.0f);
	//backGroundSoundSprite->SetSortOrder(20);

	// 배경음 Bar 이미지
	RocketEngine::GameObject* backGroundSoundBarImageObj = RocketEngine::CreateImageBox("backGroundSoundBarImageObjInGame");
	RocketEngine::SpriteRenderer* backGroundSoundBarSprite = backGroundSoundBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	backGroundSoundBarImageObj->transform.SetPosition(1057.0f, 570.0f, 0.0f);
	backGroundSoundBarSprite->SetPath("..\\UI\\Edit\\Bar.png");
	backGroundSoundBarSprite->SetSize(396.0f, 28.0f);
	backGroundSoundBarSprite->SetSortOrder(20);

	// 배경음 슬라이드 Bar Button
	RocketEngine::GameObject* backGroundSoundSlideBarButtonObj = RocketEngine::CreateButton("backGroundSoundSlideBarButtonObjInGame");
	RocketEngine::Button* backGroundSoundSlideBarButton = backGroundSoundSlideBarButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* backGroundSoundSlideBarButtonSprite = backGroundSoundSlideBarButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	backGroundSoundSlideBarButtonObj->transform.SetPosition(1073.5f, 569.5f, 0.0f);
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

	// 배경음 Down Button
	RocketEngine::GameObject* backGroundSoundDownButtonObj = RocketEngine::CreateButton("backGroundSoundDownButtonObjInGame");
	RocketEngine::Button* backGroundSoundDownButton = backGroundSoundDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* backGroundSoundDownButtonSprite = backGroundSoundDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	backGroundSoundDownButtonObj->transform.SetPosition(815.5f, 570.0f, 0.0f);
	backGroundSoundDownButton->SetSize(71.0f, 66.0f);
	backGroundSoundDownButton->SetSortOrder(50);
	backGroundSoundDownButtonSprite->SetPath("..\\UI\\Edit\\Left.png");
	backGroundSoundDownButtonSprite->SetSize(71.0f, 66.0f);
	backGroundSoundDownButtonSprite->SetSortOrder(20);

	backGroundSoundDownButton->SetOnClickEvent(
		[backGroundSoundSlideBarButtonObj]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().DownBGM();
			if (backGroundSoundSlideBarButtonObj->transform.GetPosition().x > 888.0f)
			{
				backGroundSoundSlideBarButtonObj->transform.Translate(-18.5f, 0.0f, 0.0f);
			}
		}
	);

	// 배경음 Up Button
	RocketEngine::GameObject* backGroundSoundUpButtonObj = RocketEngine::CreateButton("backGroundSoundUpButtonObjInGame");
	RocketEngine::Button* backGroundSoundUpButton = backGroundSoundUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* backGroundSoundUpButtonSprite = backGroundSoundUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	backGroundSoundUpButtonObj->transform.SetPosition(1298.5f, 570.0f, 0.0f);
	backGroundSoundUpButton->SetSize(71.0f, 66.0f);
	backGroundSoundUpButton->SetSortOrder(50);
	backGroundSoundUpButtonSprite->SetPath("..\\UI\\Edit\\Right.png");
	backGroundSoundUpButtonSprite->SetSize(71.0f, 66.0f);
	backGroundSoundUpButtonSprite->SetSortOrder(20);

	backGroundSoundUpButton->SetOnClickEvent(
		[backGroundSoundSlideBarButtonObj]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().UpBGM();
			if (backGroundSoundSlideBarButtonObj->transform.GetPosition().x < 1222.0f)
			{
				backGroundSoundSlideBarButtonObj->transform.Translate(18.5f, 0.0f, 0.0f);
			}
		}
	);


	//// 효과음 Image
	//RocketEngine::GameObject* effectSoundImageObj = RocketEngine::CreateTextBox("effectSoundImageObj");
	//RocketEngine::SpriteRenderer* effectSoundSprite = effectSoundImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//effectSoundImageObj->transform.SetPosition(740.0f, 763.0f, 0.0f);		// 임의값
	//effectSoundSprite->SetPath("..\\UI\\Edit\\HyoGyauem.png");
	//effectSoundSprite->SetSize(191.0f, 70.0f);
	//effectSoundSprite->SetSortOrder(20);

		// 효과음 Bar 이미지
	RocketEngine::GameObject* effectSoundBarImageObj = RocketEngine::CreateImageBox("effectSoundBarImageObjInGame");
	RocketEngine::SpriteRenderer* effectSoundBarSprite = effectSoundBarImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	effectSoundBarImageObj->transform.SetPosition(1057.0f, 691.0f, 0.0f);
	effectSoundBarSprite->SetPath("..\\UI\\Edit\\Bar.png");
	effectSoundBarSprite->SetSize(396.0f, 28.0f);
	effectSoundBarSprite->SetSortOrder(20);

	// 효과음 슬라이드 Bar Button
	RocketEngine::GameObject* effectSoundSlideBarButtonObj = RocketEngine::CreateButton("effectSoundSlideBarButtonObjInGame");
	RocketEngine::Button* effectSoundSlideBarButton = effectSoundSlideBarButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* effectSoundSlideBarButtonSprite = effectSoundSlideBarButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	effectSoundSlideBarButtonObj->transform.SetPosition(1073.5f, 691.0f, 0.0f);
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

	// 효과음 Down Button
	RocketEngine::GameObject* effectSoundDownButtonObj = RocketEngine::CreateButton("effectSoundDownButtonObjInGame");
	RocketEngine::Button* effectSoundDownButton = effectSoundDownButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* effectSoundDownButtonSprite = effectSoundDownButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	effectSoundDownButtonObj->transform.SetPosition(815.5f, 691.0f, 0.0f);
	effectSoundDownButton->SetSize(71.0f, 66.0f);
	effectSoundDownButton->SetSortOrder(50);
	effectSoundDownButtonSprite->SetPath("..\\UI\\Edit\\Left.png");
	effectSoundDownButtonSprite->SetSize(71.0f, 66.0f);
	effectSoundDownButtonSprite->SetSortOrder(20);

	effectSoundDownButton->SetOnClickEvent(
		[effectSoundSlideBarButtonObj]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().DownEffectSound();
			if (effectSoundSlideBarButtonObj->transform.GetPosition().x >888.0f)
			{
				effectSoundSlideBarButtonObj->transform.Translate(-18.5f, 0.0f, 0.0f);
			}
		}
	);

	// 효과음 Up Button
	RocketEngine::GameObject* effectSoundUpButtonObj = RocketEngine::CreateButton("effectSoundUpButtonObjInGame");
	RocketEngine::Button* effectSoundUpButton = effectSoundUpButtonObj->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* effectSoundUpButtonSprite = effectSoundUpButtonObj->GetComponent<RocketEngine::SpriteRenderer>();
	effectSoundUpButtonObj->transform.SetPosition(1298.5f, 691.0f, 0.0f);
	effectSoundUpButtonSprite->SetPath("..\\UI\\Edit\\Right.png");
	effectSoundUpButton->SetSize(71.0f, 66.0f);
	effectSoundUpButton->SetSortOrder(50);
	effectSoundUpButtonSprite->SetSize(71.0f, 66.0f);
	effectSoundUpButtonSprite->SetSortOrder(20);

	effectSoundUpButton->SetOnClickEvent(
		[effectSoundSlideBarButtonObj]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
			LobbyManager::Instance().UpEffectSound();
			if (effectSoundSlideBarButtonObj->transform.GetPosition().x < 1222.0f)
			{
				effectSoundSlideBarButtonObj->transform.Translate(18.5f, 0.0f, 0.0f);
			}
		}
	);
	
	// Option Obj (부모 : optionCanvas)
	//aimSensitivityImageObj->transform.SetParent(settingCanvas);
	aimSensitivityDownButtonObj->transform.SetParent(settingCanvas);
	aimSensitivityUpButtonObj->transform.SetParent(settingCanvas);
	aimSensitivityBarImageObj->transform.SetParent(settingCanvas);
	aimSensitivitySlideBarButtonObj->transform.SetParent(settingCanvas);
	//backGroundSoundImageObj->transform.SetParent(settingCanvas);
	backGroundSoundDownButtonObj->transform.SetParent(settingCanvas);
	backGroundSoundUpButtonObj->transform.SetParent(settingCanvas);
	backGroundSoundBarImageObj->transform.SetParent(settingCanvas);
	backGroundSoundSlideBarButtonObj->transform.SetParent(settingCanvas);
	//effectSoundImageObj->transform.SetParent(settingCanvas);
	effectSoundDownButtonObj->transform.SetParent(settingCanvas);
	effectSoundUpButtonObj->transform.SetParent(settingCanvas);
	effectSoundBarImageObj->transform.SetParent(settingCanvas);
	effectSoundSlideBarButtonObj->transform.SetParent(settingCanvas);

	_gameManager.SetSettingCanvas(settingCanvas);
}

void MainGameSceneView::RoundEnd()
{
	/// Object, Renderer 객체 생성
	// 라운드 종료 창
	RocketEngine::GameObject* roundEndImageObj = RocketEngine::CreateImageBox("roundEndImageObj");
	RocketEngine::SpriteRenderer* roundEndSprite = roundEndImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	roundEndImageObj->transform.SetPosition(960.0f, 540.0f, 0.0f);
	roundEndSprite->SetPath("..\\UI\\RoundOver\\RoundOver_only_1920x433.png");
	roundEndSprite->SetSize(1920.0f, 433.0f);
	roundEndSprite->SetSortOrder(0);

	// 기본 상태 설정
	roundEndImageObj->FlushDisable();

	GameManager::Instance().AssignRoundOverObj(roundEndImageObj);
}

void MainGameSceneView::RoundResultUI()
{
	/// RoundResults UI
	// Rouns Results Canvas
	RocketEngine::GameObject* roundResultCanvas = RocketEngine::CreateImageBox("roundResultCanvas");
	RocketEngine::SpriteRenderer* roundResultCanvasSprite = roundResultCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	roundResultCanvas->transform.SetPosition(964.0f, 544.0f, 0.0f);
	roundResultCanvasSprite->SetPath("..\\UI\\Result\\RoundResult_background.png");
	roundResultCanvasSprite->SetSize(862.0f, 670.0f);
	roundResultCanvasSprite->SetSortOrder(0);

	// Player1 순위
	RocketEngine::GameObject* playerRankingTextObj1 = RocketEngine::CreateObject("playerRankingTextObj1");
	RocketEngine::TextBox* playerRankingText1 = playerRankingTextObj1->AddComponent<RocketEngine::TextBox>();
	playerRankingTextObj1->transform.SetPosition(696.0f, 449.0f, 0.0f);
	playerRankingText1->SetText("1");
	playerRankingText1->SetFontSize(24);
	playerRankingText1->SetSize(100.0f, 100.0f);
	playerRankingText1->SetSortOrder(10);

	// Player2 순위
	RocketEngine::GameObject* playerRankingTextObj2 = RocketEngine::CreateObject("playerRankingTextObj2");
	RocketEngine::TextBox* playerRankingText2 = playerRankingTextObj2->AddComponent<RocketEngine::TextBox>();
	playerRankingTextObj2->transform.SetPosition(696.0f, 558.0f, 0.0f);
	playerRankingText2->SetText("2");
	playerRankingText2->SetFontSize(24);
	playerRankingText2->SetSize(100.0f, 100.0f);
	playerRankingText2->SetSortOrder(10);

	// Player3 순위
	RocketEngine::GameObject* playerRankingTextObj3 = RocketEngine::CreateObject("playerRankingTextObj3");
	RocketEngine::TextBox* playerRankingText3 = playerRankingTextObj3->AddComponent<RocketEngine::TextBox>();
	playerRankingTextObj3->transform.SetPosition(696.0f, 667.0f, 0.0f);
	playerRankingText3->SetText("3");
	playerRankingText3->SetFontSize(24);
	playerRankingText3->SetSize(100.0f, 100.0f);
	playerRankingText3->SetSortOrder(10);

	// Player4 순위
	RocketEngine::GameObject* playerRankingTextObj4 = RocketEngine::CreateObject("playerRankingTextObj4");
	RocketEngine::TextBox* playerRankingText4 = playerRankingTextObj4->AddComponent<RocketEngine::TextBox>();
	playerRankingTextObj4->transform.SetPosition(696.0f, 776.0f, 0.0f);
	playerRankingText4->SetText("4");
	playerRankingText4->SetFontSize(24);
	playerRankingText4->SetSize(100.0f, 100.0f);
	playerRankingText4->SetSortOrder(10);

	// Player1 프로필
	RocketEngine::GameObject* playerProfileImageObj1 = RocketEngine::CreateImageBox("playerProfileImageObj1");
	RocketEngine::SpriteRenderer* playerProfileSprite1 = playerProfileImageObj1->AddComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj1->transform.SetPosition(827.0f, 449.0f, 0.0f);
	playerProfileSprite1->SetPath("..\\UI\\portrait\\portrait_watermelon(100x100).png");;
	playerProfileSprite1->SetSize(100.0f, 100.0f);
	playerProfileSprite1->SetSortOrder(10);

	// Player2 프로필
	RocketEngine::GameObject* playerProfileImageObj2 = RocketEngine::CreateImageBox("playerProfileImageObj2");
	RocketEngine::SpriteRenderer* playerProfileSprite2 = playerProfileImageObj2->AddComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj2->transform.SetPosition(827.0f, 558.0f, 0.0f);
	playerProfileSprite2->SetPath("..\\UI\\portrait\\portrait_kiwi(100x100).png");;
	playerProfileSprite2->SetSize(100.0f, 100.0f);
	playerProfileSprite2->SetSortOrder(10);

	// Player3 프로필
	RocketEngine::GameObject* playerProfileImageObj3 = RocketEngine::CreateImageBox("playerProfileImageObj3");
	RocketEngine::SpriteRenderer* playerProfileSprite3 = playerProfileImageObj3->AddComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj3->transform.SetPosition(827.0f, 667.0f, 0.0f);
	playerProfileSprite3->SetPath("..\\UI\\portrait\\portrait_carrot(100x100).png");;
	playerProfileSprite3->SetSize(100.0f, 100.0f);
	playerProfileSprite3->SetSortOrder(10);

	// Player4 프로필
	RocketEngine::GameObject* playerProfileImageObj4 = RocketEngine::CreateImageBox("playerProfileImageObj4");
	RocketEngine::SpriteRenderer* playerProfileSprite4 = playerProfileImageObj4->AddComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj4->transform.SetPosition(827.0f, 776.0f, 0.0f);
	playerProfileSprite4->SetPath("..\\UI\\portrait\\portrait_banana(100x100).png");;
	playerProfileSprite4->SetSize(100.0f, 100.0f);
	playerProfileSprite4->SetSortOrder(10);

	// Player1 킬 Text
	RocketEngine::GameObject* playerkillTextObj1 = RocketEngine::CreateObject("playerkillTextObj1");
	RocketEngine::TextBox* playerkillText1 = playerkillTextObj1->AddComponent<RocketEngine::TextBox>();
	playerkillTextObj1->transform.SetPosition(1000.0f, 449.0f, 0.0f);
	playerkillText1->SetText("1");
	playerkillText1->SetFontSize(24);
	playerkillText1->SetSize(100.0f, 100.0f);
	playerkillText1->SetSortOrder(10);

	// Player2 킬 Text
	RocketEngine::GameObject* playerkillTextObj2 = RocketEngine::CreateObject("playerkillTextObj2");
	RocketEngine::TextBox* playerkillText2 = playerkillTextObj2->AddComponent<RocketEngine::TextBox>();
	playerkillTextObj2->transform.SetPosition(1000.0f, 558.0f, 0.0f);
	playerkillText2->SetText("1");
	playerkillText2->SetFontSize(24);
	playerkillText2->SetSize(100.0f, 100.0f);
	playerkillText2->SetSortOrder(10);

	// Player3 킬 Text
	RocketEngine::GameObject* playerkillTextObj3 = RocketEngine::CreateObject("playerkillTextObj3");
	RocketEngine::TextBox* playerkillText3 = playerkillTextObj3->AddComponent<RocketEngine::TextBox>();
	playerkillTextObj3->transform.SetPosition(1000.0f, 667.0f, 0.0f);
	playerkillText3->SetText("1");
	playerkillText3->SetFontSize(24);
	playerkillText3->SetSize(100.0f, 100.0f);
	playerkillText3->SetSortOrder(10);

	// Player4 킬 Text
	RocketEngine::GameObject* playerkillTextObj4 = RocketEngine::CreateObject("playerkillTextObj4");
	RocketEngine::TextBox* playerkillText4 = playerkillTextObj4->AddComponent<RocketEngine::TextBox>();
	playerkillTextObj4->transform.SetPosition(1000.0f, 776.0f, 0.0f);
	playerkillText4->SetText("1");
	playerkillText4->SetFontSize(24);
	playerkillText4->SetSize(100.0f, 100.0f);
	playerkillText4->SetSortOrder(10);

	// Player1 데스 Text
	RocketEngine::GameObject* playerDeathTextObj1 = RocketEngine::CreateObject("playerDeathTextObj1");
	RocketEngine::TextBox* playerDeathText1 = playerDeathTextObj1->AddComponent<RocketEngine::TextBox>();
	playerDeathTextObj1->transform.SetPosition(1137.0f, 449.0f, 0.0f);
	playerDeathText1->SetText("1");
	playerDeathText1->SetFontSize(24);
	playerDeathText1->SetSize(100.0f, 100.0f);
	playerDeathText1->SetSortOrder(10);

	// Player2 데스 Text
	RocketEngine::GameObject* playerDeathTextObj2 = RocketEngine::CreateObject("playerDeathTextObj2");
	RocketEngine::TextBox* playerDeathText2 = playerDeathTextObj2->AddComponent<RocketEngine::TextBox>();
	playerDeathTextObj2->transform.SetPosition(1137.0f, 558.0f, 0.0f);
	playerDeathText2->SetText("1");
	playerDeathText2->SetFontSize(24);
	playerDeathText2->SetSize(100.0f, 100.0f);
	playerDeathText2->SetSortOrder(10);

	// Player3 데스 Text
	RocketEngine::GameObject* playerDeathTextObj3 = RocketEngine::CreateObject("playerDeathTextObj3");
	RocketEngine::TextBox* playerDeathText3 = playerDeathTextObj3->AddComponent<RocketEngine::TextBox>();
	playerDeathTextObj3->transform.SetPosition(1137.0f, 667.0f, 0.0f);
	playerDeathText3->SetText("1");
	playerDeathText3->SetFontSize(24);
	playerDeathText3->SetSize(100.0f, 100.0f);
	playerDeathText3->SetSortOrder(10);

	// Player4 데스 Text
	RocketEngine::GameObject* playerDeathTextObj4 = RocketEngine::CreateObject("playerDeathTextObj4");
	RocketEngine::TextBox* playerDeathText4 = playerDeathTextObj4->AddComponent<RocketEngine::TextBox>();
	playerDeathTextObj4->transform.SetPosition(1137.0f, 776.0f, 0.0f);
	playerDeathText4->SetText("1");
	playerDeathText4->SetFontSize(24);
	playerDeathText4->SetSize(100.0f, 100.0f);
	playerDeathText4->SetSortOrder(10);

	// Player1 골드 Text
	RocketEngine::GameObject* playerGoldTextObj1 = RocketEngine::CreateObject("playerGoldTextObj1");
	RocketEngine::TextBox* playerGoldText1 = playerGoldTextObj1->AddComponent<RocketEngine::TextBox>();
	playerGoldTextObj1->transform.SetPosition(1275.0f, 449.0f, 0.0f);
	playerGoldText1->SetText("1");
	playerGoldText1->SetFontSize(24);
	playerGoldText1->SetSize(100.0f, 100.0f);
	playerGoldText1->SetSortOrder(10);

	// Player2 골드 Text
	RocketEngine::GameObject* playerGoldTextObj2 = RocketEngine::CreateObject("playerGoldTextObj2");
	RocketEngine::TextBox* playerGoldText2 = playerGoldTextObj2->AddComponent<RocketEngine::TextBox>();
	playerGoldTextObj2->transform.SetPosition(1275.0f, 558.0f, 0.0f);
	playerGoldText2->SetText("1");
	playerGoldText2->SetFontSize(24);
	playerGoldText2->SetSize(100.0f, 100.0f);
	playerGoldText2->SetSortOrder(10);

	// Player3 골드 Text
	RocketEngine::GameObject* playerGoldTextObj3 = RocketEngine::CreateObject("playerGoldTextObj3");
	RocketEngine::TextBox* playerGoldText3 = playerGoldTextObj3->AddComponent<RocketEngine::TextBox>();
	playerGoldTextObj3->transform.SetPosition(1275.0f, 667.0f, 0.0f);
	playerGoldText3->SetText("1");
	playerGoldText3->SetFontSize(24);
	playerGoldText3->SetSize(100.0f, 100.0f);
	playerGoldText3->SetSortOrder(10);

	// Player4 골드 Text
	RocketEngine::GameObject* playerGoldTextObj4 = RocketEngine::CreateObject("playerGoldTextObj4");
	RocketEngine::TextBox* playerGoldText4 = playerGoldTextObj4->AddComponent<RocketEngine::TextBox>();
	playerGoldTextObj4->transform.SetPosition(1275.0f, 776.0f, 0.0f);
	playerGoldText4->SetText("1");
	playerGoldText4->SetFontSize(24);
	playerGoldText4->SetSize(100.0f, 100.0f);
	playerGoldText4->SetSortOrder(10);

	/// Object 묶기
	playerRankingTextObj1->transform.SetParent(roundResultCanvas);
	playerRankingTextObj2->transform.SetParent(roundResultCanvas);
	playerRankingTextObj3->transform.SetParent(roundResultCanvas);
	playerRankingTextObj4->transform.SetParent(roundResultCanvas);
	playerProfileImageObj1->transform.SetParent(roundResultCanvas);
	playerProfileImageObj2->transform.SetParent(roundResultCanvas);
	playerProfileImageObj3->transform.SetParent(roundResultCanvas);
	playerProfileImageObj4->transform.SetParent(roundResultCanvas);
	playerkillTextObj1->transform.SetParent(roundResultCanvas);
	playerkillTextObj2->transform.SetParent(roundResultCanvas);
	playerkillTextObj3->transform.SetParent(roundResultCanvas);
	playerkillTextObj4->transform.SetParent(roundResultCanvas);
	playerDeathTextObj1->transform.SetParent(roundResultCanvas);
	playerDeathTextObj2->transform.SetParent(roundResultCanvas);
	playerDeathTextObj3->transform.SetParent(roundResultCanvas);
	playerDeathTextObj4->transform.SetParent(roundResultCanvas);
	playerGoldTextObj1->transform.SetParent(roundResultCanvas);
	playerGoldTextObj2->transform.SetParent(roundResultCanvas);
	playerGoldTextObj3->transform.SetParent(roundResultCanvas);
	playerGoldTextObj4->transform.SetParent(roundResultCanvas);

	/// Object 상태
	roundResultCanvas->Enable();
}

void MainGameSceneView::TotalResultUI()
{
	/// Round Total UI
	// Rouns Results Canvas
	RocketEngine::GameObject* roundTotalCanvas = RocketEngine::CreateImageBox("roundTotalCanvas");
	RocketEngine::SpriteRenderer* roundTotalCanvasSprite = roundTotalCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	roundTotalCanvas->transform.SetPosition(964.0f, 544.0f, 0.0f);
	roundTotalCanvasSprite->SetPath("..\\UI\\Result\\Result_background.png");
	roundTotalCanvasSprite->SetSize(862.0f, 670.0f);
	roundTotalCanvasSprite->SetSortOrder(5);

	// Player1 순위
	RocketEngine::GameObject* playerRankingTextObj1 = RocketEngine::CreateObject("playerRankingTextObj1");
	RocketEngine::TextBox* playerRankingText1 = playerRankingTextObj1->AddComponent<RocketEngine::TextBox>();
	playerRankingTextObj1->transform.SetPosition(696.0f, 449.0f, 0.0f);
	playerRankingText1->SetText("1");
	playerRankingText1->SetFontSize(24);
	playerRankingText1->SetSize(100.0f, 100.0f);
	playerRankingText1->SetSortOrder(10);
	playerRankingText1->SetIgnoreFocus(true);

	// Player2 순위
	RocketEngine::GameObject* playerRankingTextObj2 = RocketEngine::CreateObject("playerRankingTextObj2");
	RocketEngine::TextBox* playerRankingText2 = playerRankingTextObj2->AddComponent<RocketEngine::TextBox>();
	playerRankingTextObj2->transform.SetPosition(696.0f, 558.0f, 0.0f);
	playerRankingText2->SetText("2");
	playerRankingText2->SetFontSize(24);
	playerRankingText2->SetSize(100.0f, 100.0f);
	playerRankingText2->SetSortOrder(10);
	playerRankingText2->SetIgnoreFocus(true);

	// Player3 순위
	RocketEngine::GameObject* playerRankingTextObj3 = RocketEngine::CreateObject("playerRankingTextObj3");
	RocketEngine::TextBox* playerRankingText3 = playerRankingTextObj3->AddComponent<RocketEngine::TextBox>();
	playerRankingTextObj3->transform.SetPosition(696.0f, 667.0f, 0.0f);
	playerRankingText3->SetText("3");
	playerRankingText3->SetFontSize(24);
	playerRankingText3->SetSize(100.0f, 100.0f);
	playerRankingText3->SetSortOrder(10);
	playerRankingText3->SetIgnoreFocus(true);

	// Player4 순위
	RocketEngine::GameObject* playerRankingTextObj4 = RocketEngine::CreateObject("playerRankingTextObj4");
	RocketEngine::TextBox* playerRankingText4 = playerRankingTextObj4->AddComponent<RocketEngine::TextBox>();
	playerRankingTextObj4->transform.SetPosition(696.0f, 776.0f, 0.0f);
	playerRankingText4->SetText("4");
	playerRankingText4->SetFontSize(24);
	playerRankingText4->SetSize(100.0f, 100.0f);
	playerRankingText4->SetSortOrder(10);

	// Player1 프로필
	RocketEngine::GameObject* playerProfileImageObj1 = RocketEngine::CreateImageBox("playerProfileImageObj1");
	RocketEngine::SpriteRenderer* playerProfileSprite1 = playerProfileImageObj1->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj1->transform.SetPosition(827.0f, 449.0f, 0.0f);
	playerProfileSprite1->SetPath("..\\UI\\portrait\\portrait_watermelon(100x100).png");
	playerProfileSprite1->SetSize(100.0f, 100.0f);
	playerProfileSprite1->SetSortOrder(10);

	// Player2 프로필
	RocketEngine::GameObject* playerProfileImageObj2 = RocketEngine::CreateImageBox("playerProfileImageObj2");
	RocketEngine::SpriteRenderer* playerProfileSprite2 = playerProfileImageObj2->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj2->transform.SetPosition(827.0f, 558.0f, 0.0f);
	playerProfileSprite2->SetPath("..\\UI\\portrait\\portrait_kiwi(100x100).png");
	playerProfileSprite2->SetSize(100.0f, 100.0f);
	playerProfileSprite2->SetSortOrder(10);

	// Player3 프로필
	RocketEngine::GameObject* playerProfileImageObj3 = RocketEngine::CreateImageBox("playerProfileImageObj3");
	RocketEngine::SpriteRenderer* playerProfileSprite3 = playerProfileImageObj3->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj3->transform.SetPosition(827.0f, 667.0f, 0.0f);
	playerProfileSprite3->SetPath("..\\UI\\portrait\\portrait_carrot(100x100).png");
	playerProfileSprite3->SetSize(100.0f, 100.0f);
	playerProfileSprite3->SetSortOrder(10);

	// Player4 프로필
	RocketEngine::GameObject* playerProfileImageObj4 = RocketEngine::CreateImageBox("playerProfileImageObj4");
	RocketEngine::SpriteRenderer* playerProfileSprite4 = playerProfileImageObj4->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj4->transform.SetPosition(827.0f, 776.0f, 0.0f);
	playerProfileSprite4->SetPath("..\\UI\\portrait\\portrait_banana(100x100).png");
	playerProfileSprite4->SetSize(100.0f, 100.0f);
	playerProfileSprite4->SetSortOrder(10);

	// Player1 킬 Text
	RocketEngine::GameObject* playerkillTextObj1 = RocketEngine::CreateObject("playerkillTextObj1");
	RocketEngine::TextBox* playerkillText1 = playerkillTextObj1->AddComponent<RocketEngine::TextBox>();
	playerkillTextObj1->transform.SetPosition(1000.0f, 449.0f, 0.0f);
	playerkillText1->SetText("1");
	playerkillText1->SetFontSize(24);
	playerkillText1->SetSize(100.0f, 100.0f);
	playerkillText1->SetSortOrder(10);
	playerkillText1->SetIgnoreFocus(true);

	// Player2 킬 Text
	RocketEngine::GameObject* playerkillTextObj2 = RocketEngine::CreateObject("playerkillTextObj2");
	RocketEngine::TextBox* playerkillText2 = playerkillTextObj2->AddComponent<RocketEngine::TextBox>();
	playerkillTextObj2->transform.SetPosition(1000.0f, 558.0f, 0.0f);
	playerkillText2->SetText("1");
	playerkillText2->SetFontSize(24);
	playerkillText2->SetSize(100.0f, 100.0f);
	playerkillText2->SetSortOrder(10);
	playerkillText2->SetIgnoreFocus(true);

	// Player3 킬 Text
	RocketEngine::GameObject* playerkillTextObj3 = RocketEngine::CreateObject("playerkillTextObj3");
	RocketEngine::TextBox* playerkillText3 = playerkillTextObj3->AddComponent<RocketEngine::TextBox>();
	playerkillTextObj3->transform.SetPosition(1000.0f, 667.0f, 0.0f);
	playerkillText3->SetText("1");
	playerkillText3->SetFontSize(24);
	playerkillText3->SetSize(100.0f, 100.0f);
	playerkillText3->SetSortOrder(10);
	playerkillText3->SetIgnoreFocus(true);

	// Player4 킬 Text
	RocketEngine::GameObject* playerkillTextObj4 = RocketEngine::CreateObject("playerkillTextObj4");
	RocketEngine::TextBox* playerkillText4 = playerkillTextObj4->AddComponent<RocketEngine::TextBox>();
	playerkillTextObj4->transform.SetPosition(1000.0f, 776.0f, 0.0f);
	playerkillText4->SetText("1");
	playerkillText4->SetFontSize(24);
	playerkillText4->SetSize(100.0f, 100.0f);
	playerkillText4->SetSortOrder(10);
	playerkillText4->SetIgnoreFocus(true);

	// Player1 데스 Text
	RocketEngine::GameObject* playerDeathTextObj1 = RocketEngine::CreateObject("playerDeathTextObj1");
	RocketEngine::TextBox* playerDeathText1 = playerDeathTextObj1->AddComponent<RocketEngine::TextBox>();
	playerDeathTextObj1->transform.SetPosition(1137.0f, 449.0f, 0.0f);
	playerDeathText1->SetText("1");
	playerDeathText1->SetFontSize(24);
	playerDeathText1->SetSize(100.0f, 100.0f);
	playerDeathText1->SetSortOrder(10);
	playerDeathText1->SetIgnoreFocus(true);

	// Player2 데스 Text
	RocketEngine::GameObject* playerDeathTextObj2 = RocketEngine::CreateObject("playerDeathTextObj2");
	RocketEngine::TextBox* playerDeathText2 = playerDeathTextObj2->AddComponent<RocketEngine::TextBox>();
	playerDeathTextObj2->transform.SetPosition(1137.0f, 558.0f, 0.0f);
	playerDeathText2->SetText("1");
	playerDeathText2->SetFontSize(24);
	playerDeathText2->SetSize(100.0f, 100.0f);
	playerDeathText2->SetSortOrder(10);
	playerDeathText2->SetIgnoreFocus(true);

	// Player3 데스 Text
	RocketEngine::GameObject* playerDeathTextObj3 = RocketEngine::CreateObject("playerDeathTextObj3");
	RocketEngine::TextBox* playerDeathText3 = playerDeathTextObj3->AddComponent<RocketEngine::TextBox>();
	playerDeathTextObj3->transform.SetPosition(1137.0f, 667.0f, 0.0f);
	playerDeathText3->SetText("1");
	playerDeathText3->SetFontSize(24);
	playerDeathText3->SetSize(100.0f, 100.0f);
	playerDeathText3->SetSortOrder(10);
	playerDeathText3->SetIgnoreFocus(true);

	// Player4 데스 Text
	RocketEngine::GameObject* playerDeathTextObj4 = RocketEngine::CreateObject("playerDeathTextObj4");
	RocketEngine::TextBox* playerDeathText4 = playerDeathTextObj4->AddComponent<RocketEngine::TextBox>();
	playerDeathTextObj4->transform.SetPosition(1137.0f, 776.0f, 0.0f);
	playerDeathText4->SetText("1");
	playerDeathText4->SetFontSize(24);
	playerDeathText4->SetSize(100.0f, 100.0f);
	playerDeathText4->SetSortOrder(10);
	playerDeathText4->SetIgnoreFocus(true);

	// Player1 라운드 Text
	RocketEngine::GameObject* playerGoldTextObj1 = RocketEngine::CreateObject("playerGoldTextObj1");
	RocketEngine::TextBox* playerGoldText1 = playerGoldTextObj1->AddComponent<RocketEngine::TextBox>();
	playerGoldTextObj1->transform.SetPosition(1275.0f, 449.0f, 0.0f);
	playerGoldText1->SetText("1");
	playerGoldText1->SetFontSize(20);
	playerGoldText1->SetSize(100.0f, 100.0f);
	playerGoldText1->SetSortOrder(10);
	playerGoldText1->SetIgnoreFocus(true);


	// Player2 라운드 Text
	RocketEngine::GameObject* playerGoldTextObj2 = RocketEngine::CreateObject("playerGoldTextObj2");
	RocketEngine::TextBox* playerGoldText2 = playerGoldTextObj2->AddComponent<RocketEngine::TextBox>();
	playerGoldTextObj2->transform.SetPosition(1275.0f, 558.0f, 0.0f);
	playerGoldText2->SetText("1");
	playerGoldText2->SetFontSize(24);
	playerGoldText2->SetSize(100.0f, 100.0f);
	playerGoldText2->SetSortOrder(10);
	playerGoldText2->SetIgnoreFocus(true);

	// Player3 라운드 Text
	RocketEngine::GameObject* playerGoldTextObj3 = RocketEngine::CreateObject("playerGoldTextObj3");
	RocketEngine::TextBox* playerGoldText3 = playerGoldTextObj3->AddComponent<RocketEngine::TextBox>();
	playerGoldTextObj3->transform.SetPosition(1275.0f, 667.0f, 0.0f);
	playerGoldText3->SetText("1");
	playerGoldText3->SetFontSize(24);
	playerGoldText3->SetSize(100.0f, 100.0f);
	playerGoldText3->SetSortOrder(10);
	playerGoldText3->SetIgnoreFocus(true);

	// Player4 라운드 Text
	RocketEngine::GameObject* playerGoldTextObj4 = RocketEngine::CreateObject("playerGoldTextObj4");
	RocketEngine::TextBox* playerGoldText4 = playerGoldTextObj4->AddComponent<RocketEngine::TextBox>();
	playerGoldTextObj4->transform.SetPosition(1275.0f, 776.0f, 0.0f);
	playerGoldText4->SetText("1");
	playerGoldText4->SetFontSize(24);
	playerGoldText4->SetSize(100.0f, 100.0f);
	playerGoldText4->SetSortOrder(10);
	playerGoldText4->SetIgnoreFocus(true);

	RocketEngine::GameObject* returnLobbyBtnObj = RocketEngine::CreateButton("returnLobbyBtnObj");
	RocketEngine::SpriteRenderer* returnLobbyBtnSprite = returnLobbyBtnObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::Button* returnLobbyButton = returnLobbyBtnObj->GetComponent<RocketEngine::Button>();
	returnLobbyBtnObj->transform.SetLocalPosition(960.0f, 900.0f, 0.0f);
	returnLobbyBtnSprite->SetPath("..\\UI\\MainMenu\\Exit_213x211.png");	// 즉시부활 문구 들어있는 버튼으로 변경 필요
	returnLobbyBtnSprite->SetSize(213.0f, 211.0f);
	returnLobbyBtnSprite->SetSortOrder(35);
	returnLobbyButton->SetSize(213.0f, 211.0f);
	returnLobbyButton->SetSortOrder(60);

	returnLobbyButton->SetOnClickEvent(
		[]()
		{
			GameManager::Instance().ReturnToLobby();
		});


	/// Object 묶기
	playerRankingTextObj1->transform.SetParent(roundTotalCanvas);
	playerRankingTextObj2->transform.SetParent(roundTotalCanvas);
	playerRankingTextObj3->transform.SetParent(roundTotalCanvas);
	playerRankingTextObj4->transform.SetParent(roundTotalCanvas);
	playerProfileImageObj1->transform.SetParent(roundTotalCanvas);
	playerProfileImageObj2->transform.SetParent(roundTotalCanvas);
	playerProfileImageObj3->transform.SetParent(roundTotalCanvas);
	playerProfileImageObj4->transform.SetParent(roundTotalCanvas);
	playerkillTextObj1->transform.SetParent(roundTotalCanvas);
	playerkillTextObj2->transform.SetParent(roundTotalCanvas);
	playerkillTextObj3->transform.SetParent(roundTotalCanvas);
	playerkillTextObj4->transform.SetParent(roundTotalCanvas);
	playerDeathTextObj1->transform.SetParent(roundTotalCanvas);
	playerDeathTextObj2->transform.SetParent(roundTotalCanvas);
	playerDeathTextObj3->transform.SetParent(roundTotalCanvas);
	playerDeathTextObj4->transform.SetParent(roundTotalCanvas);
	playerGoldTextObj1->transform.SetParent(roundTotalCanvas);
	playerGoldTextObj2->transform.SetParent(roundTotalCanvas);
	playerGoldTextObj3->transform.SetParent(roundTotalCanvas);
	playerGoldTextObj4->transform.SetParent(roundTotalCanvas);
	returnLobbyBtnObj->transform.SetParent(roundTotalCanvas);

	/// Object 상태
	roundTotalCanvas->FlushDisable();

	GameManager::Instance().AssignRoundTotalCanvas(roundTotalCanvas);

	GameManager::Instance().AssignRankProfile(playerProfileSprite1, 0);
	GameManager::Instance().AssignRankProfile(playerProfileSprite2, 1);
	GameManager::Instance().AssignRankProfile(playerProfileSprite3, 2);
	GameManager::Instance().AssignRankProfile(playerProfileSprite4, 3);

	GameManager::Instance().AssignRankKillText(playerkillText1, 0);
	GameManager::Instance().AssignRankKillText(playerkillText2, 1);
	GameManager::Instance().AssignRankKillText(playerkillText3, 2);
	GameManager::Instance().AssignRankKillText(playerkillText4, 3);

	GameManager::Instance().AssignRankDeathText(playerDeathText1, 0);
	GameManager::Instance().AssignRankDeathText(playerDeathText2, 1);
	GameManager::Instance().AssignRankDeathText(playerDeathText3, 2);
	GameManager::Instance().AssignRankDeathText(playerDeathText4, 3);

	GameManager::Instance().AssignRankGoldText(playerGoldText1, 0);
	GameManager::Instance().AssignRankGoldText(playerGoldText2, 1);
	GameManager::Instance().AssignRankGoldText(playerGoldText3, 2);
	GameManager::Instance().AssignRankGoldText(playerGoldText4, 3);
}
