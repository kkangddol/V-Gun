#pragma once

namespace RocketEngine
{
	class Scene;
}

class RoundEndSceneView
{
public:
	RoundEndSceneView();

public:
	void Initialize();
	
private:
	void RoundEnd();

private:
	void AddSound();

public:
	RocketEngine::Scene* scene;
};

