#pragma once
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore::Graphics
{
	struct DirectionalLightData
	{
		RocketEngine::RMFLOAT4 ambient;
		RocketEngine::RMFLOAT4 diffuse;
		RocketEngine::RMFLOAT4 specular;
		RocketEngine::RMFLOAT3 direction;
		float intensity = 1.0f; 
		//�ڵ����� ����Ʈ �е� ��. Directional Light ���� ���� ��� ������ ���� Intensity ����.
	};

	struct PointLightData
	{
		RocketEngine::RMFLOAT4 ambient;
		RocketEngine::RMFLOAT4 diffuse;
		RocketEngine::RMFLOAT4 specular;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::RMFLOAT3 position;
		float range;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::RMFLOAT3 attenuation;
		float tPad;
	};

	struct SpotLightData
	{
		RocketEngine::RMFLOAT4 ambient;
		RocketEngine::RMFLOAT4 diffuse;
		RocketEngine::RMFLOAT4 specular;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::RMFLOAT3 position;
		float range;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::RMFLOAT3 direction;
		float spot;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::RMFLOAT3 attenuation;
		float tPad;
	};
}

///[TW] : TBA - ���� �� Flax �� Lighting ���� : ���� PBR ���̽�����!
///�׷���, �ϴ� Legacy�� ����� ���ư��� ���� ���� �����ض�.

