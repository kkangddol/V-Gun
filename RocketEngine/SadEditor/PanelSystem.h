#pragma once
///
/// 23.08.03 ������
/// �������� �г� ��ü�� �����ϴ� �г� �ý���
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

