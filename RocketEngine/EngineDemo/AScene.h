#pragma once
namespace RocketEngine
{
	class Scene;
}

class AScene
{
public:
	AScene();

public:
	void Initialize();

public:
	RocketEngine::Scene* scene;
};