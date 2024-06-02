#pragma once
#include <iostream>
#include <string>
#include "RenderType.h"

namespace RocketCore::Graphics
{
	struct ModelData
	{
		std::string fbxPath;
		std::string shaderPath;
		std::string diffusePath;
		std::string normalPath;
		std::string armPath; //AO / Roughness / Metallic 추가.
		eRenderType renderingType = eRenderType::FORWARD;

		std::string objName; //[TW] 실제 게임 엔진에서의 게임오브젝트 이름.
	};
}