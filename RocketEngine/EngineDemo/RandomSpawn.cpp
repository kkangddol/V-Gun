#include "RandomSpawn.h"

RandomSpawn::RandomSpawn(RocketEngine::GameObject* owner)
	: RocketEngine::Component(owner)
{

}

void RandomSpawn::SetSpawnPos(float x, float y, float z)
{
	gameObject->transform.SetLocalPosition(x, y, z);
}

RocketEngine::RMFLOAT3& RandomSpawn::GetSpawnerPos()
{
	return gameObject->transform.GetLocalPositionRef();
}
