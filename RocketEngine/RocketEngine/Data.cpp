#include "Data.h"

namespace RocketCore
{
	RocketEngine::RMFLOAT3 DataStruct::GetPosition()
	{
		return { transform->position_x, transform->position_y, transform->position_z };
	}

	RocketEngine::RMQuaternion DataStruct::GetRotation()
	{
		return { transform->rotation_w, transform->rotation_x, transform->rotation_y, transform->rotation_z };
	}

	RocketEngine::RMFLOAT3 DataStruct::GetScale()
	{
		return { transform->scale_x, transform->scale_y, transform->scale_z };
	}

	RocketEngine::RMFLOAT3 boxColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	RocketEngine::RMQuaternion boxColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	RocketEngine::RMFLOAT3 boxColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}

	RocketEngine::RMFLOAT3 capsuleColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	RocketEngine::RMQuaternion capsuleColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	RocketEngine::RMFLOAT3 capsuleColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}

	RocketEngine::RMFLOAT3 sphereColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	RocketEngine::RMQuaternion sphereColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	RocketEngine::RMFLOAT3 sphereColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}

	RocketEngine::RMFLOAT3 planeColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	RocketEngine::RMQuaternion planeColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	RocketEngine::RMFLOAT3 planeColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}

	RocketEngine::RMFLOAT3 staticBoxColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	RocketEngine::RMQuaternion staticBoxColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	RocketEngine::RMFLOAT3 staticBoxColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}
}