#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#include "TestSceneView.h"
#include "..\\EngineDemo\\RocketAPILoader.h"
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

TestSceneView::TestSceneView()
	: scene()
{ 

}


/// 추후 비율 조정을 위한 값들
float posRatio = 0.01f;
float scaleRatio = 0.8f;
float defaultHeight = 0.0f;
RocketEngine::RMFLOAT3 tempPos = { 0.0f,0.0f,0.0f };

// Floor
std::vector<std::vector<RocketEngine::GameObject*>> floorObjectVec(4, std::vector<RocketEngine::GameObject*>(4));
std::vector<RocketEngine::MeshRenderer*> floorRendererVec(16);

void TestSceneView::Initialize()
{
	scene = RocketEngine::CreateScene("TestForPBRjson");

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
