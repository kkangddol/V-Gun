#include "UpgradePartsSceneView.h"
#include "RocketAPILoader.h"

#include <cassert>

UpgradePartsSceneView::UpgradePartsSceneView()
	: scene()
{

}

void UpgradePartsSceneView::Initialize()
{
	scene = RocketEngine::CreateScene("Upgradepart");
	RocketEngine::LoadScene("Upgradepart");
	assert(scene);

	// Sound 추가
	AddSound();
	
	// UI 관리
	UpgrdePartsScene();
	CardScene();
}

void UpgradePartsSceneView::UpgrdePartsScene()
{
	/// Upgrade UI
	RocketEngine::GameObject* upgradeUICanvas = RocketEngine::CreateObject("upgradeCanvas");

	/// 파츠강화
	// 시간 제한 Image
	RocketEngine::GameObject* timeLimitImageObj = RocketEngine::CreateImageBox("timeLimitImageObj");
	RocketEngine::SpriteRenderer* timeLimitSprite = timeLimitImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	timeLimitImageObj->transform.SetLocalPosition(960.0f, 54.0f, 0.0f);
	timeLimitSprite->SetPath("..\\UI\\Card\\ClockBackground.png");
	timeLimitSprite->SetSize(396.0f, 166.0f);
	timeLimitSprite->SetSortOrder(10);

	//// 골드 아이콘 Image
	//RocketEngine::GameObject* coinIconImageObj = RocketEngine::CreateImageBox("coinIconImageObj");
	//RocketEngine::SpriteRenderer* coinIconSprite = coinIconImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	//coinIconImageObj->transform.SetLocalPosition(861.0f, 171.0f, 0.0f);
	//coinIconSprite->SetPath("..\\UI\\Card\\Coin.png");
	//coinIconSprite->SetSize(100.0f, 100.0f);
	//coinIconSprite->SetSortOrder(10);

	// 골드량 Image
	RocketEngine::GameObject* coinNumImageObj = RocketEngine::CreateImageBox("goldValImageObj");
	RocketEngine::SpriteRenderer* coinNumSprite = coinNumImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	coinNumImageObj->transform.SetLocalPosition(1040.0f, 171.0f, 0.0f);
	coinNumSprite->SetPath("..\\UI\\Card\\CoinQuantity.png");
	coinNumSprite->SetSize(446.0f, 110.0f);
	coinNumSprite->SetSortOrder(10);

	// Canvas Setting
	timeLimitImageObj->transform.SetParent(upgradeUICanvas);
	//coinIconImageObj->transform.SetParent(upgradeUICanvas);
	coinNumImageObj->transform.SetParent(upgradeUICanvas);

	// Object 기본 상태 설정
	//upgradeUICanvas->Enable();
}

void UpgradePartsSceneView::CardScene()
{
	/// Card UI
	// Card UI Canvas
	RocketEngine::GameObject* cardCanvas = RocketEngine::CreateStaticObject("cardCanvas");

	// 업그레이드 Box1
	RocketEngine::GameObject* cardBoxImageObj1 = RocketEngine::CreateImageBox("cardBoxImageObj1");
	RocketEngine::SpriteRenderer* cardBoxSprite1 = cardBoxImageObj1->GetComponent<RocketEngine::SpriteRenderer>();
	cardBoxImageObj1->transform.SetLocalPosition(537.0f, 588.0f, 0.0f);
	cardBoxSprite1->SetPath("..\\UI\\Card\\CardBackGround01.png");
	cardBoxSprite1->SetSize(400.0f, 700.0f);
	cardBoxSprite1->SetSortOrder(30);

	// 파츠 아이콘 Image1
	RocketEngine::GameObject* partsIconImageObj1 = RocketEngine::CreateImageBox("partsIconImageObj1");
	RocketEngine::SpriteRenderer* partsIconSprite1 = partsIconImageObj1->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconImageObj1->transform.SetLocalPosition(537.0f, 418.0f, 0.0f);
	partsIconSprite1->SetPath("..\\UI\\Card\\PartsIcon01.png");
	partsIconSprite1->SetSize(300.0f, 300.0f);
	partsIconSprite1->SetSortOrder(40);

	// 구매 Button1
	RocketEngine::GameObject* buyButtonObj1 = RocketEngine::CreateButton("buyButtonObj1");
	RocketEngine::Button* buyButton1 = buyButtonObj1->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* buyButtonSprite1 = buyButtonObj1->GetComponent<RocketEngine::SpriteRenderer>();
	buyButtonObj1->transform.SetLocalPosition(537.0f, 855.0f, 0.0f);
	buyButton1->SetSize(180.0f, 100.0f);
	buyButton1->SetSortOrder(60);
	buyButtonSprite1->SetPath("..\\UI\\Card\\Button.png");
	buyButtonSprite1->SetSize(180.0f, 100.0f);
	buyButtonSprite1->SetSortOrder(50);

	// 업그레이드 Box2
	RocketEngine::GameObject* cardBoxImageObj2 = RocketEngine::CreateImageBox("cardBoxImageObj2");
	RocketEngine::SpriteRenderer* cardBoxSprite2 = cardBoxImageObj2->GetComponent<RocketEngine::SpriteRenderer>();
	cardBoxImageObj2->transform.SetLocalPosition(959.0f, 588.0f, 0.0f);
	cardBoxSprite2->SetPath("..\\UI\\Card\\CardBackGround02.png");
	cardBoxSprite2->SetSize(400.0f, 700.0f);
	cardBoxSprite2->SetSortOrder(30);

	// 파츠 아이콘 Image2
	RocketEngine::GameObject* partsIconImageObj2 = RocketEngine::CreateImageBox("partsIconImageObj2");
	RocketEngine::SpriteRenderer* partsIconSprite2 = partsIconImageObj2->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconImageObj2->transform.SetLocalPosition(951.0f, 418.0f, 0.0f);
	partsIconSprite2->SetPath("..\\UI\\Card\\PartsIcon02.png");
	partsIconSprite2->SetSize(300.0f, 300.0f);
	partsIconSprite2->SetSortOrder(40);

	// 구매 Button2
	RocketEngine::GameObject* buyButtonObj2 = RocketEngine::CreateButton("buyButtonObj2");
	RocketEngine::Button* buyButton2 = buyButtonObj2->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* buyButtonSprite2 = buyButtonObj2->GetComponent<RocketEngine::SpriteRenderer>();
	buyButtonObj2->transform.SetLocalPosition(960.0f, 855.0f, 0.0f);
	buyButton2->SetSize(180.0f, 100.0f);
	buyButton2->SetSortOrder(60);
	buyButtonSprite2->SetPath("..\\UI\\Card\\Button.png");
	buyButtonSprite2->SetSize(180.0f, 100.0f);
	buyButtonSprite2->SetSortOrder(50);

	// 업그레이드 Box3
	RocketEngine::GameObject* cardBoxImageObj3 = RocketEngine::CreateImageBox("cardBoxImageObj3");
	RocketEngine::SpriteRenderer* cardBoxSprite3 = cardBoxImageObj3->GetComponent<RocketEngine::SpriteRenderer>();
	cardBoxImageObj3->transform.SetLocalPosition(1381.0f, 588.0f, 0.0f);
	cardBoxSprite3->SetPath("..\\UI\\Card\\CardBackGround03.png");
	cardBoxSprite3->SetSize(400.0f, 700.0f);
	cardBoxSprite3->SetSortOrder(30);

	// 파츠 아이콘 Image3
	RocketEngine::GameObject* partsIconImageObj3 = RocketEngine::CreateImageBox("partsIconImageObj3");
	RocketEngine::SpriteRenderer* partsIconSprite3 = partsIconImageObj3->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconImageObj3->transform.SetLocalPosition(1382.0f, 418.0f, 0.0f);
	partsIconSprite3->SetPath("..\\UI\\Card\\PartsIcon03.png");
	partsIconSprite3->SetSize(300.0f, 300.0f);
	partsIconSprite3->SetSortOrder(40);

	// 구매 Button3
	RocketEngine::GameObject* buyButtonObj3 = RocketEngine::CreateButton("buyButtonObj3");
	RocketEngine::Button* buyButton3 = buyButtonObj3->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* buyButtonSprite3 = buyButtonObj3->GetComponent<RocketEngine::SpriteRenderer>();
	buyButtonObj3->transform.SetLocalPosition(1382.0f, 855.0f, 0.0f);
	buyButton3->SetSize(180.0f, 100.0f);
	buyButton3->SetSortOrder(60);
	buyButtonSprite3->SetPath("..\\UI\\Card\\Button.png");
	buyButtonSprite3->SetSize(180.0f, 100.0f);
	buyButtonSprite3->SetSortOrder(50);

	// Canvas Setting
	partsIconImageObj1->transform.SetParent(cardBoxImageObj1);

	buyButtonObj1->transform.SetParent(cardBoxImageObj1);
	buyButton1->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
		}
	);

	partsIconImageObj2->transform.SetParent(cardBoxImageObj2);

	buyButtonObj2->transform.SetParent(cardBoxImageObj2);
	buyButton2->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
		}
	);

	partsIconImageObj3->transform.SetParent(cardBoxImageObj3);

	buyButtonObj3->transform.SetParent(cardBoxImageObj3);
	buyButton3->SetOnClickEvent(
		[]()
		{
			RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);
		}
	);

	cardBoxImageObj1->transform.SetParent(cardCanvas);
	cardBoxImageObj2->transform.SetParent(cardCanvas);
	cardBoxImageObj3->transform.SetParent(cardCanvas);

	// Object 기본 상태 설정
	//cardCanvas->Enable();
}

void UpgradePartsSceneView::AddSound()
{
	// 버튼 클릭
	RocketEngine::AddAudio("..\\Sound\\ButtonClick\\button_push.wav", eSoundGroup::EffectSound);
}
