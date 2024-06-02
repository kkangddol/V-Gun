#pragma once

namespace RocketEngine
{
	class Scene;
}

class InGameSceneView
{
public:
	InGameSceneView();

public:
	void Initialize();

private:
	void InGame();

private:
	void AddSound();

public:
	RocketEngine::Scene* _scene;
};

