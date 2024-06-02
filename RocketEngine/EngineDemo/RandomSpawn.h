#pragma once
#include "RocketAPILoader.h"

/// <summary>
/// 랜덤 스폰을 담당하는 클래스. 
/// </summary>

class RandomSpawn : public RocketEngine::Component
{
public:
	RandomSpawn(RocketEngine::GameObject* owner);

	void SetSpawnPos(float x, float y, float);
	RocketEngine::RMFLOAT3& GetSpawnerPos();
};

