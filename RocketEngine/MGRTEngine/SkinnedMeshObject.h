#pragma once
#include "BaseRenderable.h"

#include <iostream>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <array>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace RocketCore::Graphics
{
	struct Transform;
	class SkinnedMeshFormation;
	class MeshFormation;
	class BaseFormation;
	class Texture;
	class ShaderMaterial;
	class CameraMatrices;
	class ByteBuffer;
	class RenderUsageNode;
	class RenderUsageMesh;
	class AnimationHandler;

	//RocketEngine에서 전달받은 정보 => 발동이 될 것이다!
	class SkinnedMeshObject : public BaseRenderable
	{
		friend class ResourceManager;
		friend class ShaderUpdater;
		friend class RenderPipeline;
	public:
		//nullptr라면,자동으로 할당될 것. 모두 일단 넣어주는 방식으로 가자.
		SkinnedMeshObject(ShaderMaterial* _mat, BaseFormation* _form, 
			CameraMatrices* cameraMat, TextureStorage* tStorage, eRenderType renderType,
			DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds);
		virtual ~SkinnedMeshObject();
		//RocketEngine의 값 Override.
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM);
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m) final;
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)    final;

		virtual void PlayAnim(AnimData animData) final;

	private:
		//게임엔진에서 명령이 들어오면 해당되는 Animation을 따로 처리하는 주체.
		std::unique_ptr<AnimationHandler> m_AnimHandler;

	};
}


