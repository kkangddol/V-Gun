#pragma once

namespace RocketEngine
{
	class Scene;
}

class GameLobbySceneView
{
public:
	GameLobbySceneView();

public:
	void Initialize();
public:
	RocketEngine::Scene* scene;
};
