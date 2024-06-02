#pragma once
#include "RocketAPILoader.h"

/// <summary>
/// ���� ������ ����ϴ� Ŭ����. 
/// </summary>

class RandomSpawn : public RocketEngine::Component
{
public:
	RandomSpawn(RocketEngine::GameObject* owner);

	void SetSpawnPos(float x, float y, float);
	RocketEngine::RMFLOAT3& GetSpawnerPos();
};

