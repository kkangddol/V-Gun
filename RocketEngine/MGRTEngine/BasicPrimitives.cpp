#include "BasicPrimitives.h"
#include <PrimitiveBatch.h>
#include <Effects.h>
#include "Define3D.h"

namespace RocketCore::Graphics
{
	BasicPrimitives::BasicPrimitives(ID3D11DeviceContext* devcon, eBasicPrimitiveType primType)
	{
		this->_primType = primType;

		if (_primType == eBasicPrimitiveType::CUBE)
		{
			_geomShape = GeometricPrimitive::CreateCube(devcon, 1.0f, false);
		}
		else if (_primType == eBasicPrimitiveType::SPHERE)
		{
			_geomShape = GeometricPrimitive::CreateSphere(devcon, 1.0f, 8, false, false);
		}
	}

	BasicPrimitives::~BasicPrimitives()
	{
		//
	}

	void BasicPrimitives::Render(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, bool isWire, float r, float g, float b, float a)
	{
		DirectX::XMFLOAT4 tCol = { r, g, b, a };
		DirectX::XMVECTOR tColMat = DirectX::XMLoadFloat4(&tCol);

		DirectX::XMMATRIX tWorld = DirectX::XMLoadFloat4x4(&world);
		DirectX::XMMATRIX tView = DirectX::XMLoadFloat4x4(&view);
		DirectX::XMMATRIX tProj = DirectX::XMLoadFloat4x4(&proj);

		if (isWire)
		{
			_geomShape->Draw(tWorld, tView, tProj, tColMat, nullptr, true);
		}
		else
		{
			_geomShape->Draw(tWorld, tView, tProj, tColMat);
		}

		
	}

	

}