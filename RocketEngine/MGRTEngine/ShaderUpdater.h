///나중에 셰이더 구조 고칠 때 활용할 수 있다. 
///절.대. 헤더에 인클루드해서는 안된다. 전방참조 필수!

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

//링크 에러 주의.
#include "BaseRenderable.h"
#include "TypedefGraphicHelper.h"
#include "Texture.h"
#include "MeshObject.h"
#include "SkinnedMeshObject.h"
#include "CameraMatrices.h"
#include "Shader.h"
#include "ShaderMaterial.h"

//외부 프로젝트에서 가져오는 경로.
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
	//템플릿 특수화를 위해 마련된 공간.

	///Forward:
	class BasicTexLegacy {};		// BasicTex.cso.
	class SkinnedMeshTexLegacy {};	// SkinnedMeshTex.cso.
	class PosNormalTexLegacy {}; // PosNormalTex.cso. (첫번째 라이팅 적용) 
	
	///Deferred:
	class DeferredPhongLegacy {};
	class DeferredPBRType {};
	class DeferredSkinnedPBRType {};

}

namespace RocketCore::Graphics
{
	class ShaderUpdater
	{
		//인덱스는 매번 받게 바뀐다.
		using MemoryVariant =
			std::variant<DirectX::XMMATRIX* /* 0 */, ByteBuffer*  /* 1 */,
			ID3D11ShaderResourceView* /* 2 */, float* /* 3 */ , 
			LegacyMaterialData*/* 4 */, int* /* 5 */,
			DirectionalLightData* /* 6 */, PBRMaterialData* /* 7 */>;

		//std::get이 런타임에 정해진 값으로는 발동될 수 없다. 
		//MemoryVariant의 레이아웃에 따라서, enum들을 정해놓자! (연동 몹시 중요)
		enum
		{
			XMMATRIX_POINTER = 0,
			BYTEBUFFER_POINTER = 1,
			SRV_POINTER = 2,
			FLOAT_POINTER = 3,
			CUSTOM_LEGACY_MATERIAL_POINTER = 4, // 이런 식으로 계속 확장해 나가면 된다.
			INT_POINTER = 5,
			CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER = 6,
			CUSTOM_PBR_MATERIAL_POINTER = 7
		};

	public:
		//				Variable			Variant Index
		std::vector<std::pair<MemoryVariant, size_t>> _varVariantList;

		template <typename T>
		void Initialize(BaseRenderable* baseRenderable); ////오브젝트가 만들어질 때, 일괄적으로 소속 오브젝트의 주소는 활용을 위해 전달되어야 한다.

		//모든 Object에 통용되기 위해, SkinnedMeshObject 전용인 Interval까지 포함시켰다.
		template <typename T>
		void UpdateVariables(const DirectX::XMFLOAT4X4& worldTM, double interval);

	private:
		BaseRenderable* _assignedObject = nullptr;
	};

	template <typename T>
	void ShaderUpdater::Initialize(BaseRenderable* baseRenderable) //무조건 들어올 때부터 값이 있어야!
	{
		//모든 함수들은 특수화된 상태에서 호출되어야 한다.
		//기본 상태로 들어오면 안된다.
		assert(false);
	}

	template <typename T>
	void ShaderUpdater::UpdateVariables(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		//모든 함수들은 특수화된 상태에서 호출되어야 한다.
		//기본 상태로 들어오면 안된다.
		assert(false);
	}

	///여기서 특수화된 함수들을 선언/정의 분리해줘야. (H: 선언. CPP : 정의)
	
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







