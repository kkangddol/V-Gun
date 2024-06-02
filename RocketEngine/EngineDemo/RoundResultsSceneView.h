#pragma once

namespace RocketEngine
{
	class Scene;
}

class RoundResultsSceneView
{
public:
	RoundResultsSceneView();

public:
	void Initialize();

private:
	void RoundResult();
	void TotalResult();

public:
	RocketEngine::Scene* _scene;
};

