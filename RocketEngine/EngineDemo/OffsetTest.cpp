#include "OffsetTest.h"
#include "RocketAPILoader.h"

#include "DebugCameraMove.h"
#include "HierarchyController.h"

OffsetTest::OffsetTest()
	: scene()
{

}

void OffsetTest::Initialize()
{
	scene = RocketEngine::CreateScene("OffsetTest");
	RocketEngine::GetMainCamera()->gameObject->AddComponent<DebugCameraMove>();
	RocketEngine::GetMainCamera()->gameObject->transform.SetPosition(0.0f, 3.0f, -10.0f);
	RocketEngine::GetMainCamera()->gameObject->transform.SetRotation({ 1.0f,0.0f,0.0f, 0.0f });

	RocketEngine::GameObject* plane = RocketEngine::CreateObject("plane");
	RocketEngine::PlaneCollider* planeCol = plane->AddComponent<RocketEngine::PlaneCollider>();
	plane->transform.Rotate(0.0f, 0.0f, 90.0f);
	 
	// B Object
	RocketEngine::GameObject* offsetTest = RocketEngine::CreateObject("offsetTest");
	offsetTest->AddComponent<HierarchyController>();
	 
	RocketEngine::MeshRenderer* renderer = offsetTest->AddComponent<RocketEngine::MeshRenderer>();
	renderer->SetMeshPath("../3DModels/Fruit/Watermelon.fbx");
	renderer->SetShaderPath("../FX/Default/PosNormalTex.cso");
	renderer->SetDiffusePath("../3DModels/Fruit/Fruit_BaseColor.png");
	renderer->SetNormalPath("../3DModels/Fruit/Fruit_Normal.png");
	renderer->SetRenderType(RocketCore::Graphics::eRenderType::FORWARD);
	 
	renderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	renderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	renderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	renderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });

// 	auto col = offsetTest->AddComponent<RocketEngine::SphereCollider>();
// 	col->SetPositionOffset({1.0f, 0.0f, 0.0f});
// 	col->SetRadius(0.5f);

	auto col = offsetTest->AddComponent<RocketEngine::CapsuleCollider>();
	col->SetPositionOffset({ 0.0f, 1.0f, 0.0f });
	col->SetRotationOffset( 0.0f,0.0f,90.0f );
	col->SetCapsuleInfo(0.5f, 0.5f);
}
