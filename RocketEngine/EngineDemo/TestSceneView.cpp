#include "TestSceneView.h"
#include "RocketAPILoader.h"
#include "CameraMove.h"
#include "PlayerMoveTest.h"
#include "NetworkManager.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

TestSceneView::TestSceneView()
	: scene()
{ 

}

void TestSceneView::Initialize()
{
	scene = RocketEngine::CreateScene("TEST");
	assert(scene);
	/*
	{
		RocketEngine::GameObject* cameraObj = scene->GetMainCamera()->gameObject;
		//cameraObj->AddComponent<RocketEngine::Raycast>();

		/// StaticPlane 생성
		RocketEngine::GameObject* plane = RocketEngine::CreateObject("plane");
		RocketEngine::PlaneCollider* planeCol = plane->AddComponent<RocketEngine::PlaneCollider>();
		plane->transform.Rotate(0.0f, 0.0f, 90.0f);

		/// SkinnedMeshObject 생성 및 테스트
		RocketEngine::GameObject* skinObject = RocketEngine::CreateObject("skinObject");
		RocketEngine::SkinnedMeshRenderer* skinRenderer = skinObject->AddComponent<RocketEngine::SkinnedMeshRenderer>();
		RocketEngine::AnimationController* animController = skinObject->AddComponent<RocketEngine::AnimationController>();

		skinRenderer->SetMeshPath("../3DModels/Timmy/TimmyModel.fbx");
		skinRenderer->SetShaderPath("../FX/Default/SkinnedMeshTex.cso");
		skinRenderer->SetDiffusePath("../3DModels/Timmy/Textures/Timmy_Diffuse.png");

		// Animator 및 State 생성.
		RocketEngine::Animator* skinAnimator = new RocketEngine::Animator();
		RocketEngine::State* testState = skinAnimator->AddNewState("test");

		testState->SetAnimDescName("testAnim");
		testState->SetAnimDescAnimPath("../3DModels/Timmy/Animations/Timmy_Shooting.fbx");
		testState->SetAnimDescOwnerPath("../3DModels/Timmy/TimmyModel.fbx");
		testState->SetLoop(true);

		skinAnimator->SetInitState(testState->GetStateName());

		// AnimationController에 Animator 부착.
		animController->SetAnimator(skinAnimator);


		// 	animController->SetAnimDataName("testAnim");
		// 	animController->SetAnimDataLoop(true);
		// 	//[TW] AnimDesc에도 이름을 배정해줘야 시스템 발동, 추가했음.
		// 	animController->SetAnimDescName("testAnim");
		// 	animController->SetAnimDataLoop(false);
		// 	//animController->SetAnimDescAnimPath("../3DModels/Timmy/Animations/Timmy_DeathFromFrontHeadshot.fbx");
		// 	animController->SetAnimDescAnimPath("../3DModels/Timmy/Animations/Timmy_Shooting.fbx");
		// 	animController->SetAnimDescOwnerPath("../3DModels/Timmy/TimmyModel.fbx");

		// 	skinRenderer->SetMeshPath("../3DModels/Cylinder/Cylinder.fbx");
		// 	skinRenderer->SetShaderPath("../FX/Default/SkinnedMeshTex.cso");
		// 	skinRenderer->SetDiffusePath("../3DModels/Cylinder/Textures/WoodCrate01.dds");
		// 
		// 	animController->SetAnimDataName("testAnim");
		// 	//[TW] AnimDesc에도 이름을 배정해줘야 시스템 발동, 추가했음.
		// 	animController->SetAnimDescName("testAnim");
		// 	animController->SetAnimDescAnimPath("../3DModels/Cylinder/Cylinder.fbx");
		// 	animController->SetAnimDescOwnerPath("../3DModels/Cylinder/Cylinder.fbx");

		skinObject->transform.SetLocalScale(0.1f, 0.1f, 0.1f);

		skinObject->transform.SetLocalPosition(0.0f, 0.0f, 0.0f);

		/// MeshObject 생성 및 테스트1

		//RocketEngine::GameObject* mokujin = _rocket.CreateObject();
		RocketEngine::GameObject* mokujin = RocketEngine::CreateObject("mokujin");
		RocketEngine::MeshRenderer* mokujinRenderer = mokujin->AddComponent<RocketEngine::MeshRenderer>();
		RocketEngine::BoxCollider* mokujinCollider = mokujin->AddComponent<RocketEngine::BoxCollider>();
		PlayerMoveTest* playerMove = mokujin->AddComponent<PlayerMoveTest>();
		playerMove->moveSpeed = 100.0f;

		mokujinRenderer->SetMeshPath("../3DModels/MokujinRigged/MokujinRigged.fbx");
		mokujinRenderer->SetShaderPath("../FX/Default/PosNormalTex.cso");
		mokujinRenderer->SetDiffusePath("../3DModels/MokujinRigged/Textures/Mokujin Base Color.png");
		mokujinRenderer->SetNormalPath("../3DModels/MokujinRigged/Textures/Mokujin Normal.png");

		mokujinRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
		mokujinRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
		mokujinRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
		mokujinRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });

		mokujinCollider->SetVolume(5.0f, 15.0f, 5.0f);

		mokujin->transform.SetLocalPosition(5.0f, 10.0f, 0.0f);
		mokujin->transform.SetLocalScale(0.1f, 0.1f, 0.1f);

		// Tween 테스트1

		//RocketEngine::Tween* mokujinTween = _rocket.CreateTween();
	//	RocketEngine::Tween* mokujinTween = RocketEngine::CreateTween();
	//	mokujinTween->GetData(&(mokujin->transform.GetLocalScaleRef())).DoMove({0.15f,0.15f,0.15f}, 1.5f).Delay(2.0f).SetEase(RocketEngine::OUTBOUNCE);

		/// MeshObject 생성 및 테스트2

		//RocketEngine::GameObject* mokujin2 = _rocket.CreateObject();
		RocketEngine::GameObject* mokujin2 = RocketEngine::CreateObject("mokujin2");
		RocketEngine::MeshRenderer* mokujinRenderer2 = mokujin2->AddComponent<RocketEngine::MeshRenderer>();
		//RocketEngine::BoxCollider* mokujinCollider2 = mokujin2->AddComponent<RocketEngine::BoxCollider>();

		mokujinRenderer2->SetMeshPath("../3DModels/MokujinRigged/MokujinRigged.fbx");
		mokujinRenderer2->SetShaderPath("../FX/Default/PosNormalTex.cso");
		mokujinRenderer2->SetDiffusePath("../3DModels/MokujinRigged/Textures/Mokujin Base Color.png");
		mokujinRenderer2->SetNormalPath("../3DModels/MokujinRigged/Textures/Mokujin Normal.png");

		mokujinRenderer2->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
		mokujinRenderer2->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
		mokujinRenderer2->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
		mokujinRenderer2->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });

		//mokujinCollider2->SetVolume(5.0f, 15.0f, 5.0f);

		mokujin2->transform.SetPosition(-5.0f, 0.0f, 0.0f);
		mokujin2->transform.SetScale(0.1f, 0.1f, 0.1f);

		mokujin2->transform.SetParent(mokujin);


		// Tween 테스트2

		//RocketEngine::Tween* mokujinTween2 = _rocket.CreateTween();
	//	RocketEngine::Tween* mokujinTween2 = RocketEngine::CreateTween();
	//	mokujinTween2->GetData(&(mokujin2->transform.GetLocalPositionRef().x)).DoMove(-7.0f, 1.5f).Delay(0.5f).SetEase(RocketEngine::OUTELASTIC);

		/// TextRender 테스트
		RocketEngine::GameObject* textObject = RocketEngine::CreateObject("TextTest");
		RocketEngine::TextBox* textRenderer = textObject->AddComponent<RocketEngine::TextBox>();

		textObject->transform.SetLocalPosition(500.0f, 500.0f, 0.0f);

		/// SpriteRender 테스트
		RocketEngine::GameObject* ImageObject = RocketEngine::CreateObject("ImageTest");
		RocketEngine::SpriteRenderer* spriteRenderer = ImageObject->AddComponent<RocketEngine::SpriteRenderer>();
		RocketEngine::Button* imageButton = ImageObject->AddComponent<RocketEngine::Button>();

		ImageObject->transform.Translate({ 56.5f,65.5f,0.0f });
		spriteRenderer->SetPath("..\\GraphicsResources\\Images\\luffy.jpg");
		imageButton->SetOnClickEvent([ImageObject]() {ImageObject->transform.Translate(100.f, 100.f, 0.0f); });

		/// 카메라에 카메라 이동 컴포넌트 부착

		scene->GetMainCamera()->gameObject->AddComponent<CameraMove>();

		/// Light Test
		RocketEngine::GameObject* dirLightObj = RocketEngine::CreateObject("dirLight");
		RocketEngine::DirectionalLight* dirLight = dirLightObj->AddComponent<RocketEngine::DirectionalLight>();
		dirLightObj->transform.Rotate(45.0f, 0.0f, 0.0f);

		RocketEngine::GameObject* pointLightObj = RocketEngine::CreateObject("pointLight");
		RocketEngine::PointLight* pointLight = pointLightObj->AddComponent<RocketEngine::PointLight>();
		pointLightObj->transform.SetLocalPosition(-5.0f, 0.0f, -5.0f);

		RocketEngine::GameObject* spotLightObj = RocketEngine::CreateObject("spotLight");
		RocketEngine::SpotLight* spotLight = spotLightObj->AddComponent<RocketEngine::SpotLight>();
		spotLightObj->transform.SetLocalPosition(5.0f, 0.0f, 5.0f);
		spotLightObj->transform.Rotate(0.0f, -45.0f, 0.0f);
	}
	*/

	/// NetworkManager 생성 및 테스트
	/*RocketEngine::GameObject* NetManager = RocketEngine::CreateObject("NetMangerTest");
	NetManager->AddComponent<NetworkManager>();*/
	RocketEngine::AddStaticComponent(&(NetworkManager::Instance()));
	//NetworkManager::Instance().Start();
	// 
// 	RocketEngine::GameObject* NetManager = RocketEngine::CreateObject("NetMangerTest");
// 	NetManager->AddComponent<NetworkManager>();
	


	return;

	/// Stress Test
	{
		srand((unsigned int)time(NULL));

		for (int i = 0; i < 300; i++)
		{
			std::string tempName = "dummyWall_";
			tempName += i;
			RocketEngine::GameObject* temp = RocketEngine::CreateObject(tempName.c_str());
			RocketEngine::MeshRenderer* tempRenderer = temp->AddComponent<RocketEngine::MeshRenderer>();

			tempRenderer->SetMeshPath("../3DModels/DummyWall/build_dummy.fbx");
			tempRenderer->SetShaderPath("../FX/Default/BasicTex.cso");
			tempRenderer->SetDiffusePath("../3DModels/Box/Box0102_BaseColor.png"); //반드시 모든 Diffuse Path는 Set되어 있어야 합니다.

			int numX = rand();
			int numY = rand();
			int numZ = rand();
			temp->transform.SetLocalPosition({ (float)(numX % 50 - 25), (float)(15), (float)(numZ % 50 - 25) });
			//temp->transform.SetPosition(4.0f, 0.0f, 0.0f);
			temp->transform.SetLocalScale(0.01f, 0.01f, 0.01f);
		}
		//return;
		for (int i = 0; i < 300; i++)
		{
			std::string tempName = "dummyBox1_";
			tempName += i;
			RocketEngine::GameObject* temp = RocketEngine::CreateObject(tempName.c_str());
			RocketEngine::MeshRenderer* tempRenderer = temp->AddComponent<RocketEngine::MeshRenderer>();

			tempRenderer->SetMeshPath("../3DModels/Box/Box01.fbx");
			tempRenderer->SetShaderPath("../FX/Default/BasicTex.cso");
			tempRenderer->SetDiffusePath("../3DModels/Box/Box0102_BaseColor.png");

			int numX = rand();
			int numY = rand();
			int numZ = rand();
			temp->transform.SetLocalPosition({ (float)(numX % 50 - 25), (float)(10), (float)(numZ % 50 - 25) });
			//temp->transform.SetPosition(2.0f, 0.0f, 0.0f);
			temp->transform.SetLocalScale(0.01f, 0.01f, 0.01f);
		}

		for (int i = 0; i < 300; i++)
		{
			std::string tempName = "dummyBox2_";
			tempName += i;
			RocketEngine::GameObject* temp = RocketEngine::CreateObject(tempName.c_str());
			RocketEngine::MeshRenderer* tempRenderer = temp->AddComponent<RocketEngine::MeshRenderer>();

			tempRenderer->SetMeshPath("../3DModels/Box/Box02.fbx");
			tempRenderer->SetShaderPath("../FX/Default/BasicTex.cso");
			tempRenderer->SetDiffusePath("../3DModels/Box/Box0102_ARM.png");

			int numX = rand();
			int numY = rand();
			int numZ = rand();
			temp->transform.SetLocalPosition({ (float)(numX % 50 - 25), (float)(5), (float)(numZ % 50 - 25) });
			//temp->transform.SetPosition(4.0f, 0.0f, 0.0f);
			temp->transform.SetLocalScale(0.01f, 0.01f, 0.01f);
		}
	}
	
}
