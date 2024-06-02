#pragma once

class LobbyManager;

namespace RocketEngine
{
	class Scene;
}

class MainMenuSceneView
{
public:
	MainMenuSceneView();

public:
	void Initialize();

private:
	void MainMenu();

private:
	void Lobby();

private:
	void LobbyObject();
	void LobbyObject2();

private:
	void LobbyPlayerSlot();

private:
	void AddSound();

private:
	RocketEngine::Scene* _scene;
	LobbyManager& _lobbyManager;

private:
	bool testBool = true;
};

