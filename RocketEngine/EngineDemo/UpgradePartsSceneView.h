#pragma once

namespace RocketEngine
{
	class Scene;
}

class UpgradePartsSceneView
{
public:
	UpgradePartsSceneView();

public:
	void Initialize();

private:
	void UpgrdePartsScene();
	void CardScene();

private:
	void AddSound();

public:
	RocketEngine::Scene* scene;
};

