#include "pch.h"
#include "GameManager.h"
#include "RocketAPILoader.h"
#include "PlayerInfo.h"
#include "PlayerMove.h"
#include "NetworkManager.h"
#include "RandomSpawn.h"
#include "LobbyManager.h"

static RocketEngine::GameObject* GO = nullptr;

GameManager& GameManager::Instance()
{
	static GameManager instance(GO);
	return instance;
}

GameManager::GameManager(RocketEngine::GameObject* owner)
	: RocketEngine::Component(owner)
{
	RocketEngine::AddStaticComponent(this);

	auto tr1 = new RocketEngine::Transform(nullptr);
	tr1->SetLocalPosition({ 16.0f,0.0f,-16.99f });
	tr1->Rotate(0.0f, -45.0f, 0.0f);

	auto tr2 = new RocketEngine::Transform(nullptr);
	tr2->SetLocalPosition({ -17.6f,0.0f,-16.99f });
	tr2->Rotate(0.0f, 45.0f, 0.0f);

	auto tr3 = new RocketEngine::Transform(nullptr);
	tr3->SetLocalPosition({ -17.6f,0.0f,16.99f });
	tr3->Rotate(0.0f, 135.0f, 0.0f);

	auto tr4 = new RocketEngine::Transform(nullptr);
	tr4->SetLocalPosition({ 13.97f,0.0f,16.99f });
	tr4->Rotate(0.0f, -135.0f, 0.0f);

	auto tr5 = new RocketEngine::Transform(nullptr);
	tr5->SetLocalPosition({ -1.29f,3.62f,-17.63f });
	tr5->Rotate(0.0f, 10.0f, 0.0f);

	auto tr6 = new RocketEngine::Transform(nullptr);
	tr6->SetLocalPosition({ -5.39f,2.97f,-0.36f });
	tr6->Rotate(0.0f, 90.0f, 0.0f);

	auto tr7 = new RocketEngine::Transform(nullptr);
	tr7->SetLocalPosition({ 0.38f,3.8f,2.71f });
	tr7->Rotate(0.0f, -90.0f, 0.0f);

	auto tr8 = new RocketEngine::Transform(nullptr);
	tr8->SetLocalPosition({ -5.07f,0.0f,2.55f });
	tr8->Rotate(0.0f, 135.0f, 0.0f);

	_randomSpawnPos.push_back(tr1);
	_randomSpawnPos.push_back(tr2);
	_randomSpawnPos.push_back(tr3);
	_randomSpawnPos.push_back(tr4);
	_randomSpawnPos.push_back(tr5);
	_randomSpawnPos.push_back(tr6);
	_randomSpawnPos.push_back(tr7);
	_randomSpawnPos.push_back(tr8);

	_isPartsClicked[0] = false;
	_isPartsClicked[1] = false;
	_isPartsClicked[2] = false;
}

void GameManager::Start()
{
	if (_settingCanvas != nullptr)	_settingCanvas->Disable();

	GameManager::Instance().InitGameManager();
}

void GameManager::Update()
{
	if (state != Protocol::ROUND_STATE_PLAYING &&
		state != Protocol::ROUND_STATE_LODDINGWAIT)
		return;

	if (remainTime > 0.0f)
	{
		remainTime -= RocketEngine::GetDeltaTime();
	}
	else
	{
		remainTime = 0.0f;
	}
	SetRoundTimeText(remainTime);

	SetAmmoText(to_string(_playersIndex[myIndex]->currentAmmoCount));

	// 죽은자는 말이 없다
	if (GameManager::GetPlayerInfo(myId)->isDie)
	{
		_reviveTimeTextBox->SetText(std::to_string((int)_reviveTime));
		_reviveTime -= RocketEngine::GetDeltaTime();
		return;
	}

	static float deltaTime = 0;

	deltaTime += RocketEngine::GetDeltaTime();

	// 하드코딩
	if (deltaTime < 1 / 144.f)
		return;
	deltaTime = 0;

	auto positon = myPlayer->transform.GetPosition();
	auto rotation = myPlayer->transform.GetRotation();
	if (
		std::abs(this->lastSendPosition.x - positon.x) < 0.05f &&
		std::abs(this->lastSendPosition.y - positon.y) < 0.05f &&
		std::abs(this->lastSendPosition.z - positon.z) < 0.05f &&

		std::abs(this->lastSendRotation.w - rotation.w) < 0.05f &&
		std::abs(this->lastSendRotation.x - rotation.x) < 0.05f &&
		std::abs(this->lastSendRotation.y - rotation.y) < 0.05f &&
		std::abs(this->lastSendRotation.z - rotation.z) < 0.05f
		)
		return;

	this->lastSendPosition = positon;
	this->lastSendRotation = rotation;

	// Todo 회전값이 크지 않다면 스킵하기

	NetworkManager::Instance().SendMyTransfrom(this->lastSendPosition, this->lastSendRotation);

	// 라운드 시간 감소
	//if (!(roundTime < 0))
	//{
	//	roundTime -= deltaTime;
	//}
	//else
	//{
	//	roundTime = 0;
	//}
}

void GameManager::InitGameManager()
{
	myIndex = 0;

	this->InitRound();

	for (auto player : _playersIndex)
	{
		player->isHost = false;
		player->playerIndex = 0;
		player->Init();
	}

	//_playersMap.clear();
}

void GameManager::InitRound()
{
	state = Protocol::ROUND_STATE_NONE;

	currentKill = 0;
	totalKill = 0;
	roundTime = 0.0f;
	remainTime = 0.0f;

	selectPartsList.clear();

	::memset(&lastSendPosition, 0, sizeof(lastSendPosition));

	::memset(&lastSendRotation, 0, sizeof(lastSendRotation));

	for (auto player : _playersIndex)
	{
		player->InitRound();
	}

	_reviveTime = 0;
}

PlayerInfo* GameManager::GetPlayerInfo(uint32 index)
{
	return _playersIndex[index];
}

PlayerInfo* GameManager::GetPlayerInfo(uint64 id)
{
	return _playersMap[id];
}

PlayerInfo* GameManager::GetPlayerInfo(const Protocol::Player& player)
{
	return GetPlayerInfo(player.index());
}

PlayerInfo* GameManager::GetPlayerInfo(RocketEngine::GameObject* gameObject)
{
	return gameObject->GetComponent<PlayerInfo>();
}

PlayerInfo* GameManager::GetPlayerInfo()
{
	return GetPlayerInfo(this->myId);
}

void GameManager::EnterPlayer(const Protocol::Player& player)
{
	auto playerInfo = GetPlayerInfo(player.index());

	playerInfo->playerId = player.id();
	playerInfo->playerIndex = player.index();

	this->_playersMap[player.id()] = _playersIndex[playerInfo->playerIndex];
}

void GameManager::LeavePlayer(const Protocol::Player& player)
{
	if (player.index() == 0)
		return;

	auto playerInfo = GetPlayerInfo(player.index());

	if (playerInfo == nullptr)
		return;

	// this->_playersMap.erase(player.id());

	playerInfo->Init();
	playerInfo->playerId = 0;
	playerInfo->playerIndex = 0;
}

void GameManager::SpawnPlayer(RocketEngine::GameObject* player, int posIndex)
{
	_isPartsClicked[0] = false;
	_isPartsClicked[1] = false;
	_isPartsClicked[2] = false;

	while (ShowCursor(false) >= 0) {}
	if (_randomSpawnPos.size() <= posIndex)
		posIndex = -1;

	player->transform.SetPosition(_randomSpawnPos[posIndex]->GetLocalPosition());
	player->transform.SetRotation(_randomSpawnPos[posIndex]->GetLocalRotation());

	player->GetComponent<PlayerMove>()->remotePosition = _randomSpawnPos[posIndex]->GetLocalPosition();
	player->GetComponent<PlayerMove>()->remoteQuaternion = _randomSpawnPos[posIndex]->GetLocalRotation();

	_inGameCanvas->Enable();
	_deadCanvas->Disable();
	for (int i = 0; i < 3; ++i)
	{
		_partsSprite[i]->gameObject->transform.GetParent()->SetPosition(_partsOriginalPos[i]);
	}
}

void GameManager::SetRoundInfo(Protocol::RoundState state, uint32 time)
{
	this->state = state;
	this->remainTime = (float)time;	// ms 단위로 변경
}

void GameManager::SetSettingCanvas(RocketEngine::GameObject* canvasObj)
{
	_settingCanvas = canvasObj;
}

void GameManager::Setting()
{
	while (ShowCursor(true) < 0) {}
	_settingCanvas->Enable();
}

void GameManager::ExitSetting()
{
	while (ShowCursor(false) >= 0) {}
	_settingCanvas->Disable();
}

void GameManager::AddPlayerInfo(PlayerInfo* playerInfo)
{
	_playersIndex.push_back(playerInfo);
}

void GameManager::SetMyPlayer(int index)
{
	myIndex = index;
	_playersIndex[index] = _playersIndex[0];
	myPlayer = _playersIndex[0]->gameObject;
	myPlayer->GetComponent<RocketEngine::AnimationController>()->GetAnimator()->onStateChanged = []() { GameManager::Instance().OnStateChanged(); };
}

void GameManager::OnStartGame(int index)
{
	// TODO : 마우스 없애야함
	_playersIndex[index]->gameObject->Enable();

	if (myId == _playersIndex[index]->playerId)
	{
		switch (index)
		{
			case 1:
				_playerSprite->SetPath("..\\UI\\InGame\\Character_Watermelon_494x178.png");
				break;
			case 2:
				_playerSprite->SetPath("..\\UI\\InGame\\Character_Kiw_494x178i.png");
				break;
			case 3:
				_playerSprite->SetPath("..\\UI\\InGame\\Character_Carrot_494x178.png");
				break;
			case 4:
				_playerSprite->SetPath("..\\UI\\InGame\\Character_Banana_494x178.png");
				break;
			default:
				break;
		}
	}
}

std::string GameManager::OnStateChanged()
{
	return myPlayer->GetComponent<RocketEngine::AnimationController>()->GetCurrentState()->GetStateName();
}


/// InGameUI
#pragma region UI관련

void GameManager::SetPlayerKillCountText(const int killNum, int playerIndex)
{
	auto killCount = std::to_string(killNum);

	switch (playerIndex)
	{
		case 1:
		{
			_player1KillText->SetText(killCount);
		}
		break;
		case 2:
		{
			_player2KillText->SetText(killCount);
		}
		break;
		case 3:
		{
			_player3KillText->SetText(killCount);

		}
		break;
		case 4:
		{
			_player4KillText->SetText(killCount);
		}
		break;
		default:
			break;
	}
}

void GameManager::SetRoundTimeText(const float timeNow)
{
	int min = (int)timeNow / 60;
	int sec = (int)timeNow % 60;
	std::string time;
	if (sec < 10)
	{
		time = std::to_string(min) + " : 0" + std::to_string(sec);
	}
	else
	{
		time = std::to_string(min) + " : " + std::to_string(sec);
	}
	_roundTimeText->SetText(time);
}

void GameManager::SetPlayerSpritePath(const std::string& path)
{
	_playerSprite->SetPath(path);
}

void GameManager::SetPlayerHpBar(float hp)
{
	float maxHp = _playersIndex[myIndex]->stats.healthPoint;
	float ratio = hp / maxHp * 100.0f;
	_hpBarSprite->SetFillRatio(ratio);
}

void GameManager::SetAmmoText(const std::string& text)
{
	_ammoObj->SetText(text);
}

void GameManager::SetGunSpritePath(const std::string& path)
{
	_gunSprite->SetPath(path);
}

void GameManager::ChangeGunModeling(int index)
{
	for (int i = 0; i < 4; ++i)
	{
		_gunModeling[i]->Disable();
	}

	_gunModeling[index]->Enable();
}

void GameManager::UpdateGoldText(std::string text)
{
	_goldText->SetText(text);
}

void GameManager::UpdatePartsInfo()
{
	_reviveTime = 10.0f;
	int index = 0;
	for (const auto& parts : selectPartsList)
	{
		switch (parts.first)
		{
			case Protocol::PARTS_VGUN_CARROT :
			{
				// 당근
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\carrot.png");

				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("당근 1성\n\n빠른 장전속도, 준수한 연사속도, 낮은 데미지\n\n 등급에 따른 데미지 증가\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("당근 2성\n\n빠른 장전속도, 준수한 연사속도, 낮은 데미지\n\n 등급에 따른 데미지 증가\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("당근 3성\n\n빠른 장전속도, 준수한 연사속도, 낮은 데미지\n\n 등급에 따른 데미지 증가\n\n가격: 100");
					}
					break;
					default:
						break;
				}
			}
			break;
			case Protocol::PARTS_VGUN_WATERMELON :
			{
				// 수박
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\watermelon.png");
				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("수박 1성\n\n높은 데미지, 느린 장전속도, 연사속도, 이동속도\n\n 등급에 따른 데미지 증가\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("수박 2성\n\n높은 데미지, 느린 장전속도, 연사속도, 이동속도\n\n 등급에 따른 데미지 증가\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("수박 3성\n\n높은 데미지, 느린 장전속도, 연사속도, 이동속도\n\n 등급에 따른 데미지 증가\n\n가격: 100");
					}
					break;
					default:
						break;
				}
			}
			break;
			case Protocol::PARTS_VGUN_KIWI :
			{
				// 키위 1성
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\kiwi.png");
				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("키위 1성\n\n빠른 이동속도, 준수한 데미지, 연사속도\n\n 등급에 따른 데미지 증가\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("키위 2성\n\n빠른 이동속도, 준수한 데미지, 연사속도\n\n 등급에 따른 데미지 증가\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("키위 3성\n\n빠른 이동속도, 준수한 데미지, 연사속도\n\n 등급에 따른 데미지 증가\n\n가격: 100");
					}
					break;
					default:
						break;
				}
			}
			break;
			case Protocol::PARTS_VGUN_BANANA :
			{
				// 바나나
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\Banana.png");
				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("바나나 1성\n\n빠른 장전속도, 연사속도, 낮은 데미지\n\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("바나나 2성\n\n빠른 장전속도, 연사속도, 낮은 데미지\n\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("바나나 3성\n\n빠른 장전속도, 연사속도, 낮은 데미지\n\n\n가격: 100");
					}
					break;
					default:
						break;
				}
			}
			break;
			case Protocol::PARTS_HEALTHPOINT :
			{
				// 체력
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\Heal.png");
				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("체력 1성\n\n체력 + 20 상승\n\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("체력 2성\n\n체력 + 30 상승\n\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("체력 3성\n\n체력 + 40 상승\n\n\n가격: 100");
					}
					break;
					default:
						break;
				}
			}
			break;
			case Protocol::PARTS_MOVESPEED :
			{
				// 이동 속도
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\speedup.png");
				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("이동속도 1성\n\n이동속도 + 5% 상승\n\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("이동속도 2성\n\n이동속도 + 7% 상승\n\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("이동속도 3성\n\n이동속도 + 10% 상승\n\n\n가격: 100");
					}
					break;
					default:
						break;
				}
			}
			break;
			case Protocol::PARTS_RELOADTIME :
			{
				// 장전 속도
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\Loadspeedup.png");
				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("장전속도 1성\n\n장전속도 -0.01감소\n\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("장전속도 2성\n\n장전속도 -0.02감소\n\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("장전속도 3성\n\n장전속도 -0.03감소\n\n\n가격: 100");
					}
					break;
					default:
						break;
				}

			}
			break;
			case Protocol::PARTS_BULLETCAPACITY :
			{
				// 장탄 증가 1성
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\Bulletmountup.png");
				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("장탄증가 1성\n\n장탄 +3 증가\n\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("장탄증가 2성\n\n장탄 +5 증가\n\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("장탄증가 3성\n\n장탄 +7 증가\n\n\n가격: 100");
					}
					break;
					default:
						break;
				}
			}
			break;
			case Protocol::PARTS_DAMAGE :
			{
				// 공격력
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\Strikingpower.png");
				switch (parts.second)
				{
					case 1:
					{
						_partsText[index]->SetText("공격력 1성\n\n공격력 +3 증가\n\n\n가격: 30");
					}
					break;
					case 2:
					{
						_partsText[index]->SetText("공격력 2성\n\n공격력 +5 증가\n\n\n가격: 70");
					}
					break;
					case 3:
					{
						_partsText[index]->SetText("공격력 3성\n\n공격력 +7 증가\n\n\n가격: 100");
					}
					break;
					default:
						break;
				}
			}
			break;
			case Protocol::PARTS_ISABLEDOUBLEJUMP :
			{
				// 2단 점프
				_partsSprite[index]->SetPath("..\\UI\\Card\\PartsIcons\\Jumpx2.png");
				_partsText[index]->SetText("2단 점프\n\n2단 점프 가능\n\n\n가격: 70");
			}
			break;

			default:
				break;
		}
		index++;
	}

}

void GameManager::OnClickParts(int index)
{
	while (ShowCursor(true) < 0) {}

	if (_isPartsClicked[index])
	{
		return;
	}

	NetworkManager::Instance().OnClickSelectParts(index);
}

void GameManager::SuccessPurchase(int index)
{
	RocketEngine::PlayAudioOnceNested("../Sound/PartsBuy/parts_buy.wav",1.0f);

	_isPartsClicked[index] = true;

	RocketEngine::Transform* cardTr = _partsSprite[index]->gameObject->transform.GetParent();
	RocketEngine::RMFLOAT3 destination = cardTr->GetLocalPosition();
	destination.y -= 1000.0f;

	RocketEngine::Tween* tween = RocketEngine::CreateTween();
	tween->GetData(&cardTr->GetLocalPositionRef())
		.DoMove(destination, 1)
		.SetEase(RocketEngine::INOUTBACK);
}

void GameManager::OnRevive()
{
	NetworkManager::Instance().OnClickRespawnButton();
}

void GameManager::OnReroll()
{
	NetworkManager::Instance().OnClickRerollShopParts();
}

void GameManager::OnDie()
{
	_reviveTime = 10.0f;
	_isPartsClicked[0] = false;
	_isPartsClicked[1] = false;
	_isPartsClicked[2] = false;
	while (ShowCursor(true) < 0) {}
	_inGameCanvas->Disable();
	_deadCanvas->Enable();
}

void GameManager::ChangeGunSprite(std::string path)
{
	_gunSprite->SetPath(path);
}

void GameManager::AssignInGameCanvas(RocketEngine::GameObject* obj)
{
	_inGameCanvas = obj;
}

void GameManager::AssignPlayer1KillText(RocketEngine::TextBox* textBox)
{
	_player1KillText = textBox;
}

void GameManager::AssignPlayer2KillText(RocketEngine::TextBox* textBox)
{
	_player2KillText = textBox;
}

void GameManager::AssignPlayer3KillText(RocketEngine::TextBox* textBox)
{
	_player3KillText = textBox;
}

void GameManager::AssignPlayer4KillText(RocketEngine::TextBox* textBox)
{
	_player4KillText = textBox;
}

void GameManager::AssignRoundTimeText(RocketEngine::TextBox* textBox)
{
	_roundTimeText = textBox;
}

void GameManager::AssignPlayerSprite(RocketEngine::SpriteRenderer* renderer)
{
	_playerSprite = renderer;
}

void GameManager::AssignHPBarSprite(RocketEngine::SpriteRenderer* renderer)
{
	_hpBarSprite = renderer;
}

void GameManager::AssignAmmoText(RocketEngine::TextBox* textBox)
{
	textBox->SetText(std::to_string(_playersIndex[myIndex]->stats.bulletCapacity));
	_ammoObj = textBox;
}

void GameManager::AssignGunSprite(RocketEngine::SpriteRenderer* renderer)
{
	_gunSprite = renderer;
}

void GameManager::AssignReviveTimeText(RocketEngine::TextBox* textBox)
{
	_reviveTimeTextBox = textBox;
}

void GameManager::AssignGoldText(RocketEngine::TextBox* textBox)
{
	_goldText = textBox;
}

void GameManager::AssignDeadCanvas(RocketEngine::GameObject* obj)
{
	_deadCanvas = obj;
}

void GameManager::AssignPartsSpriteInfo(RocketEngine::SpriteRenderer* renderer, int index)
{
	_partsSprite[index] = renderer;
	_partsOriginalPos[index] = renderer->gameObject->transform.GetParent()->GetLocalPosition();
}

void GameManager::AssignPartsTextInfo(RocketEngine::TextBox* textBox, int index)
{
	_partsText[index] = textBox;
}

void GameManager::AssignGunModeling(RocketEngine::GameObject* obj, int index)
{
	_gunModeling[index] = obj;
}

void GameManager::PopUpRoundOver()
{
	_inGameCanvas->Disable();
	_deadCanvas->Disable();
	_roundOverObj->Enable();
}

void GameManager::ShowResult()
{
	_inGameCanvas->Disable();
	_deadCanvas->Disable();
	_roundOverObj->Disable();
	_roundTotalCanvas->FlushEnable();

	while (ShowCursor(true) < 0) {}
	_isGameEnd = true;
}

void GameManager::ReturnToLobby()
{
	_roundTotalCanvas->Disable();

	// TODO : 로비로 돌아가자.
	_isGameEnd = false;
	while (ShowCursor(true) < 0) {}
	RocketEngine::LoadScene("MainMenu");
}

bool GameManager::IsGameEnd()
{
	return _isGameEnd;
}

void GameManager::DisableUnused(int index)
{
	for (int i = index; i < 4; ++i)
	{
		_rankProfile[i]->gameObject->Disable();
		_rankKill[i]->gameObject->Disable();
		_rankDeath[i]->gameObject->Disable();
		_rankGold[i]->gameObject->Disable();
	}
}

void GameManager::SetRankProfile(std::string path, int index)
{
	_rankProfile[index]->SetPath(path);
}

void GameManager::SetRankKill(std::string text, int index)
{
	_rankKill[index]->SetText(text);
}

void GameManager::SetRankDeath(std::string text, int index)
{
	_rankDeath[index]->SetText(text);
}

void GameManager::SetRankGold(std::string text, int index)
{
	_rankGold[index]->SetText(text);
}

void GameManager::AssignRoundTotalCanvas(RocketEngine::GameObject* obj)
{
	_roundTotalCanvas = obj;
}

void GameManager::AssignRoundOverObj(RocketEngine::GameObject* obj)
{
	_roundOverObj = obj;
}

void GameManager::AssignRankProfile(RocketEngine::SpriteRenderer* renderer, int index)
{
	_rankProfile[index] = renderer;
}

void GameManager::AssignRankKillText(RocketEngine::TextBox* textBox, int index)
{
	_rankKill[index] = textBox;
}

void GameManager::AssignRankDeathText(RocketEngine::TextBox* textBox, int index)
{
	_rankDeath[index] = textBox;
}

void GameManager::AssignRankGoldText(RocketEngine::TextBox* textBox, int index)
{
	_rankGold[index] = textBox;
}

#pragma endregion UI관련
