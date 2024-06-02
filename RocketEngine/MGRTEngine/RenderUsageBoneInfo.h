#pragma once
#include "SimpleMath.h"

namespace RocketCore::Graphics
{
	class RenderUsageBoneInfo
	{
	public:
		RenderUsageBoneInfo();
		DirectX::SimpleMath::Matrix FinalTransformation; // Final transformation to apply to vertices 
		DirectX::SimpleMath::Matrix BoneOffset; // Initial offset from local to bone space. 
	};
}


