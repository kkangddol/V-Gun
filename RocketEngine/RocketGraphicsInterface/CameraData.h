#pragma once
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore::Graphics
{
	struct CameraData
	{
		RocketEngine::RMFLOAT3 position;
		RocketEngine::RMQuaternion rotation;
		//RocketEngine::RMFLOAT4x4 matrix;		// Mk.2 때 카메라 효과? 줄때 사용할 예정.

		float nearZ;					// frustum의 가까운 평면까지의 거리
		float farZ;					// frustum의 먼 평면까지의 거리
		float aspect;					// 가로 / 세로 비율
		float fovY;					// fov각도를 60분법으로 갖고있음
		float nearWindowHeight;		// frustum의 가까운 평면의 높이
		float farWindowHeight;			// frustum의 먼 평면의 높이
	};
}
