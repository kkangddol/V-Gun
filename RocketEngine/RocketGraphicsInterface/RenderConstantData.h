#pragma once
#include <string>
#include <vector>
#include "..\\RocketMath\\RocketMath.h"
#include "LightData.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")

#endif // _DEBUG
namespace RocketCore::Graphics
{
	struct RenderConstantData
	{
		//������ ���̴� ���� ����, ���� ���� �־��.
		std::vector<DirectionalLightData> dirLightList;
		std::vector<PointLightData> pointLightList;
		std::vector<SpotLightData> spotLightList;

		float deltaTime;
	};
}
