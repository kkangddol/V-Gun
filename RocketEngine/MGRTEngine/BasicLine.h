#pragma once
#include <DirectXMath.h>
#include <memory>
#include "VertexTypes.h"
#include "PrimitiveBatch.h"
#include "Effects.h"
#include "CommonStates.h"
#include <windef.h>

namespace RocketCore::Graphics
{
	class CameraMatrices;

	class BasicLine
	{
	public:
		BasicLine(CameraMatrices* camMatrices);
		virtual ~BasicLine();

		void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primBatch, DirectX::BasicEffect* bEffect, DirectX::CommonStates* cState, DirectX::XMFLOAT3 first, DirectX::XMFLOAT3 second, DirectX::XMFLOAT4 color);

	private:
		CameraMatrices* _camMatrix = nullptr;
		//DrawLine¿ª ¿ß«— PrimitiveBatch
		
	};
}


