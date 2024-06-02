#pragma once
#include "DLLExporter.h"
#include "MeshRendererBase.h"
#include "..\\RocketGraphicsInterface\\ModelData.h"

namespace RocketEngine
{
	class GameObject;

	class ROCKET_API SkinnedMeshRenderer final : public RocketCore::MeshRendererBase
	{
	public:
		SkinnedMeshRenderer(GameObject* owner);
	};
}
