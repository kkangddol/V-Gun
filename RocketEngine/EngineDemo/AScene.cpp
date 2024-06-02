#include "AScene.h"
#include "RocketAPILoader.h"

#include "SceneLoadTest.h"
#include "DebugCameraMove.h"

AScene::AScene()
	: scene()
{

}

void AScene::Initialize()
{
	scene = RocketEngine::CreateScene("AScene");
	RocketEngine::GetMainCamera()->gameObject->AddComponent<DebugCameraMove>();

// 	// A Object
// 	RocketEngine::GameObject* player = RocketEngine::CreateObject("player");
// 
// 	RocketEngine::MeshRenderer* playerRenderer = player->AddComponent<RocketEngine::MeshRenderer>();
// 	playerRenderer->SetMeshPath("../3DModels/Player/4QCharacter_tpose4.fbx");
// 	//playerRenderer->SetShaderPath("../FX/Default/BasicTex.cso");
// 	playerRenderer->SetShaderPath("../FX/Default/PosNormalTex.cso");
// 	playerRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture2/banana/TT_checker_2048x2048_COLOR_GRID_BaseColor.png");
// 	playerRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture2/banana/TT_checker_2048x2048_COLOR_GRID_Normal.png");
// 	playerRenderer->SetRenderType(RocketCore::Graphics::eRenderType::FORWARD);
// 
// 	playerRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
// 	playerRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
// 	playerRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
// 	playerRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });

	//player->AddComponent<SceneLoadTest>();

	///[TW] 유나님이 보내주신 캐릭터 돌아가는지 테스트해봄 여기에!
	RocketEngine::GameObject* testMainChar = RocketEngine::CreateObject("PleaseWorkPlease");
	//testMainChar->transform.SetLocalPosition(0.1f, -0.4f, 0.4f);
	//testMainChar->transform.SetLocalRotationEuler(80.0f, -90.0f, 5.0f);
	testMainChar->AddComponent<SceneLoadTest>();

	RocketEngine::SkinnedMeshRenderer* tGunRenderer = testMainChar->AddComponent<RocketEngine::SkinnedMeshRenderer>();
	tGunRenderer->SetMeshPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
	tGunRenderer->SetShaderPath("../FX/Default/SkinnedMeshTex.cso");
	tGunRenderer->SetDiffusePath("../3DModels/Player/Vgun_character_texture3/banana/BaseColor.png");
	tGunRenderer->SetNormalPath("../3DModels/Player/Vgun_character_texture3/banana/Normal.png");
	tGunRenderer->SetLegacyAmbient({ 1.0f,1.0f, 1.0f, 1.0f });
	tGunRenderer->SetLegacyDiffuse({ 1.0f,0.1f, 0.1f, 1.0f });
	tGunRenderer->SetLegacySpecular({ 1.0f,1.0f, 1.0f, 1.0f });
	tGunRenderer->SetLegacyReflect({ 1.0f,1.0f, 1.0f, 1.0f });

	RocketEngine::AnimationController* tAnimController = testMainChar->AddComponent<RocketEngine::AnimationController>();

	// Animator 및 State 생성.
	RocketEngine::Animator* tSkinAnimator = new RocketEngine::Animator();
	RocketEngine::State* tTestState = tSkinAnimator->AddNewState("test");

	tTestState->SetAnimDataName("testAnim");
	tTestState->SetAnimDescName("testAnim");
	tTestState->SetAnimDescAnimPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_sidestep_ani.fbx");
	tTestState->SetAnimDescOwnerPath("../3DModels/Player/Vgun_Character_ani/4QCharacter_Tpose_ani.fbx");
	tTestState->SetLoop(true);

	tSkinAnimator->SetInitState(tTestState->GetStateName());

	// AnimationController에 Animator 부착.
	tAnimController->SetAnimator(tSkinAnimator);
}
