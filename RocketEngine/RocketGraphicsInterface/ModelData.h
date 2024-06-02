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
		std::string armPath; //AO / Roughness / Metallic �߰�.
		eRenderType renderingType = eRenderType::FORWARD;

		std::string objName; //[TW] ���� ���� ���������� ���ӿ�����Ʈ �̸�.
	};
}