#include "RenderUsageBoneInfo.h"
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	RenderUsageBoneInfo::RenderUsageBoneInfo()
	{
		BoneOffset = DirectX::SimpleMath::Matrix::Identity;
		FinalTransformation = DirectX::SimpleMath::Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
}
