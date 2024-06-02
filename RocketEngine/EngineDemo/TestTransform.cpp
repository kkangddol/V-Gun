#include "TestTransform.h"
#include "RocketAPILoader.h"
#include "DebugCameraMove.h"
#include "HierarchyController.h"
#include "../RocketGraphicsInterface/RenderType.h" //[TW]
#include "../RocketMath/RocketMath.h"
#include "SceneLoadTest.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

TestTransform::TestTransform()
	: scene()
{

}

void TestTransform::Initialize()
{
	scene = RocketEngine::CreateScene("TESTTRANSFORM");

	RocketEngine::GetMainCamera()->gameObject->AddComponent<DebugCameraMove>();

	/// 물체들?
	// StaticPlane 생성
	RocketEngine::GameObject* plane = RocketEngine::CreateObject("plane");
	RocketEngine::PlaneCollider* planeCol = plane->AddComponent<RocketEngine::PlaneCollider>();
	plane->transform.Rotate(0.0f, 0.0f, 90.0f);

	/// 자식 테스트
	// Player
	//RocketEngine::GameObject* player = RocketEngine::CreateObject("player");
	//HierarchyController* pCtrl = player->AddComponent<HierarchyController>();
	//
	//RocketEngine::MeshRenderer* playerRenderer = player->AddComponent<RocketEngine::MeshRenderer>();
	//playerRenderer->SetMeshPath("../3DModels/Player/4QCharacter_tpose4.fbx");
	////playerRenderer->SetShaderPath("../FX/Default/BasicTex.cso");
	//playerRenderer->SetShaderPath("../FX/Default/PosNormalTex.cso");
	////playerRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture/watermelon/TT_checker_2048x2048_UV_GRID_BaseColor.png");
	////playerRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture/watermelon/TT_checker_2048x2048_UV_GRID_Normal.png");
	//playerRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture2/banana/TT_checker_2048x2048_COLOR_GRID_BaseColor.png");
	//playerRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture2/banana/TT_checker_2048x2048_COLOR_GRID_Normal.png");
	//playerRenderer->SetRenderType(RocketCore::Graphics::eRenderType::FORWARD);
	//
	//playerRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	//playerRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	//playerRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	//playerRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	//
	//RocketEngine::BoxCollider* playerCol = player->AddComponent<RocketEngine::BoxCollider>();
	//
	//// 자식
	//RocketEngine::GameObject* child = RocketEngine::CreateObject("child");
	//child->transform.SetPosition(0.0f, 1.5f, 0.0f);
	////child->transform.SetRotationEuler(0.0f, 45.0f, 45.0f);
	//
	//RocketEngine::MeshRenderer* childRenderer = child->AddComponent<RocketEngine::MeshRenderer>();
	//childRenderer->SetMeshPath("../3DModels/Player/4QCharacter_tpose4.fbx");
	////childRenderer->SetShaderPath("../FX/Default/BasicTex.cso");
	//childRenderer->SetShaderPath("../FX/Default/PosNormalTex.cso");
	////childRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture/banana/TT_checker_2048x2048_UV_GRID_BaseColor.png");
	////childRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture/banana/TT_checker_2048x2048_UV_GRID_Normal.png");
	//childRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture2/watermelon/TT_checker_2048x2048_COLOR_GRID_BaseColor.png");
	//childRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture2/watermelon/TT_checker_2048x2048_COLOR_GRID_Normal.png");
	//childRenderer->SetRenderType(RocketCore::Graphics::eRenderType::FORWARD);
	//
	//childRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	//childRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	//childRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	//childRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });

	//RocketEngine::BoxCollider* childCol = child->AddComponent<RocketEngine::BoxCollider>();
	///LIGHTS
	RocketEngine::GameObject* dirLightObj = RocketEngine::CreateObject("dirLight");
	RocketEngine::DirectionalLight* dirLight = dirLightObj->AddComponent<RocketEngine::DirectionalLight>();
	dirLight->SetAmbient(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	dirLight->SetDiffuse(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	dirLight->SetIntensity(0.5f);
	//dirLight->SetIntensity(2.0f);
	dirLight->SetSpecular(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	dirLightObj->transform.Rotate(180.0f, 0.0f, 0.0f);

	RocketEngine::GameObject* dirLightObj2 = RocketEngine::CreateObject("dirLight");
	RocketEngine::DirectionalLight* dirLight2 = dirLightObj2->AddComponent<RocketEngine::DirectionalLight>();
	dirLight2->SetAmbient(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	dirLight2->SetDiffuse(RocketEngine::RMFLOAT4(1.0f, 0.1f, 1.0f, 1.0f));
	dirLight2->SetIntensity(0.5f);
	//dirLight2->SetIntensity(2.0f);
	dirLight2->SetSpecular(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	dirLightObj2->transform.Rotate(20.0f, 0.0f, 0.0f);

	///
	
	//pCtrl->childSave = &(child->transform);
	//
	//playerCol->SetFixedJoint(childCol);


	///[TW] Deferred 렌더링을 위해 230815 추가.
	RocketEngine::GameObject* childDeferred = RocketEngine::CreateObject("childDeferred");
	childDeferred->transform.SetPosition(0.0f, 3.5f, 1.0f);

	RocketEngine::MeshRenderer* deferredChildRenderer = childDeferred->AddComponent<RocketEngine::MeshRenderer>();
	deferredChildRenderer->SetMeshPath("../3DModels/Player/4QCharacter_tpose4.fbx");
	deferredChildRenderer->SetShaderPath("../FX/Default/DeferredPhongShader.cso");

	deferredChildRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture2/watermelon/TT_checker_2048x2048_COLOR_GRID_BaseColor.png");
	deferredChildRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture2/watermelon/TT_checker_2048x2048_COLOR_GRID_Normal.png");
	deferredChildRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PHONG);

	deferredChildRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	deferredChildRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	deferredChildRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	deferredChildRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });


	///PBR 옵젝 테스트
	RocketEngine::GameObject* temp_childDeferred = RocketEngine::CreateObject("PBR_OBJECT_DEFERRED");
	temp_childDeferred->transform.SetPosition(10.0f, 3.5f, 1.0f);

	RocketEngine::MeshRenderer* temp_deferredChildRenderer = temp_childDeferred->AddComponent<RocketEngine::MeshRenderer>();
	temp_deferredChildRenderer->SetMeshPath("../3DModels/Box/Box01.fbx");
	temp_deferredChildRenderer->SetShaderPath("../FX/Default/BasicPBR.cso");
	//childRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture/banana/TT_checker_2048x2048_UV_GRID_BaseColor.png");
	//childRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture/banana/TT_checker_2048x2048_UV_GRID_Normal.png");
	temp_deferredChildRenderer->SetDiffusePath("../3DModels/Box/Box0102_BaseColor.png");
	temp_deferredChildRenderer->SetNormalPath("../3DModels/Box/Box0102_Normal.png");
	temp_deferredChildRenderer->SetARMPath("../3DModels/Box/Box0102_ARM.png");
	temp_deferredChildRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);

	temp_deferredChildRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	temp_deferredChildRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	temp_deferredChildRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	temp_deferredChildRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });

	///[TW] 유나님이 보내주신 캐릭터 돌아가는지 테스트해봄 여기에!
	RocketEngine::GameObject* testMainChar = RocketEngine::CreateObject("PleaseWorkPlease");
	//testMainChar->transform.SetLocalPosition(0.1f, -0.4f, 0.4f);
	//testMainChar->transform.SetLocalRotationEuler(80.0f, -90.0f, 5.0f);
	testMainChar->AddComponent<SceneLoadTest>();

	RocketEngine::SkinnedMeshRenderer* tGunRenderer = testMainChar->AddComponent<RocketEngine::SkinnedMeshRenderer>();
	tGunRenderer->SetMeshPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
	//tGunRenderer->SetMeshPath("../3DModels/Timmy/TimmyModel.fbx");
	tGunRenderer->SetShaderPath("../FX/Default/BasicPBRSkinned.cso");

	tGunRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture3/banana/BaseColor.png");
	tGunRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture3/banana/Normal.png");
	tGunRenderer->SetARMPath("../3DModels/Player/Vgun_character_texture3/banana/ARM.png");

	//tGunRenderer->SetDiffusePath("../3DModels/Timmy/Textures/Timmy_Diffuse.png");
	//tGunRenderer->SetNormalPath("../3DModels/Timmy/Textures/Timmy_Normal.png");
	//tGunRenderer->SetARMPath("../3DModels/Timmy/Textures/Timmy_Glossiness.png");

	tGunRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	tGunRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	tGunRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	tGunRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });

	tGunRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR_SKINNED);

	RocketEngine::AnimationController* tAnimController = testMainChar->AddComponent<RocketEngine::AnimationController>();

	// Animator 및 State 생성.
	RocketEngine::Animator* tSkinAnimator = new RocketEngine::Animator();
	RocketEngine::State* tTestState = tSkinAnimator->AddNewState("test");

	tTestState->SetAnimDataName("testAnim");
	tTestState->SetAnimDescName("testAnim");
	//tTestState->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_sidestep_ani.fbx");
	//tTestState->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_jump_ani.fbx");
	//tTestState->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_die_ani.fbx");
	tTestState->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_reload_ani.fbx");
	tTestState->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
	
	//tTestState->SetAnimDescAnimPath("../3DModels/Timmy/Animations/Timmy_FlyPunch.fbx");
	//tTestState->SetAnimDescAnimPath("../3DModels/Timmy/Animations/Timmy_HipHopDancing.fbx");
	//tTestState->SetAnimDescOwnerPath("../3DModels/Timmy/TimmyModel.fbx");

	tTestState->SetLoop(true);

	tSkinAnimator->SetInitState(tTestState->GetStateName());

	// AnimationController에 Animator 부착.
	tAnimController->SetAnimator(tSkinAnimator);

	auto FillRectTest = RocketEngine::CreateImageBox("FillRectTest");
	auto fillTestSprite = FillRectTest->GetComponent<RocketEngine::SpriteRenderer>();
	FillRectTest->transform.SetPosition(784.0f, 81.0f, 0.0f);
	fillTestSprite->SetSize(113.f, 131.0f);
	//fillTestSprite->SetFillRatio(20.0f);
	fillTestSprite->SetFillRatio(50.0f);
	//fillTestSprite->SetFillRatio(70.0f);



	RocketEngine::GameObject* vgunObject = RocketEngine::CreateObject("VGun");
	//vgunObject->transform.SetLocalPosition(0.25f, -0.38f, 0.75f);
	//vgunObject->transform.SetRotationEuler(2.0f, 135.0f, 5.0f);
	vgunObject->transform.SetLocalPosition(5.0f, 5.0f, 5.0f);
	vgunObject->transform.SetRotationEuler(0.0f, -50.0f, -5.0f);

	RocketEngine::SkinnedMeshRenderer* gunRenderer = vgunObject->AddComponent<RocketEngine::SkinnedMeshRenderer>();
	gunRenderer->SetMeshPath("../3DModels/FirstPersonShooting/fps_modelong_shooting_ani.fbx");
	//gunRenderer->SetShaderPath("../FX/Default/SkinnedMeshTex.cso");
	gunRenderer->SetShaderPath("../FX/Default/BasicPBRSkinned.cso");
	gunRenderer->SetDiffusePath("../3DModels/FirstPersonShooting/the_first_person_texture/banana/basecolor.png");
	//gunRenderer->SetDiffusePath("../3DModels/FirstPersonShooting/the_first_person_texture/banana/nomal.png");
	//gunRenderer->SetDiffusePath("../3DModels/Player/Textures/banana/TT_checker_2048x2048_UV_GRID_BaseColor.png");
	gunRenderer->SetNormalPath("../3DModels/FirstPersonShooting/the_first_person_texture/banana/nomal.png");
	//gunRenderer->SetNormalPath("../3DModels/Player/Textures/banana/TT_checker_2048x2048_UV_GRID_Normal.png");
	//gunRenderer->SetARMPath("../3DModels/FirstPersonShooting/the_first_person_texture/banana/arm.png");
	gunRenderer->SetARMPath("../3DModels/Box/Box0102_ARM.png");
	gunRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	gunRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	gunRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	gunRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	gunRenderer->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR_SKINNED);

	RocketEngine::AnimationController* gun_tAnimController = vgunObject->AddComponent<RocketEngine::AnimationController>();

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
	gun_tAnimController->SetAnimator(gunAnimator);






	//{
	//	///
	//	RocketEngine::GameObject* testMainChar2 = RocketEngine::CreateObject("PleaseWorkPlease2");
	//	testMainChar2->transform.SetLocalPosition(3.0f, 0.0f, 0.0f);
	//	//testMainChar->transform.SetLocalRotationEuler(80.0f, -90.0f, 5.0f);
	//
	//	RocketEngine::SkinnedMeshRenderer* tGunRenderer2 = testMainChar2->AddComponent<RocketEngine::SkinnedMeshRenderer>();
	//	tGunRenderer2->SetMeshPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
	//	tGunRenderer2->SetShaderPath("../FX/Default/BasicPBRSkinned.cso");
	//	tGunRenderer2->SetDiffusePath("../3DModels/Player/Vgun_character_texture3/banana/BaseColor.png");
	//	tGunRenderer2->SetNormalPath("../3DModels/Player/Vgun_character_texture3/banana/Normal.png");
	//	tGunRenderer2->SetARMPath("../3DModels/Player/Vgun_character_texture3/banana/ARM.png");
	//	tGunRenderer2->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	//	tGunRenderer2->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	//	tGunRenderer2->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	//	tGunRenderer2->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	//
	//	tGunRenderer2->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR_SKINNED);
	//
	//	RocketEngine::AnimationController* tAnimController2 = testMainChar2->AddComponent<RocketEngine::AnimationController>();
	//
	//	// Animator 및 State 생성.
	//	RocketEngine::Animator* tSkinAnimator2 = new RocketEngine::Animator();
	//	RocketEngine::State* tTestState2 = tSkinAnimator2->AddNewState("test2");
	//
	//	tTestState2->SetAnimDataName("testAnim2");
	//	tTestState2->SetAnimDescName("testAnim2");
	//	tTestState2->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_jump_ani.fbx");
	//	tTestState2->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
	//	tTestState2->SetLoop(true);
	//
	//	tSkinAnimator2->SetInitState(tTestState2->GetStateName());
	//
	//	// AnimationController에 Animator 부착.
	//	tAnimController2->SetAnimator(tSkinAnimator2);
	//}
	//{
	//	///
	//	RocketEngine::GameObject* testMainChar2 = RocketEngine::CreateObject("PleaseWorkPlease3");
	//	testMainChar2->transform.SetLocalPosition(-3.0f, 0.0f, 0.0f);
	//	//testMainChar->transform.SetLocalRotationEuler(80.0f, -90.0f, 5.0f);
	//
	//	RocketEngine::SkinnedMeshRenderer* tGunRenderer2 = testMainChar2->AddComponent<RocketEngine::SkinnedMeshRenderer>();
	//	tGunRenderer2->SetMeshPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
	//	tGunRenderer2->SetShaderPath("../FX/Default/BasicPBRSkinned.cso");
	//	tGunRenderer2->SetDiffusePath("../3DModels/Player/Vgun_character_texture3/banana/BaseColor.png");
	//	tGunRenderer2->SetNormalPath("../3DModels/Player/Vgun_character_texture3/banana/Normal.png");
	//	tGunRenderer2->SetARMPath("../3DModels/Player/Vgun_character_texture3/banana/ARM.png");
	//	tGunRenderer2->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	//	tGunRenderer2->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	//	tGunRenderer2->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	//	tGunRenderer2->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });
	//
	//	tGunRenderer2->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR_SKINNED);
	//
	//	RocketEngine::AnimationController* tAnimController2 = testMainChar2->AddComponent<RocketEngine::AnimationController>();
	//
	//	// Animator 및 State 생성.
	//	RocketEngine::Animator* tSkinAnimator2 = new RocketEngine::Animator();
	//	RocketEngine::State* tTestState2 = tSkinAnimator2->AddNewState("test3");
	//
	//	tTestState2->SetAnimDataName("testAnim3");
	//	tTestState2->SetAnimDescName("testAnim3");
	//	tTestState2->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_die_ani.fbx");
	//	tTestState2->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
	//	tTestState2->SetLoop(true);
	//
	//	tSkinAnimator2->SetInitState(tTestState2->GetStateName());
	//
	//	// AnimationController에 Animator 부착.
	//	tAnimController2->SetAnimator(tSkinAnimator2);
	//}





}
