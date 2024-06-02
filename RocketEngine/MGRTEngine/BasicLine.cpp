#include "BasicLine.h"
#include "LowLevelDX11.h"
#include "Define3D.h"
#include "DirectXHelpers.h"
#include "CameraMatrices.h"



namespace RocketCore::Graphics
{
	BasicLine::BasicLine(CameraMatrices* camMatrices)
	{
		_camMatrix = camMatrices;
	}

	BasicLine::~BasicLine()
	{

	}

	void BasicLine::Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primBatch, DirectX::BasicEffect* bEffect, DirectX::CommonStates* cState, DirectX::XMFLOAT3 first, DirectX::XMFLOAT3 second, DirectX::XMFLOAT4 color)
	{
		primBatch->DrawLine(DirectX::VertexPositionColor(first, color), DirectX::VertexPositionColor(second, color));
	}
	

}
