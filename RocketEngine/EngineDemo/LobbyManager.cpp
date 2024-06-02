#include "LobbyManager.h"
#include "RocketAPILoader.h"
#include <windows.h>
#include <string>
#include "NetworkManager.h"

static RocketEngine::GameObject* GO = nullptr;

LobbyManager& LobbyManager::Instance()
{
	static LobbyManager instance(GO);
	return instance;
}

LobbyManager::LobbyManager(RocketEngine::GameObject* owner)
	: RocketEngine::Component(owner),
	_playerSlots()
{
	RocketEngine::AddStaticComponent(this);
	for (int i = 0; i < 5; i++)
	{
		_playerSlots[i] = nullptr;
	}
}

void LobbyManager::Start()
{
	if(_mainCanvas != nullptr)	_mainCanvas->Enable();
	if (_joinCanvas != nullptr)	_joinCanvas->Disable();
	if (_tutorialCanvas != nullptr)	_tutorialCanvas->Disable();
	if (_settingCanvas != nullptr)	_settingCanvas->Disable();
	if (_lobbyCanvas != nullptr)	_lobbyCanvas->Disable();
	if (_roomSettingCanvas != nullptr)	_roomSettingCanvas->Disable();
	for (int i = 1; i < 5; i++)
	{
		if (_playerSlots[i] != nullptr)	_playerSlots[i]->Disable();
	}
	
	// 로비의 BGM
	RocketEngine::AddAudio("../Sound/TitleBGM/title_bgm_50.wav", BackGroundMusic);
	RocketEngine::PlayAudioRepeat("../Sound/TitleBGM/title_bgm_50.wav", 1.0f);
}

Protocol::RoomState LobbyManager::GetStatus()
{
	return _nowStatus;
}

void LobbyManager::SetStatus(Protocol::RoomState status)
{
	_nowStatus = status;
}

void LobbyManager::CreateRoom()
{
	if (!NetworkManager::Instance().IsConnected())
	{
		return;
	}

	// 모든 UI 끄고 방 생성.
	NetworkManager::Instance().OnClickCreateRoomButton();
}

void LobbyManager::FailCreateRoom()
{
	_mainCanvas->Enable();
}

void LobbyManager::JoinRoom()
{
	_joinCanvas->Enable();
}

void LobbyManager::Tutorial()
{
	_tutorialCanvas->Enable();
}

void LobbyManager::Setting()
{
	_settingCanvas->Enable();
}

void LobbyManager::QuitGame()
{
	RocketEngine::RocketDestroyWindow();
}

void LobbyManager::ExitJoinRoom()
{
	_joinCanvas->Disable();
}

void LobbyManager::JoinWithCode()
{
	if (!NetworkManager::Instance().IsConnected())
	{
		return;
	}

	// joinUI의 TextBox에서 텍스트(코드) 갖고와서 방 입장 시도
	std::string codeStr = _codeTextBox->GetText();

	if (codeStr.length() != 4)
	{
		return;
	}

	for (auto c : codeStr)
	{
		if (c < 0x30 || 0x39 < c)
		{
			return;
		}
	}

	int code = stoi(codeStr);

	if (code < 1000 || 9999 < code)
	{
		return;
	}

	NetworkManager::Instance().OnClickEnterRoomButton(code);
}

void LobbyManager::SuccessJoinRoom()
{
	_mainCanvas->Disable();
	_joinCanvas->Disable();
	_tutorialCanvas->Disable();
	_settingCanvas->Disable();

	SetStatus(Protocol::RoomState::ROOM_STATE_LOBBY);

	RocketEngine::GameObject* camObj = RocketEngine::GetMainCamera()->gameObject;

	RocketEngine::Tween* camTween = RocketEngine::CreateTween();
	camTween->GetData(&(camObj->transform.GetLocalPositionRef()))
		.DoMove({ 0.0f,1.0f,10.0f }, 1.5f)
		.Delay(0.05f)
		.SetEase(RocketEngine::OUTBACK)
		.OnComplete([this]()
			{
				_lobbyCanvas->FlushEnable();
				_roomSettingObj->FlushDisable();
				_gameStartButton->FlushDisable();	// 방장만, 2인이상일때 활성화!
				if (_isHost)
				{
					_roomSettingObj->Enable();
				}
			});
}

void LobbyManager::FailJoinRoom()
{
	_mainCanvas->Enable();
}

void LobbyManager::ExitTutorial()
{
	_tutorialCanvas->Disable();
}

void LobbyManager::UpSensitivity()
{
	float temp = RocketEngine::GetMouseSensitivity() * 1.2f;
	RocketEngine::SetMouseSensitivity(temp);
}

void LobbyManager::DownSensitivity()
{
	float temp = RocketEngine::GetMouseSensitivity() / 1.2f;
	RocketEngine::SetMouseSensitivity(temp);
}

void LobbyManager::UpBGM()
{
	float temp = RocketEngine::GetGroupVolume(eSoundGroup::BackGroundMusic) * 1.2f;
	RocketEngine::SetGroupVolume(eSoundGroup::BackGroundMusic, temp);
}

void LobbyManager::DownBGM()
{
	float temp = RocketEngine::GetGroupVolume(eSoundGroup::BackGroundMusic) / 1.2f;
	RocketEngine::SetGroupVolume(eSoundGroup::BackGroundMusic, temp);
}

void LobbyManager::UpEffectSound()
{
	float temp = RocketEngine::GetGroupVolume(eSoundGroup::EffectSound) * 1.2f;
	RocketEngine::SetGroupVolume(eSoundGroup::EffectSound, temp);
}

void LobbyManager::DownEffectSound()
{
	float temp = RocketEngine::GetGroupVolume(eSoundGroup::EffectSound) / 1.2f;
	RocketEngine::SetGroupVolume(eSoundGroup::EffectSound, temp);
}

void LobbyManager::ExitSetting()
{
	_settingCanvas->Disable();
}

void LobbyManager::OnClickGameStartButton()
{
	if (!_isHost)
	{
		return;
	}

	if (_playerCount < 2)
	{
		return;
	}

	// TODO : 현재 방에 있는 인원들에게 게임 시작하라고 뿌리기
	NetworkManager::Instance().OnClickGameStartButton();
}

void LobbyManager::ReceiveGameStart(int roundCount, int roundTime)
{
	_roundKillCount = roundCount;
	_roundTime = roundTime;

	//RocketEngine::PlayEnd("../Sound/TitleBGM/title_bgm_50.wav");
	RocketEngine::PlayAllEnd();
	RocketEngine::PlayAudioRepeat("../Sound/BGM/ingame_bgm2_50_50.wav", 1.0f);

	ShowCursor(FALSE);
	RocketEngine::LoadScene("MainGame");
}

void LobbyManager::EnterPlayer(int index, uint64_t id)
{
	if (_isHost)
	{
		_roomSettingObj->Enable();
	}

	_playerCount++;
	if (2 <= _playerCount && _isHost)
	{
		_gameStartButton->Enable();
	}
	_playerSlots[index]->Enable();
}

void LobbyManager::LeavePlayer(int index)
{
	if (_isHost)
	{
		_roomSettingObj->Enable();
	}

	_playerCount--;
	if (_playerCount < 2 && _isHost)
	{
		_gameStartButton->Disable();
	}
	_playerSlots[index]->Disable();
}

void LobbyManager::RoomSetting()
{
	_roomSettingCanvas->Enable();
}

void LobbyManager::ExitRoomSetting()
{
	_roomSettingCanvas->Disable();
	NetworkManager::Instance().OnClickChangeRoomSetting(_roundKillCount, _roundTime);
}

void LobbyManager::UpRoundKill()
{
	_roundKillCount++;
	if (60 < _roundKillCount)
	{
		_roundKillCount = 60;
	}

	_roundKillCountText->SetText(std::to_string(_roundKillCount));
}

void LobbyManager::DownRoundKill()
{
	_roundKillCount--;
	if (_roundKillCount < 10)
	{
		_roundKillCount = 10;
	}

	_roundKillCountText->SetText(std::to_string(_roundKillCount));

}

void LobbyManager::UpRoundTime()
{
	_roundTime += 10;
	if (300 < _roundTime)
	{
		_roundTime = 300;
	}

	_roundTimeText->SetText(std::to_string(_roundTime));
}

void LobbyManager::DownRoundTime()
{
	_roundTime -= 10;
	if (_roundTime < 10)
	{
		_roundTime = 10;
	}

	_roundTimeText->SetText(std::to_string(_roundTime));
}

void LobbyManager::EnableRoomSettingBtn()
{
	_roomSettingObj->Enable();
}

void LobbyManager::ExitRoom()
{
	NetworkManager::Instance().OnClickExitRoom();

	SetStatus(Protocol::ROOM_STATE_NONE);
	_lobbyCanvas->Disable();
	_roomSettingCanvas->Disable();

	RocketEngine::GameObject* camObj = RocketEngine::GetMainCamera()->gameObject;

	RocketEngine::Tween* camTween = RocketEngine::CreateTween();
	camTween->GetData(&(camObj->transform.GetLocalPositionRef()))
		.DoMove({ 0.0f,5.0f,20.0f }, 1.5f)
		.SetEase(RocketEngine::INBACK)
		.OnComplete([this]()
			{
				_mainCanvas->Enable();
			});
}

RocketEngine::GameObject* LobbyManager::GetLobbyCanvas()
{
	return _lobbyCanvas;
}

RocketEngine::GameObject* LobbyManager::GetRoomSettingCanvas()
{
	return _roomSettingCanvas;
}

RocketEngine::TextBox* LobbyManager::GetRoomCodeTextBox()
{
	return _roomCodeTextBox;
}

RocketEngine::GameObject* LobbyManager::GetGameStartButton()
{
	return _gameStartButton;
}

void LobbyManager::SetLobbyCanvas(RocketEngine::GameObject* canvasObj)
{
	_lobbyCanvas = canvasObj;
}

void LobbyManager::SetRoomSettingCanvas(RocketEngine::GameObject* canvasObj)
{
	_roomSettingCanvas = canvasObj;
}

void LobbyManager::SetRoomCodeTextBox(RocketEngine::TextBox* textBox)
{
	_roomCodeTextBox = textBox;
}

void LobbyManager::SetGameStartButton(RocketEngine::GameObject* buttonObj)
{
	_gameStartButton = buttonObj;
}

void LobbyManager::AssignKillCountTextBox(RocketEngine::TextBox* textBox)
{
	_roundKillCountText = textBox;
}

void LobbyManager::AssignRoundTimeTextBox(RocketEngine::TextBox* textBox)
{
	_roundTimeText = textBox;
}

void LobbyManager::AssignRoomSettingObj(RocketEngine::GameObject* obj)
{
	_roomSettingObj = obj;
}

void LobbyManager::SetPlayerSlot(int index, RocketEngine::GameObject* player)
{
	_playerSlots[index] = player;
}

RocketEngine::GameObject* LobbyManager::GetPlayerSlot(int index)
{
	return _playerSlots[index];
}

void LobbyManager::SetRoomCode(int roomCode)
{
	if (_roomCodeTextBox != nullptr) _roomCodeTextBox->SetText(std::to_string(roomCode));
	_roomCode = roomCode;
}

void LobbyManager::SetRoundKillCount(int roundKillCount)
{
	_roundKillCount = roundKillCount;
	_roundKillCountText->SetText(std::to_string(roundKillCount));
}

void LobbyManager::SetRoundTime(int roundTime)
{
	_roundTime = roundTime;
	_roundTimeText->SetText(std::to_string(roundTime));
}

int LobbyManager::GetRoomCode()
{
	return _roomCode;
}

int LobbyManager::GetRoundKillCount()
{
	return _roundKillCount;
}

int LobbyManager::GetRoundTime()
{
	return _roundTime;
}

uint64_t LobbyManager::GetID()
{
	return _myID;
}

bool LobbyManager::IsHost()
{
	return _isHost;
}

int LobbyManager::GetIndex()
{
	return _myIndex;
}

void LobbyManager::SetID(uint64_t id)
{
	_myID = id;
}

void LobbyManager::SetIsHost(bool isHost)
{
	_isHost = isHost;
}

void LobbyManager::SetIndex(int index)
{
	_myIndex = index;

	if (_myIndex != 1)
	{
		_isHost = false;
	}
}

RocketEngine::GameObject* LobbyManager::GetMainCanvas()
{
	return _mainCanvas;
}

RocketEngine::GameObject* LobbyManager::GetJoinCanvas()
{
	return _joinCanvas;
}

RocketEngine::GameObject* LobbyManager::GetTutorialCanvas()
{
	return _tutorialCanvas;
}

RocketEngine::GameObject* LobbyManager::GetSettingCanvas()
{
	return _settingCanvas;
}

RocketEngine::TextBox* LobbyManager::GetCodeInputTextBox()
{
	return _codeTextBox;
}

void LobbyManager::SetMainCanvas(RocketEngine::GameObject* canvasObj)
{
	_mainCanvas = canvasObj;
}

void LobbyManager::SetJoinCanvas(RocketEngine::GameObject* canvasObj)
{
	_joinCanvas = canvasObj;
}

void LobbyManager::SetTutorialCanvas(RocketEngine::GameObject* canvasObj)
{
	_tutorialCanvas = canvasObj;
}

void LobbyManager::SetSettingCanvas(RocketEngine::GameObject* canvasObj)
{
	_settingCanvas = canvasObj;
}

void LobbyManager::SetCodeInputTextBox(RocketEngine::TextBox* textBox)
{
	_codeTextBox = textBox;
}
