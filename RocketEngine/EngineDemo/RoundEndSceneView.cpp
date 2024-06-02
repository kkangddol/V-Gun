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

	// Sound �߰�

	// UI ����
	RoundEnd();
}

void RoundEndSceneView::RoundEnd()
{
	/// Object, Renderer ��ü ����
	// ���� ���� â
	RocketEngine::GameObject* roundEndImageObj = RocketEngine::CreateImageBox("roundEndImageObj");
	RocketEngine::SpriteRenderer* roundEndSprite = roundEndImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	roundEndImageObj->transform.SetPosition(960.0f, 540.0f, 0.0f);
	roundEndSprite->SetPath("..\\UI\\RoundOver\\RoundOver_only_1920x433.png");
	roundEndSprite->SetSize(1920.0f, 1080.0f);
	roundEndSprite->SetSortOrder(0);

	//// 1�� ���� ��? �ʿ������ ����
	//RocketEngine::GameObject* roundEndTextObj = RocketEngine::CreateTextBox("roundEndTextObj");
	//RocketEngine::TextBox* roundEndText = roundEndTextObj->GetComponent<RocketEngine::TextBox>();
	//roundEndTextObj->transform.SetPosition(0.0f, 0.0f, 0.0f); // �ӽ� ��ǥ
	//roundEndText->SetText("1�� 100���");
	//roundEndText->SetSize(100.0f, 100.0f);	// �ӽ�
	//roundEndText->SetSortOrder(20);

	// �⺻ ���� ����
	//roundEndImageObj->Enable();

	/// Button ����
}

void RoundEndSceneView::AddSound()
{

}
