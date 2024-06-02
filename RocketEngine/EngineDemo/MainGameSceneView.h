#pragma once

class GameManager;
class LobbyManager;

namespace RocketEngine
{
	class Scene;
	class Camera;
}

class MainGameSceneView
{
public:
	MainGameSceneView();

public:
	void Initialize();

private:
	void MapObject();
	void Player();
	void InGameUI();
	void DeathUI();
	void SettingUI();
	void RoundEnd();
	void RoundResultUI();
	void TotalResultUI();

private:
	GameManager& _gameManager;
	LobbyManager& _lobbyManager;

public:
	RocketEngine::Scene* scene;

private:

};

