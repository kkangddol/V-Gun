#include "RoundEndSceneView.h"
#include "RocketAPILoader.h"

#include <cassert>

RoundEndSceneView::RoundEndSceneView()
	: scene()
{

}

void RoundEndSceneView::Initialize()
{
	scene = RocketEngine::CreateScene("RoundEndScene");
	RocketEngine::LoadScene("RoundEndScene");
	assert(scene);

	// Sound 추가

	// UI 관리
	RoundEnd();
}

void RoundEndSceneView::RoundEnd()
{
	/// Object, Renderer 객체 생성
	// 라운드 종료 창
	RocketEngine::GameObject* roundEndImageObj = RocketEngine::CreateImageBox("roundEndImageObj");
	RocketEngine::SpriteRenderer* roundEndSprite = roundEndImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	roundEndImageObj->transform.SetPosition(960.0f, 540.0f, 0.0f);
	roundEndSprite->SetPath("..\\UI\\RoundOver\\RoundOver_only_1920x433.png");
	roundEndSprite->SetSize(1920.0f, 1080.0f);
	roundEndSprite->SetSortOrder(0);

	//// 1안 적용 시? 필요없으면 삭제
	//RocketEngine::GameObject* roundEndTextObj = RocketEngine::CreateTextBox("roundEndTextObj");
	//RocketEngine::TextBox* roundEndText = roundEndTextObj->GetComponent<RocketEngine::TextBox>();
	//roundEndTextObj->transform.SetPosition(0.0f, 0.0f, 0.0f); // 임시 좌표
	//roundEndText->SetText("1등 100골드");
	//roundEndText->SetSize(100.0f, 100.0f);	// 임시
	//roundEndText->SetSortOrder(20);

	// 기본 상태 설정
	//roundEndImageObj->Enable();

	/// Button 설정
}

void RoundEndSceneView::AddSound()
{

}
