#pragma once
///
/// 23.08.03 오수안
/// 에디터의 패널 전체를 관리하는 패널 시스템
/// 

#include "../EngineDemo/RocketAPILoader.h"

class PanelSystem
{
public:
	PanelSystem();
	~PanelSystem();

public:
	void Start();
	void Update();

private:
	RocketEngine::EngineProcess* _engine;
	bool _isPanelInit;
	float _cameraMoveSpeed;

private:
	RocketEngine::Scene* _currentScene;
};

