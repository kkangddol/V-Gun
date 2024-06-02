///�׷��Ƚ� �������� ���� ������ Camera ����� �޾Ƽ� �������� Ȱ��.

#pragma once
#include <DirectXMath.h>
#include <array>
#include "../RocketGraphicsInterface/CameraData.h"
#include "MathHelper.h"
#include "SimpleMath.h"

namespace RocketCore::Graphics
{
	struct CameraMatrices
	{
		CameraMatrices();
		void UpdateLocalAxisValues(const CameraData* camData); // 1
		void UpdateViewMatrix(const CameraData* camData); // 2
		void UpdateFrustumPlanes(); // 3 
		DirectX::XMFLOAT4X4 viewMat;
		DirectX::XMFLOAT4X4 projectionMat;

		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mRight;
		DirectX::XMFLOAT3 mUp;
		DirectX::XMFLOAT3 mLook;

		DirectX::XMMATRIX View() const;
		DirectX::XMMATRIX Proj() const;

		std::array<DirectX::XMFLOAT4, 6> _frustumPlanes;
	};
}