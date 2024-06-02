#pragma once

class GameManager;
class LobbyManager;

namespace RocketEngine
{
	class Scene;
	class Camera;
}

class BScene
{
public:
	BScene();

public:
	void Initialize();

private:
	void MapObject();
	void Player();
	void InGameUI();

private:
	GameManager& _gameManager;
	LobbyManager& _lobbyManager;

public:
	RocketEngine::Scene* scene;
};