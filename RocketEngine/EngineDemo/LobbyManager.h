#pragma once
#include "RocketAPILoader.h"
#include "Enum.pb.h"

class LobbyManager : public RocketEngine::Component
{
public:
	static LobbyManager& Instance();

private:
	LobbyManager(RocketEngine::GameObject* owner);

protected:
	virtual void Start() override;

	/// Status
public:
	Protocol::RoomState GetStatus();

public:
	void SetStatus(Protocol::RoomState status);

private:
	Protocol::RoomState _nowStatus = Protocol::ROOM_STATE_NONE;

#pragma region MainMenu
	/// 메인 메뉴 기능들
public:
	void CreateRoom();
	void FailCreateRoom();
	void JoinRoom();
	void Tutorial();
	void Setting();
	void QuitGame();

	// JoinRoom
public:
	void ExitJoinRoom();
	void JoinWithCode();
	void SuccessJoinRoom();
	void FailJoinRoom();

	// Tutorial
public:
	void ExitTutorial();

	// Setting
public:
	void UpSensitivity();
	void DownSensitivity();
	void UpBGM();
	void DownBGM();
	void UpEffectSound();
	void DownEffectSound();
	void ExitSetting();

	/// 메인 메뉴 객체들
public:
	RocketEngine::GameObject* GetMainCanvas();
	RocketEngine::GameObject* GetJoinCanvas();
	RocketEngine::GameObject* GetTutorialCanvas();
	RocketEngine::GameObject* GetSettingCanvas();
	RocketEngine::TextBox* GetCodeInputTextBox();

public:
	void SetMainCanvas(RocketEngine::GameObject* canvasObj);
	void SetJoinCanvas(RocketEngine::GameObject* canvasObj);
	void SetTutorialCanvas(RocketEngine::GameObject* canvasObj);
	void SetSettingCanvas(RocketEngine::GameObject* canvasObj);
	void SetCodeInputTextBox(RocketEngine::TextBox* textBox);

private:
	RocketEngine::GameObject* _mainCanvas = nullptr;
	RocketEngine::GameObject* _joinCanvas = nullptr;
	RocketEngine::GameObject* _tutorialCanvas = nullptr;
	RocketEngine::GameObject* _settingCanvas = nullptr;
	RocketEngine::TextBox* _codeTextBox = nullptr;
#pragma endregion MainMenu


#pragma region Lobby
	/// 로비 기능들
public:
	void OnClickGameStartButton();							// 게임시작버튼 눌렀을때
	void ReceiveGameStart(int roundCount, int roundTime);	// 서버로부터 게임시작 메시지를 받았을 때

public:
	void EnterPlayer(int index, uint64_t id);
	void LeavePlayer(int index);

public:
	void RoomSetting();
	void ExitRoomSetting();
	void UpRoundKill();
	void DownRoundKill();
	void UpRoundTime();
	void DownRoundTime();
	void EnableRoomSettingBtn();

	// 방나가기
public:
	void ExitRoom();

	/// 로비 객체들
public:
	RocketEngine::GameObject* GetLobbyCanvas();
	RocketEngine::GameObject* GetRoomSettingCanvas();
	RocketEngine::TextBox* GetRoomCodeTextBox();
	RocketEngine::GameObject* GetGameStartButton();

	void SetLobbyCanvas(RocketEngine::GameObject* canvasObj);
	void SetRoomSettingCanvas(RocketEngine::GameObject* canvasObj);
	void SetRoomCodeTextBox(RocketEngine::TextBox* textBox);
	void SetGameStartButton(RocketEngine::GameObject* buttonObj);
	void AssignKillCountTextBox(RocketEngine::TextBox* textBox);
	void AssignRoundTimeTextBox(RocketEngine::TextBox* textBox);
	void AssignRoomSettingObj(RocketEngine::GameObject* obj);

private:
	RocketEngine::GameObject* _lobbyCanvas = nullptr;
	RocketEngine::GameObject* _roomSettingCanvas = nullptr;
	RocketEngine::GameObject* _roomSettingObj = nullptr;
	RocketEngine::TextBox* _roomCodeTextBox = nullptr;
	RocketEngine::GameObject* _gameStartButton = nullptr;
	RocketEngine::TextBox* _roundKillCountText = nullptr;
	RocketEngine::TextBox* _roundTimeText = nullptr;

public:
	void SetPlayerSlot(int index, RocketEngine::GameObject* player);

	RocketEngine::GameObject* GetPlayerSlot(int index);

private:
	RocketEngine::GameObject* _playerSlots[5];	// 5개지만 0은 안씀, 1~4만 사용.

	/// 로비 정보
public:
	void SetRoomCode(int roomCode);
	void SetRoundKillCount(int roundKillCount);
	void SetRoundTime(int roundTime);

	int GetRoomCode();
	int GetRoundKillCount();
	int GetRoundTime();

private:
	int _roomCode;
	int _roundKillCount = 40;
	int _roundTime = 120;
	int _playerCount = 0;

#pragma endregion Lobby

#pragma region userInfo
	/// 개인 정보
public:
	uint64_t GetID();
	bool IsHost();
	int GetIndex();

	void SetID(uint64_t id);
	void SetIsHost(bool isHost);
	void SetIndex(int index);


private:
	uint64_t _myID;
	int _myIndex;
	bool _isHost = false;
#pragma endregion userInfo
};