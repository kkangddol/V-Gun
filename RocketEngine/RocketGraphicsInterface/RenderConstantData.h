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
		//실제로 셰이더 넣을 때는, 개수 제한 넣어야.
		std::vector<DirectionalLightData> dirLightList;
		std::vector<PointLightData> pointLightList;
		std::vector<SpotLightData> spotLightList;

		float deltaTime;
	};
}
