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
		//RocketEngine::RMFLOAT4x4 matrix;		// Mk.2 �� ī�޶� ȿ��? �ٶ� ����� ����.

		float nearZ;					// frustum�� ����� �������� �Ÿ�
		float farZ;					// frustum�� �� �������� �Ÿ�
		float aspect;					// ���� / ���� ����
		float fovY;					// fov������ 60�й����� ��������
		float nearWindowHeight;		// frustum�� ����� ����� ����
		float farWindowHeight;			// frustum�� �� ����� ����
	};
}
