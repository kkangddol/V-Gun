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

	//RocketEngine���� ���޹��� ���� => �ߵ��� �� ���̴�!
	class SkinnedMeshObject : public BaseRenderable
	{
		friend class ResourceManager;
		friend class ShaderUpdater;
		friend class RenderPipeline;
	public:
		//nullptr���,�ڵ����� �Ҵ�� ��. ��� �ϴ� �־��ִ� ������� ����.
		SkinnedMeshObject(ShaderMaterial* _mat, BaseFormation* _form, 
			CameraMatrices* cameraMat, TextureStorage* tStorage, eRenderType renderType,
			DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds);
		virtual ~SkinnedMeshObject();
		//RocketEngine�� �� Override.
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM);
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m) final;
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)    final;

		virtual void PlayAnim(AnimData animData) final;

	private:
		//���ӿ������� ����� ������ �ش�Ǵ� Animation�� ���� ó���ϴ� ��ü.
		std::unique_ptr<AnimationHandler> m_AnimHandler;

	};
}


