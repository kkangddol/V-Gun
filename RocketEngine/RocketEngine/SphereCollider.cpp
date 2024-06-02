#include "SphereCollider.h"

#include "DebugSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace RocketEngine
{
	SphereCollider::SphereCollider(GameObject* owner)
		: DynamicCollider(owner)
	{

	}

	void SphereCollider::Update()
	{
		RMFLOAT4X4 worldTM = GetOffsetTM() * gameObject->transform.GetWorldTM();
		RMFLOAT3 WHD = { GetWidth(), GetHeight(), GetDepth() };
		RMFLOAT4 color = { 0.0f,1.0f,0.0f,1.0f };
		RocketCore::DebugSystem::Instance().DrawDebugBox(worldTM, WHD, true, color);
	}

	float SphereCollider::GetWidth() const
	{
		return _radius * 2 * _scaleOffset.x * gameObject->transform.GetScale().x;
	}

	float SphereCollider::GetHeight() const
	{
		return _radius * 2 * _scaleOffset.y * gameObject->transform.GetScale().y;
	}

	float SphereCollider::GetDepth() const
	{
		return _radius * 2 * _scaleOffset.z * gameObject->transform.GetScale().z;
	}

	float SphereCollider::GetRadius() const
	{
		return _radius;
	}

	void SphereCollider::SetRadius(float radius)
	{
		_radius = radius;
	}

}


