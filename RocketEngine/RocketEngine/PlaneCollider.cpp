#include "PlaneCollider.h"

#include "DebugSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace RocketEngine
{
	PlaneCollider::PlaneCollider(GameObject* owner)
		: StaticCollider(owner)
	{

	}

	/// 디버그 정보 출력.
	void PlaneCollider::Update()
	{
		RMFLOAT4X4 worldTM = gameObject->transform.GetWorldTM();
		RMFLOAT3 WHD = { 0.001f,100.0f,100.0f};
		RMFLOAT4 color = { 0.0f,0.0f,1.0f,1.0f };
		RocketCore::DebugSystem::Instance().DrawDebugBox(worldTM, WHD, true, color);
	}

	RocketEngine::RMFLOAT3 PlaneCollider::GetNormalVector() const
	{
		RMFLOAT3 axis = gameObject->transform.GetRight();
		return RMFloat3Normalize(axis);
	}

	float PlaneCollider::GetDistance() const
	{
		return RMFloat3Length(gameObject->transform.GetLocalPosition());
	}
}

