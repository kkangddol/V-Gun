#pragma once
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	struct Transform
	{
		Transform()
		{
			m_Scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
			m_Rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_Position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
		DirectX::XMFLOAT3 m_Scale;
		DirectX::XMFLOAT3 m_Rotation;
		DirectX::XMFLOAT3 m_Position;
	};
}


