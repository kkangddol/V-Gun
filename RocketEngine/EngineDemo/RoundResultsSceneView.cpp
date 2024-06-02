#include "RoundResultsSceneView.h"
#include "RocketAPILoader.h"

#include <cassert>

RoundResultsSceneView::RoundResultsSceneView()
	: _scene()
{

}

void RoundResultsSceneView::Initialize()
{
	_scene = RocketEngine::CreateScene("Upgradepart");
	RocketEngine::LoadScene("Upgradepart");
	assert(_scene);

	RoundResult();
	//TotalResult();
}

void RoundResultsSceneView::RoundResult()
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
	RocketEngine::GameObject* playerRankingTextObj1 = RocketEngine::CreateTextBox("playerRankingTextObj1");
	RocketEngine::SpriteRenderer* playerRankingSprite1 = playerRankingTextObj1->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRankingText1 = playerRankingTextObj1->GetComponent<RocketEngine::TextBox>();
	playerRankingTextObj1->transform.SetPosition(696.0f, 449.0f, 0.0f);
	playerRankingSprite1->SetSize(0.0f, 0.0f);
	playerRankingText1->SetText("1");
	playerRankingText1->SetFontSize(24);
	playerRankingText1->SetSize(100.0f, 100.0f);
	playerRankingText1->SetSortOrder(10);

	// Player2 순위
	RocketEngine::GameObject* playerRankingTextObj2 = RocketEngine::CreateTextBox("playerRankingTextObj2");
	RocketEngine::SpriteRenderer* playerRankingSprite2 = playerRankingTextObj2->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRankingText2 = playerRankingTextObj2->GetComponent<RocketEngine::TextBox>();
	playerRankingTextObj2->transform.SetPosition(696.0f, 558.0f, 0.0f);
	playerRankingSprite2->SetSize(0.0f, 0.0f);
	playerRankingText2->SetText("2");
	playerRankingText2->SetFontSize(24);
	playerRankingText2->SetSize(100.0f, 100.0f);
	playerRankingText2->SetSortOrder(10);

	// Player3 순위
	RocketEngine::GameObject* playerRankingTextObj3 = RocketEngine::CreateTextBox("playerRankingTextObj3");
	RocketEngine::SpriteRenderer* playerRankingSprite3 = playerRankingTextObj3->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRankingText3 = playerRankingTextObj3->GetComponent<RocketEngine::TextBox>();
	playerRankingTextObj3->transform.SetPosition(696.0f, 667.0f, 0.0f);
	playerRankingSprite3->SetSize(0.0f, 0.0f);
	playerRankingText3->SetText("3");
	playerRankingText3->SetFontSize(24);
	playerRankingText3->SetSize(100.0f, 100.0f);
	playerRankingText3->SetSortOrder(10);

	// Player4 순위
	RocketEngine::GameObject* playerRankingTextObj4 = RocketEngine::CreateTextBox("playerRankingTextObj4");
	RocketEngine::SpriteRenderer* playerRankingSprite4 = playerRankingTextObj4->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRankingText4 = playerRankingTextObj4->GetComponent<RocketEngine::TextBox>();
	playerRankingTextObj4->transform.SetPosition(696.0f, 776.0f, 0.0f);
	playerRankingSprite4->SetSize(0.0f, 0.0f);
	playerRankingText4->SetText("4");
	playerRankingText4->SetFontSize(24);
	playerRankingText4->SetSize(100.0f, 100.0f);
	playerRankingText4->SetSortOrder(10);

	// Player1 프로필
	RocketEngine::GameObject* playerProfileImageObj1 = RocketEngine::CreateImageBox("playerProfileImageObj1");
	RocketEngine::SpriteRenderer* playerProfileSprite1 = playerProfileImageObj1->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj1->transform.SetPosition(827.0f, 449.0f, 0.0f);
	playerProfileSprite1->SetPath("..\\UI\\portrait\\portrait_banana(100x100).png");;
	playerProfileSprite1->SetSize(100.0f, 100.0f);
	playerProfileSprite1->SetSortOrder(10);

	// Player2 프로필
	RocketEngine::GameObject* playerProfileImageObj2 = RocketEngine::CreateImageBox("playerProfileImageObj2");
	RocketEngine::SpriteRenderer* playerProfileSprite2 = playerProfileImageObj2->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj2->transform.SetPosition(827.0f, 558.0f, 0.0f);
	playerProfileSprite2->SetPath("..\\UI\\portrait\\portrait_carrot(100x100).png");;
	playerProfileSprite2->SetSize(100.0f, 100.0f);
	playerProfileSprite2->SetSortOrder(10);

	// Player3 프로필
	RocketEngine::GameObject* playerProfileImageObj3 = RocketEngine::CreateImageBox("playerProfileImageObj3");
	RocketEngine::SpriteRenderer* playerProfileSprite3 = playerProfileImageObj3->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj3->transform.SetPosition(827.0f, 667.0f, 0.0f);
	playerProfileSprite3->SetPath("..\\UI\\portrait\\portrait_kiwi(100x100).png");;
	playerProfileSprite3->SetSize(100.0f, 100.0f);
	playerProfileSprite3->SetSortOrder(10);

	// Player4 프로필
	RocketEngine::GameObject* playerProfileImageObj4 = RocketEngine::CreateImageBox("playerProfileImageObj4");
	RocketEngine::SpriteRenderer* playerProfileSprite4 = playerProfileImageObj4->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj4->transform.SetPosition(827.0f, 776.0f, 0.0f);
	playerProfileSprite4->SetPath("..\\UI\\portrait\\portrait_watermelon(100x100).png");;
	playerProfileSprite4->SetSize(100.0f, 100.0f);
	playerProfileSprite4->SetSortOrder(10);

	// Player1 킬 Text
	RocketEngine::GameObject* playerkillTextObj1 = RocketEngine::CreateTextBox("playerkillTextObj1");
	RocketEngine::SpriteRenderer* playerkillSprite1 = playerkillTextObj1->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerkillText1 = playerkillTextObj1->GetComponent<RocketEngine::TextBox>();
	playerkillTextObj1->transform.SetPosition(1000.0f, 449.0f, 0.0f);
	playerkillSprite1->SetSize(0.0f, 0.0f);
	playerkillText1->SetText("1");
	playerkillText1->SetFontSize(24);
	playerkillText1->SetSize(100.0f, 100.0f);
	playerkillText1->SetSortOrder(10);

	// Player2 킬 Text
	RocketEngine::GameObject* playerkillTextObj2 = RocketEngine::CreateTextBox("playerkillTextObj2");
	RocketEngine::SpriteRenderer* playerkillSprite2 = playerkillTextObj2->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerkillText2 = playerkillTextObj2->GetComponent<RocketEngine::TextBox>();
	playerkillTextObj2->transform.SetPosition(1000.0f, 558.0f, 0.0f);
	playerkillSprite2->SetSize(0.0f, 0.0f);
	playerkillText2->SetText("1");
	playerkillText2->SetFontSize(24);
	playerkillText2->SetSize(100.0f, 100.0f);
	playerkillText2->SetSortOrder(10);

	// Player3 킬 Text
	RocketEngine::GameObject* playerkillTextObj3 = RocketEngine::CreateTextBox("playerkillTextObj3");
	RocketEngine::SpriteRenderer* playerkillSprite3 = playerkillTextObj3->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerkillText3 = playerkillTextObj3->GetComponent<RocketEngine::TextBox>();
	playerkillTextObj3->transform.SetPosition(1000.0f, 667.0f, 0.0f);
	playerkillSprite3->SetSize(0.0f, 0.0f);
	playerkillText3->SetText("1");
	playerkillText3->SetFontSize(24);
	playerkillText3->SetSize(100.0f, 100.0f);
	playerkillText3->SetSortOrder(10);

	// Player4 킬 Text
	RocketEngine::GameObject* playerkillTextObj4 = RocketEngine::CreateTextBox("playerkillTextObj4");
	RocketEngine::SpriteRenderer* playerkillSprite4 = playerkillTextObj4->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerkillText4 = playerkillTextObj4->GetComponent<RocketEngine::TextBox>();
	playerkillTextObj4->transform.SetPosition(1000.0f, 776.0f, 0.0f);
	playerkillSprite4->SetSize(0.0f, 0.0f);
	playerkillText4->SetText("1");
	playerkillText4->SetFontSize(24);
	playerkillText4->SetSize(100.0f, 100.0f);
	playerkillText4->SetSortOrder(10);

	// Player1 데스 Text
	RocketEngine::GameObject* playerDeathTextObj1 = RocketEngine::CreateTextBox("playerDeathTextObj1");
	RocketEngine::SpriteRenderer* playerDeathSprite1 = playerDeathTextObj1->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerDeathText1 = playerDeathTextObj1->GetComponent<RocketEngine::TextBox>();
	playerDeathTextObj1->transform.SetPosition(1137.0f, 449.0f, 0.0f);
	playerDeathSprite1->SetSize(0.0f, 0.0f);
	playerDeathText1->SetText("1");
	playerDeathText1->SetFontSize(24);
	playerDeathText1->SetSize(100.0f, 100.0f);
	playerDeathText1->SetSortOrder(10);

	// Player2 데스 Text
	RocketEngine::GameObject* playerDeathTextObj2 = RocketEngine::CreateTextBox("playerDeathTextObj2");
	RocketEngine::SpriteRenderer* playerDeathSprite2 = playerDeathTextObj2->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerDeathText2 = playerDeathTextObj2->GetComponent<RocketEngine::TextBox>();
	playerDeathTextObj2->transform.SetPosition(1137.0f, 558.0f, 0.0f);
	playerDeathSprite2->SetSize(0.0f, 0.0f);
	playerDeathText2->SetText("1");
	playerDeathText2->SetFontSize(24);
	playerDeathText2->SetSize(100.0f, 100.0f);
	playerDeathText2->SetSortOrder(10);

	// Player3 데스 Text
	RocketEngine::GameObject* playerDeathTextObj3 = RocketEngine::CreateTextBox("playerDeathTextObj3");
	RocketEngine::SpriteRenderer* playerDeathSprite3 = playerDeathTextObj3->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerDeathText3 = playerDeathTextObj3->GetComponent<RocketEngine::TextBox>();
	playerDeathTextObj3->transform.SetPosition(1137.0f, 667.0f, 0.0f);
	playerDeathSprite3->SetSize(0.0f, 0.0f);
	playerDeathText3->SetText("1");
	playerDeathText3->SetFontSize(24);
	playerDeathText3->SetSize(100.0f, 100.0f);
	playerDeathText3->SetSortOrder(10);

	// Player4 데스 Text
	RocketEngine::GameObject* playerDeathTextObj4 = RocketEngine::CreateTextBox("playerDeathTextObj4");
	RocketEngine::SpriteRenderer* playerDeathSprite4 = playerDeathTextObj4->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerDeathText4 = playerDeathTextObj4->GetComponent<RocketEngine::TextBox>();
	playerDeathTextObj4->transform.SetPosition(1137.0f, 776.0f, 0.0f);
	playerDeathSprite4->SetSize(0.0f, 0.0f);
	playerDeathText4->SetText("1");
	playerDeathText4->SetFontSize(24);
	playerDeathText4->SetSize(100.0f, 100.0f);
	playerDeathText4->SetSortOrder(10);

	// Player1 골드 Text
	RocketEngine::GameObject* playerGoldTextObj1 = RocketEngine::CreateTextBox("playerGoldTextObj1");
	RocketEngine::SpriteRenderer* playerGoldSprite1 = playerGoldTextObj1->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerGoldText1 = playerGoldTextObj1->GetComponent<RocketEngine::TextBox>();
	playerGoldTextObj1->transform.SetPosition(1275.0f, 449.0f, 0.0f);
	playerGoldSprite1->SetSize(0.0f, 0.0f);
	playerGoldText1->SetText("1");
	playerGoldText1->SetFontSize(24);
	playerGoldText1->SetSize(100.0f, 100.0f);
	playerGoldText1->SetSortOrder(10);

	// Player2 골드 Text
	RocketEngine::GameObject* playerGoldTextObj2 = RocketEngine::CreateTextBox("playerGoldTextObj2");
	RocketEngine::SpriteRenderer* playerGoldSprite2 = playerGoldTextObj2->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerGoldText2 = playerGoldTextObj2->GetComponent<RocketEngine::TextBox>();
	playerGoldTextObj2->transform.SetPosition(1275.0f, 558.0f, 0.0f);
	playerGoldSprite2->SetSize(0.0f, 0.0f);
	playerGoldText2->SetText("1");
	playerGoldText2->SetFontSize(24);
	playerGoldText2->SetSize(100.0f, 100.0f);
	playerGoldText2->SetSortOrder(10);

	// Player3 골드 Text
	RocketEngine::GameObject* playerGoldTextObj3 = RocketEngine::CreateTextBox("playerGoldTextObj3");
	RocketEngine::SpriteRenderer* playerGoldSprite3 = playerGoldTextObj3->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerGoldText3 = playerGoldTextObj3->GetComponent<RocketEngine::TextBox>();
	playerGoldTextObj3->transform.SetPosition(1275.0f, 667.0f, 0.0f);
	playerGoldSprite3->SetSize(0.0f, 0.0f);
	playerGoldText3->SetText("1");
	playerGoldText3->SetFontSize(24);
	playerGoldText3->SetSize(100.0f, 100.0f);
	playerGoldText3->SetSortOrder(10);

	// Player4 골드 Text
	RocketEngine::GameObject* playerGoldTextObj4 = RocketEngine::CreateTextBox("playerGoldTextObj4");
	RocketEngine::SpriteRenderer* playerGoldSprite4 = playerGoldTextObj4->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerGoldText4 = playerGoldTextObj4->GetComponent<RocketEngine::TextBox>();
	playerGoldTextObj4->transform.SetPosition(1275.0f, 776.0f, 0.0f);
	playerGoldSprite4->SetSize(0.0f, 0.0f);
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

void RoundResultsSceneView::TotalResult()
{
	/// Round Total UI
	// Rouns Results Canvas
	RocketEngine::GameObject* roundTotalCanvas = RocketEngine::CreateImageBox("roundTotalCanvas");
	RocketEngine::SpriteRenderer* roundTotalCanvasSprite = roundTotalCanvas->GetComponent<RocketEngine::SpriteRenderer>();
	roundTotalCanvas->transform.SetPosition(964.0f, 544.0f, 0.0f);
	roundTotalCanvasSprite->SetPath("..\\UI\\Result\\RoundResult_background.png");
	roundTotalCanvasSprite->SetSize(862.0f, 670.0f);
	roundTotalCanvasSprite->SetSortOrder(0);

	// Player1 순위
	RocketEngine::GameObject* playerRankingTextObj1 = RocketEngine::CreateTextBox("playerRankingTextObj1");
	RocketEngine::SpriteRenderer* playerRankingSprite1 = playerRankingTextObj1->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRankingText1 = playerRankingTextObj1->GetComponent<RocketEngine::TextBox>();
	playerRankingTextObj1->transform.SetPosition(696.0f, 449.0f, 0.0f);
	playerRankingSprite1->SetSize(0.0f, 0.0f);
	playerRankingText1->SetText("1");
	playerRankingText1->SetFontSize(24);
	playerRankingText1->SetSize(100.0f, 100.0f);
	playerRankingText1->SetSortOrder(10);

	// Player2 순위
	RocketEngine::GameObject* playerRankingTextObj2 = RocketEngine::CreateTextBox("playerRankingTextObj2");
	RocketEngine::SpriteRenderer* playerRankingSprite2 = playerRankingTextObj2->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRankingText2 = playerRankingTextObj2->GetComponent<RocketEngine::TextBox>();
	playerRankingTextObj2->transform.SetPosition(696.0f, 558.0f, 0.0f);
	playerRankingSprite2->SetSize(0.0f, 0.0f);
	playerRankingText2->SetText("2");
	playerRankingText2->SetFontSize(24);
	playerRankingText2->SetSize(100.0f, 100.0f);
	playerRankingText2->SetSortOrder(10);

	// Player3 순위
	RocketEngine::GameObject* playerRankingTextObj3 = RocketEngine::CreateTextBox("playerRankingTextObj3");
	RocketEngine::SpriteRenderer* playerRankingSprite3 = playerRankingTextObj3->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRankingText3 = playerRankingTextObj3->GetComponent<RocketEngine::TextBox>();
	playerRankingTextObj3->transform.SetPosition(696.0f, 667.0f, 0.0f);
	playerRankingSprite3->SetSize(0.0f, 0.0f);
	playerRankingText3->SetText("3");
	playerRankingText3->SetFontSize(24);
	playerRankingText3->SetSize(100.0f, 100.0f);
	playerRankingText3->SetSortOrder(10);

	// Player4 순위
	RocketEngine::GameObject* playerRankingTextObj4 = RocketEngine::CreateTextBox("playerRankingTextObj4");
	RocketEngine::SpriteRenderer* playerRankingSprite4 = playerRankingTextObj4->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRankingText4 = playerRankingTextObj4->GetComponent<RocketEngine::TextBox>();
	playerRankingTextObj4->transform.SetPosition(696.0f, 776.0f, 0.0f);
	playerRankingSprite4->SetSize(0.0f, 0.0f);
	playerRankingText4->SetText("4");
	playerRankingText4->SetFontSize(24);
	playerRankingText4->SetSize(100.0f, 100.0f);
	playerRankingText4->SetSortOrder(10);

	// Player1 프로필
	RocketEngine::GameObject* playerProfileImageObj1 = RocketEngine::CreateImageBox("playerProfileImageObj1");
	RocketEngine::SpriteRenderer* playerProfileSprite1 = playerProfileImageObj1->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj1->transform.SetPosition(827.0f, 449.0f, 0.0f);
	playerProfileSprite1->SetPath("..\\UI\\portrait\\portrait_banana(100x100).png");;
	playerProfileSprite1->SetSize(100.0f, 100.0f);
	playerProfileSprite1->SetSortOrder(10);

	// Player2 프로필
	RocketEngine::GameObject* playerProfileImageObj2 = RocketEngine::CreateImageBox("playerProfileImageObj2");
	RocketEngine::SpriteRenderer* playerProfileSprite2 = playerProfileImageObj2->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj2->transform.SetPosition(827.0f, 558.0f, 0.0f);
	playerProfileSprite2->SetPath("..\\UI\\portrait\\portrait_carrot(100x100).png");;
	playerProfileSprite2->SetSize(100.0f, 100.0f);
	playerProfileSprite2->SetSortOrder(10);

	// Player3 프로필
	RocketEngine::GameObject* playerProfileImageObj3 = RocketEngine::CreateImageBox("playerProfileImageObj3");
	RocketEngine::SpriteRenderer* playerProfileSprite3 = playerProfileImageObj3->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj3->transform.SetPosition(827.0f, 667.0f, 0.0f);
	playerProfileSprite3->SetPath("..\\UI\\portrait\\portrait_kiwi(100x100).png");;
	playerProfileSprite3->SetSize(100.0f, 100.0f);
	playerProfileSprite3->SetSortOrder(10);

	// Player4 프로필
	RocketEngine::GameObject* playerProfileImageObj4 = RocketEngine::CreateImageBox("playerProfileImageObj4");
	RocketEngine::SpriteRenderer* playerProfileSprite4 = playerProfileImageObj4->GetComponent<RocketEngine::SpriteRenderer>();
	playerProfileImageObj4->transform.SetPosition(827.0f, 776.0f, 0.0f);
	playerProfileSprite4->SetPath("..\\UI\\portrait\\portrait_watermelon(100x100).png");;
	playerProfileSprite4->SetSize(100.0f, 100.0f);
	playerProfileSprite4->SetSortOrder(10);

	// Player1 킬 Text
	RocketEngine::GameObject* playerkillTextObj1 = RocketEngine::CreateTextBox("playerkillTextObj1");
	RocketEngine::SpriteRenderer* playerkillSprite1 = playerkillTextObj1->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerkillText1 = playerkillTextObj1->GetComponent<RocketEngine::TextBox>();
	playerkillTextObj1->transform.SetPosition(1000.0f, 449.0f, 0.0f);
	playerkillSprite1->SetSize(0.0f, 0.0f);
	playerkillText1->SetText("1");
	playerkillText1->SetFontSize(24);
	playerkillText1->SetSize(100.0f, 100.0f);
	playerkillText1->SetSortOrder(10);

	// Player2 킬 Text
	RocketEngine::GameObject* playerkillTextObj2 = RocketEngine::CreateTextBox("playerkillTextObj2");
	RocketEngine::SpriteRenderer* playerkillSprite2 = playerkillTextObj2->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerkillText2 = playerkillTextObj2->GetComponent<RocketEngine::TextBox>();
	playerkillTextObj2->transform.SetPosition(1000.0f, 558.0f, 0.0f);
	playerkillSprite2->SetSize(0.0f, 0.0f);
	playerkillText2->SetText("1");
	playerkillText2->SetFontSize(24);
	playerkillText2->SetSize(100.0f, 100.0f);
	playerkillText2->SetSortOrder(10);

	// Player3 킬 Text
	RocketEngine::GameObject* playerkillTextObj3 = RocketEngine::CreateTextBox("playerkillTextObj3");
	RocketEngine::SpriteRenderer* playerkillSprite3 = playerkillTextObj3->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerkillText3 = playerkillTextObj3->GetComponent<RocketEngine::TextBox>();
	playerkillTextObj3->transform.SetPosition(1000.0f, 667.0f, 0.0f);
	playerkillSprite3->SetSize(0.0f, 0.0f);
	playerkillText3->SetText("1");
	playerkillText3->SetFontSize(24);
	playerkillText3->SetSize(100.0f, 100.0f);
	playerkillText3->SetSortOrder(10);

	// Player4 킬 Text
	RocketEngine::GameObject* playerkillTextObj4 = RocketEngine::CreateTextBox("playerkillTextObj4");
	RocketEngine::SpriteRenderer* playerkillSprite4 = playerkillTextObj4->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerkillText4 = playerkillTextObj4->GetComponent<RocketEngine::TextBox>();
	playerkillTextObj4->transform.SetPosition(1000.0f, 776.0f, 0.0f);
	playerkillSprite4->SetSize(0.0f, 0.0f);
	playerkillText4->SetText("1");
	playerkillText4->SetFontSize(24);
	playerkillText4->SetSize(100.0f, 100.0f);
	playerkillText4->SetSortOrder(10);

	// Player1 데스 Text
	RocketEngine::GameObject* playerDeathTextObj1 = RocketEngine::CreateTextBox("playerDeathTextObj1");
	RocketEngine::SpriteRenderer* playerDeathSprite1 = playerDeathTextObj1->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerDeathText1 = playerDeathTextObj1->GetComponent<RocketEngine::TextBox>();
	playerDeathTextObj1->transform.SetPosition(1137.0f, 449.0f, 0.0f);
	playerDeathSprite1->SetSize(0.0f, 0.0f);
	playerDeathText1->SetText("1");
	playerDeathText1->SetFontSize(24);
	playerDeathText1->SetSize(100.0f, 100.0f);
	playerDeathText1->SetSortOrder(10);

	// Player2 데스 Text
	RocketEngine::GameObject* playerDeathTextObj2 = RocketEngine::CreateTextBox("playerDeathTextObj2");
	RocketEngine::SpriteRenderer* playerDeathSprite2 = playerDeathTextObj2->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerDeathText2 = playerDeathTextObj2->GetComponent<RocketEngine::TextBox>();
	playerDeathTextObj2->transform.SetPosition(1137.0f, 558.0f, 0.0f);
	playerDeathSprite2->SetSize(0.0f, 0.0f);
	playerDeathText2->SetText("1");
	playerDeathText2->SetFontSize(24);
	playerDeathText2->SetSize(100.0f, 100.0f);
	playerDeathText2->SetSortOrder(10);

	// Player3 데스 Text
	RocketEngine::GameObject* playerDeathTextObj3 = RocketEngine::CreateTextBox("playerDeathTextObj3");
	RocketEngine::SpriteRenderer* playerDeathSprite3 = playerDeathTextObj3->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerDeathText3 = playerDeathTextObj3->GetComponent<RocketEngine::TextBox>();
	playerDeathTextObj3->transform.SetPosition(1137.0f, 667.0f, 0.0f);
	playerDeathSprite3->SetSize(0.0f, 0.0f);
	playerDeathText3->SetText("1");
	playerDeathText3->SetFontSize(24);
	playerDeathText3->SetSize(100.0f, 100.0f);
	playerDeathText3->SetSortOrder(10);

	// Player4 데스 Text
	RocketEngine::GameObject* playerDeathTextObj4 = RocketEngine::CreateTextBox("playerDeathTextObj4");
	RocketEngine::SpriteRenderer* playerDeathSprite4 = playerDeathTextObj4->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerDeathText4 = playerDeathTextObj4->GetComponent<RocketEngine::TextBox>();
	playerDeathTextObj4->transform.SetPosition(1137.0f, 776.0f, 0.0f);
	playerDeathSprite4->SetSize(0.0f, 0.0f);
	playerDeathText4->SetText("1");
	playerDeathText4->SetFontSize(24);
	playerDeathText4->SetSize(100.0f, 100.0f);
	playerDeathText4->SetSortOrder(10);

	// Player1 라운드 Text
	RocketEngine::GameObject* playerRoundTextObj1 = RocketEngine::CreateTextBox("playerRoundTextObj1");
	RocketEngine::SpriteRenderer* playerRoundSprite1 = playerRoundTextObj1->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRoundText1 = playerRoundTextObj1->GetComponent<RocketEngine::TextBox>();
	playerRoundTextObj1->transform.SetPosition(1275.0f, 449.0f, 0.0f);
	playerRoundSprite1->SetSize(0.0f, 0.0f);
	playerRoundText1->SetText("1");
	playerRoundText1->SetFontSize(20);
	playerRoundText1->SetSize(100.0f, 100.0f);
	playerRoundText1->SetSortOrder(10);

	// Player2 라운드 Text
	RocketEngine::GameObject* playerRoundTextObj2 = RocketEngine::CreateTextBox("playerRoundTextObj2");
	RocketEngine::SpriteRenderer* playerRoundSprite2 = playerRoundTextObj2->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRoundText2 = playerRoundTextObj2->GetComponent<RocketEngine::TextBox>();
	playerRoundTextObj2->transform.SetPosition(1275.0f, 558.0f, 0.0f);
	playerRoundSprite2->SetSize(0.0f, 0.0f);
	playerRoundText2->SetText("1");
	playerRoundText2->SetFontSize(24);
	playerRoundText2->SetSize(100.0f, 100.0f);
	playerRoundText2->SetSortOrder(10);

	// Player3 라운드 Text
	RocketEngine::GameObject* playerRoundTextObj3 = RocketEngine::CreateTextBox("playerRoundTextObj3");
	RocketEngine::SpriteRenderer* playerRoundSprite3 = playerRoundTextObj3->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRoundText3 = playerRoundTextObj3->GetComponent<RocketEngine::TextBox>();
	playerRoundTextObj3->transform.SetPosition(1275.0f, 667.0f, 0.0f);
	playerRoundSprite3->SetSize(0.0f, 0.0f);
	playerRoundText3->SetText("1");
	playerRoundText3->SetFontSize(24);
	playerRoundText3->SetSize(100.0f, 100.0f);
	playerRoundText3->SetSortOrder(10);

	// Player4 라운드 Text
	RocketEngine::GameObject* playerRoundTextObj4 = RocketEngine::CreateTextBox("playerRoundTextObj4");
	RocketEngine::SpriteRenderer* playerRoundSprite4 = playerRoundTextObj4->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* playerRoundText4 = playerRoundTextObj4->GetComponent<RocketEngine::TextBox>();
	playerRoundTextObj4->transform.SetPosition(1275.0f, 776.0f, 0.0f);
	playerRoundSprite4->SetSize(0.0f, 0.0f);
	playerRoundText4->SetText("1");
	playerRoundText4->SetFontSize(24);
	playerRoundText4->SetSize(100.0f, 100.0f);
	playerRoundText4->SetSortOrder(10);

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
	playerRoundTextObj1->transform.SetParent(roundTotalCanvas);
	playerRoundTextObj2->transform.SetParent(roundTotalCanvas);
	playerRoundTextObj3->transform.SetParent(roundTotalCanvas);
	playerRoundTextObj4->transform.SetParent(roundTotalCanvas);

	/// Object 상태
	roundTotalCanvas->Enable();
}


