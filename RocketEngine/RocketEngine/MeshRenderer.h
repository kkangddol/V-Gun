#pragma once
#include "DLLExporter.h"
#include "MeshRendererBase.h"
#include "..\\RocketGraphicsInterface\\ModelData.h"

namespace RocketEngine
{
	class GameObject;

	class ROCKET_API MeshRenderer final : public RocketCore::MeshRendererBase
	{
	public:
		MeshRenderer(GameObject* owner);
	};
}

