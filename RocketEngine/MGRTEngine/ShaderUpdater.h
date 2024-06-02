///���߿� ���̴� ���� ��ĥ �� Ȱ���� �� �ִ�. 
///��.��. ����� ��Ŭ����ؼ��� �ȵȴ�. �������� �ʼ�!

#pragma once
#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <variant>
#include <DirectXMath.h>
#include <d3d11.h>
#include "ByteBuffer.hpp"
#include <cassert>
#include "MathHelper.h"

//��ũ ���� ����.
#include "BaseRenderable.h"
#include "TypedefGraphicHelper.h"
#include "Texture.h"
#include "MeshObject.h"
#include "SkinnedMeshObject.h"
#include "CameraMatrices.h"
#include "Shader.h"
#include "ShaderMaterial.h"

//�ܺ� ������Ʈ���� �������� ���.
#include "../RocketGraphicsInterface/MaterialData.h"
#include "../RocketGraphicsInterface/LightData.h"
#include "../RocketMath/RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG


namespace RocketCore::Graphics
{
	//���ø� Ư��ȭ�� ���� ���õ� ����.

	///Forward:
	class BasicTexLegacy {};		// BasicTex.cso.
	class SkinnedMeshTexLegacy {};	// SkinnedMeshTex.cso.
	class PosNormalTexLegacy {}; // PosNormalTex.cso. (ù��° ������ ����) 
	
	///Deferred:
	class DeferredPhongLegacy {};
	class DeferredPBRType {};
	class DeferredSkinnedPBRType {};

}

namespace RocketCore::Graphics
{
	class ShaderUpdater
	{
		//�ε����� �Ź� �ް� �ٲ��.
		using MemoryVariant =
			std::variant<DirectX::XMMATRIX* /* 0 */, ByteBuffer*  /* 1 */,
			ID3D11ShaderResourceView* /* 2 */, float* /* 3 */ , 
			LegacyMaterialData*/* 4 */, int* /* 5 */,
			DirectionalLightData* /* 6 */, PBRMaterialData* /* 7 */>;

		//std::get�� ��Ÿ�ӿ� ������ �����δ� �ߵ��� �� ����. 
		//MemoryVariant�� ���̾ƿ��� ����, enum���� ���س���! (���� ���� �߿�)
		enum
		{
			XMMATRIX_POINTER = 0,
			BYTEBUFFER_POINTER = 1,
			SRV_POINTER = 2,
			FLOAT_POINTER = 3,
			CUSTOM_LEGACY_MATERIAL_POINTER = 4, // �̷� ������ ��� Ȯ���� ������ �ȴ�.
			INT_POINTER = 5,
			CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER = 6,
			CUSTOM_PBR_MATERIAL_POINTER = 7
		};

	public:
		//				Variable			Variant Index
		std::vector<std::pair<MemoryVariant, size_t>> _varVariantList;

		template <typename T>
		void Initialize(BaseRenderable* baseRenderable); ////������Ʈ�� ������� ��, �ϰ������� �Ҽ� ������Ʈ�� �ּҴ� Ȱ���� ���� ���޵Ǿ�� �Ѵ�.

		//��� Object�� ���Ǳ� ����, SkinnedMeshObject ������ Interval���� ���Խ��״�.
		template <typename T>
		void UpdateVariables(const DirectX::XMFLOAT4X4& worldTM, double interval);

	private:
		BaseRenderable* _assignedObject = nullptr;
	};

	template <typename T>
	void ShaderUpdater::Initialize(BaseRenderable* baseRenderable) //������ ���� ������ ���� �־��!
	{
		//��� �Լ����� Ư��ȭ�� ���¿��� ȣ��Ǿ�� �Ѵ�.
		//�⺻ ���·� ������ �ȵȴ�.
		assert(false);
	}

	template <typename T>
	void ShaderUpdater::UpdateVariables(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		//��� �Լ����� Ư��ȭ�� ���¿��� ȣ��Ǿ�� �Ѵ�.
		//�⺻ ���·� ������ �ȵȴ�.
		assert(false);
	}

	///���⼭ Ư��ȭ�� �Լ����� ����/���� �и������. (H: ����. CPP : ����)
	
	///Forward
	//BasicTex.cso
	template<>
	void ShaderUpdater::Initialize<BasicTexLegacy>(BaseRenderable* baseRenderable);

	template<>
	void ShaderUpdater::UpdateVariables<BasicTexLegacy>(const DirectX::XMFLOAT4X4& worldTM, double interval);

	//SkinnedMeshTex.cso
	template<>
	void ShaderUpdater::Initialize<SkinnedMeshTexLegacy>(BaseRenderable* baseRenderable);

	template<>
	void ShaderUpdater::UpdateVariables<SkinnedMeshTexLegacy>(const DirectX::XMFLOAT4X4& worldTM, double interval);

	//PosNormalTex.cso
	template<>
	void ShaderUpdater::Initialize<PosNormalTexLegacy>(BaseRenderable* baseRenderable);

	template<>
	void ShaderUpdater::UpdateVariables<PosNormalTexLegacy>(const DirectX::XMFLOAT4X4& worldTM, double interval);

	///Deferred
	//DeferredPhongShader.cso
	template<>
	void ShaderUpdater::Initialize<DeferredPhongLegacy>(BaseRenderable* baseRenderable);

	template<>
	void ShaderUpdater::UpdateVariables<DeferredPhongLegacy>(const DirectX::XMFLOAT4X4& worldTM, double interval);
	
	//BasicPBR.cso
	template<>
	void ShaderUpdater::Initialize<DeferredPBRType>(BaseRenderable* baseRenderable);

	template<>
	void ShaderUpdater::UpdateVariables<DeferredPBRType>(const DirectX::XMFLOAT4X4& worldTM, double interval);

	//BasicPBRSkinned.cso
	template<>
	void ShaderUpdater::Initialize<DeferredSkinnedPBRType>(BaseRenderable* baseRenderable);

	template<>
	void ShaderUpdater::UpdateVariables<DeferredSkinnedPBRType>(const DirectX::XMFLOAT4X4& worldTM, double interval);
}







