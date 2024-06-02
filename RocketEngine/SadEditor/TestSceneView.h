#pragma once

namespace RocketEngine
{
	class Scene;
}

class TestSceneView
{
public:
	TestSceneView();

public:
	void Initialize();

public:
	RocketEngine::Scene* scene;

private:
	//RocketAPILoader& _rocket;
};
