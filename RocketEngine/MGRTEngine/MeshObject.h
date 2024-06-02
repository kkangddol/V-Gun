#pragma once
#include "BaseRenderable.h"
#include <DirectXMath.h>
#include <string>
#include <vector>

namespace RocketCore::Graphics
{
	class MeshObject : public BaseRenderable
	{
	public:
		MeshObject(ShaderMaterial* _mat, BaseFormation* _form, 
			CameraMatrices* cameraMat, TextureStorage* tStorage, eRenderType renderType, 
			DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds);

		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM);

		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m) final;
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)    final;
	
	};
}


