#pragma once
#include "RocketAPILoader.h"
#include <string>
#include <vector>
#include "Struct.pb.h"

class PlayerInfo;

namespace RocketEngine
{
	class TextBox;
	class SpriteRenderer;
	class AnimationController;
}

class GameManager final : public RocketEngine::Component
{
public:
	static GameManager& Instance();

private:
	GameManager(RocketEngine::GameObject* owner);
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void InitGameManager();
	void InitRound();
public:
	using uint32 = unsigned __int32;
	using uint64 = unsigned __int64;
	PlayerInfo* GetPlayerInfo();
	PlayerInfo* GetPlayerInfo(uint32 index);
	PlayerInfo* GetPlayerInfo(uint64 id);
	PlayerInfo* GetPlayerInfo(const Protocol::Player& player);
	PlayerInfo* GetPlayerInfo(RocketEngine::GameObject* gameObject);

	void EnterPlayer(const Protocol::Player& player);
	void LeavePlayer(const Protocol::Player& player);
	void SpawnPlayer(RocketEngine::GameObject* player, int posIndex);
	std::string OnStateChanged();

public:
	void AddPlayerInfo(PlayerInfo* playerInfo);
	void SetMyPlayer(int index);
	void OnStartGame(int index);
	void SetRoundInfo(Protocol::RoundState state, uint32 time);

	// 세팅창
	void SetSettingCanvas(RocketEngine::GameObject* canvasObj);
	void Setting();
	void ExitSetting();

public:
	uint64 myId;
	uint32 myIndex;

	Protocol::RoundState state;

	int currentKill;
	int totalKill;
	float roundTime = 0.0f;
	float remainTime = 0.0f;

public:
	RocketEngine::GameObject* myPlayer;

	// 파츠 이름, 등급
	std::vector<std::pair<Protocol::Parts, int>> selectPartsList;
private:
	RocketEngine::RMFLOAT3 lastSendPosition;
	RocketEngine::RMQuaternion lastSendRotation;

	std::vector<PlayerInfo*> _playersIndex;

	std::map<uint64, PlayerInfo*> _playersMap;

	std::vector<RocketEngine::Transform*> _randomSpawnPos;

	/////////////////////////////////
	/// UI 관련
public:
	void SetPlayerKillCountText(const int killNum, int playerIndex);

	void SetRoundTimeText(const float timeNow);
	void SetPlayerSpritePath(const std::string& path);
	void SetPlayerHpBar(float hp);
	void SetAmmoText(const std::string& text);
	void SetGunSpritePath(const std::string& path);
	void ChangeGunModeling(int index);
	void UpdateGoldText(std::string text);

	void UpdatePartsInfo();
	void OnClickParts(int index);
	void SuccessPurchase(int index);
	void OnRevive();
	void OnReroll();

	// 죽었을때
public:
	void OnDie();
	void ChangeGunSprite(std::string path);


private:
	float _reviveTime = 0.0f;

	/// UI 객체들
public:
	void AssignInGameCanvas(RocketEngine::GameObject* obj);
	void AssignPlayer1KillText(RocketEngine::TextBox* textBox);
	void AssignPlayer2KillText(RocketEngine::TextBox* textBox);
	void AssignPlayer3KillText(RocketEngine::TextBox* textBox);
	void AssignPlayer4KillText(RocketEngine::TextBox* textBox);
	void AssignRoundTimeText(RocketEngine::TextBox* textBox);
	void AssignPlayerSprite(RocketEngine::SpriteRenderer* renderer);
	void AssignHPBarSprite(RocketEngine::SpriteRenderer* renderer);
	void AssignAmmoText(RocketEngine::TextBox* textBox);
	void AssignGunSprite(RocketEngine::SpriteRenderer* renderer);
	void AssignReviveTimeText(RocketEngine::TextBox* textBox);
	void AssignGoldText(RocketEngine::TextBox* textBox);

	// parts
	void AssignDeadCanvas(RocketEngine::GameObject* obj);
	void AssignPartsSpriteInfo(RocketEngine::SpriteRenderer* renderer, int index);
	void AssignPartsTextInfo(RocketEngine::TextBox* textBox, int index);
	void AssignGunModeling(RocketEngine::GameObject* obj, int index);

private:
	RocketEngine::GameObject* _inGameCanvas = nullptr;
	RocketEngine::TextBox* _player1KillText = nullptr;
	RocketEngine::TextBox* _player2KillText = nullptr;
	RocketEngine::TextBox* _player3KillText = nullptr;
	RocketEngine::TextBox* _player4KillText = nullptr;
	RocketEngine::TextBox* _roundTimeText = nullptr;
	RocketEngine::SpriteRenderer* _playerSprite = nullptr;
	RocketEngine::SpriteRenderer* _hpBarSprite = nullptr;
	RocketEngine::TextBox* _ammoObj = nullptr;
	RocketEngine::SpriteRenderer* _gunSprite = nullptr;
	RocketEngine::GameObject* _settingCanvas = nullptr;

	RocketEngine::GameObject* _deadCanvas = nullptr;
	RocketEngine::SpriteRenderer* _partsSprite[3];
	RocketEngine::TextBox* _partsText[3];
	RocketEngine::RMFLOAT3 _partsOriginalPos[3];
	bool _isPartsClicked[3];

	RocketEngine::TextBox* _reviveTimeTextBox = nullptr;
	RocketEngine::TextBox* _goldText = nullptr;
	RocketEngine::GameObject* _gunModeling[4];				// 1인칭 총도 UI로 치나?



	/// 결과 UI
	public:
		void PopUpRoundOver();
		void ShowResult();
		void ReturnToLobby();

	public:
		bool IsGameEnd();
	private:
		bool _isGameEnd = false;

	public:
		void DisableUnused(int index);

	public:
		void SetRankProfile(std::string path, int index);
		void SetRankKill(std::string text, int index);
		void SetRankDeath(std::string text, int index);
		void SetRankGold(std::string text, int index);

	public:
		void AssignRoundTotalCanvas(RocketEngine::GameObject* obj);
		void AssignRoundOverObj(RocketEngine::GameObject* obj);
		void AssignRankProfile(RocketEngine::SpriteRenderer* renderer, int index);
		void AssignRankKillText(RocketEngine::TextBox* textBox, int index);
		void AssignRankDeathText(RocketEngine::TextBox* textBox, int index);
		void AssignRankGoldText(RocketEngine::TextBox* textBox, int index);

	private:
		RocketEngine::GameObject* _roundTotalCanvas;
		RocketEngine::GameObject* _roundOverObj;
		RocketEngine::SpriteRenderer* _rankProfile[4];
		RocketEngine::TextBox* _rankKill[4];
		RocketEngine::TextBox* _rankDeath[4];
		RocketEngine::TextBox* _rankGold[4];
};

