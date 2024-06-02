#include "BaseRenderable.h"
#include "BaseFormation.h"
#include "ShaderMaterial.h"
#include "CameraMatrices.h"
#include "LowLevelDX11.h"
#include "SceneConstantsUpdater.h"
#include <d3d11.h>
#include "ResourceManager.h"
#include "TypedefGraphicHelper.h"
#include "ShaderUpdater.h"
#include <filesystem>


namespace RocketCore::Graphics
{
	BaseRenderable::BaseRenderable(ShaderMaterial* mat, BaseFormation* form, 
		CameraMatrices* cameraMat, TextureStorage* tStorage, eRenderType renderType,
		DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds) :
		_material(mat), _formation(form), _cameraMatrix(cameraMat),
		_dev(LowLevelDX11::GetInstance()->m_d3dDevice.Get()), _devcon(LowLevelDX11::GetInstance()->m_d3dMainContext.Get()),
		_textureStorage(tStorage), _sceneConstantsUpdater(SceneConstantsUpdater::GetInstance()), _renderingType(renderType),
		_minBoundingBox(minBounds), _maxBoundingBox(maxBounds)
	{
		//셰이더 업데이터 새로 동적 할당.
		this->_shaderUpdater = new ShaderUpdater;

		//Legacy Material 기본 Property 설정.
		_legacyMaterialData._ambient.x = 0.7f;
		_legacyMaterialData._ambient.y = 0.7f;
		_legacyMaterialData._ambient.z = 0.7f;
		_legacyMaterialData._ambient.w = 0.7f;

		_legacyMaterialData._diffuse.x = 1.0f; //얘가 1.0f이니, 투명도가 조정됨.
		_legacyMaterialData._diffuse.y = 1.0f;
		_legacyMaterialData._diffuse.z = 1.0f;
		_legacyMaterialData._diffuse.w = 1.0f;

		_legacyMaterialData._specular.w = 1.0f;
		_legacyMaterialData._specular.x = 1.0f;
		_legacyMaterialData._specular.y = 1.0f; ///다음 미션 : 라이트 하드코딩해서 작동시켜보기.
		_legacyMaterialData._specular.z = 1.0f;

		_legacyMaterialData._reflect.x = 0.0f;
		_legacyMaterialData._reflect.y = 0.0f;
		_legacyMaterialData._reflect.z = 0.0f;
		_legacyMaterialData._reflect.w = 0.0f;

		//PBR Material Property 기본 설정.
		_pbrMaterialData._albedo.x = 1.0f;
		_pbrMaterialData._albedo.y = 1.0f;
		_pbrMaterialData._albedo.z = 1.0f;
		_pbrMaterialData._albedo.w = 1.0f;
		
		_pbrMaterialData._metallic = 1.0f;
		_pbrMaterialData._roughness = 1.0f;
		_pbrMaterialData._specular =1.0f;
		_pbrMaterialData._emission =1.0f;
	}

	BaseRenderable::~BaseRenderable()
	{
		if (_shaderUpdater != nullptr)
		{
			delete _shaderUpdater;
		}
	}


	void BaseRenderable::Render(const RocketEngine::RMFLOAT4X4& worldTM)
{
		//IRenderable에 의해.
	}

	void BaseRenderable::Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m)
	{
		////IRenderable에 의해.
	}

	void BaseRenderable::Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)
	{
		////IRenderable에 의해.
	}

	void BaseRenderable::PlayAnim(AnimData animData)
	{
		//IRenderable에 의해.
	}

	RocketCore::Graphics::TextureStorage* BaseRenderable::GetTextureStorage()
	{
		return _textureStorage;
	}

	RocketCore::Graphics::ShaderMaterial* BaseRenderable::GetShaderMaterial()
	{
		return this->_material;
	}

	RocketCore::Graphics::CameraMatrices* BaseRenderable::GetCameraMatrices()
	{
		return this->_cameraMatrix;
	}

	RocketCore::Graphics::BaseFormation* BaseRenderable::GetBaseFormation()
	{
		return this->_formation;
	}

	RocketCore::Graphics::LegacyMaterialData BaseRenderable::GetLegacyMaterialData()
	{
		return this->_legacyMaterialData;
	}

	RocketCore::Graphics::PBRMaterialData BaseRenderable::GetPBRMaterialData()
	{
		return this->_pbrMaterialData;
	}

	RocketCore::Graphics::ShaderUpdater* BaseRenderable::GetShaderUpdater()
	{
		return _shaderUpdater;
	}

	void BaseRenderable::SetShaderInitFunction(std::function<void(BaseRenderable*)> param)
	{
		this->_shaderInitFunction = param;
	}

	void BaseRenderable::SetShaderUpdateFunction(std::function<void(const DirectX::XMFLOAT4X4&, double)> param)
	{
		this->_shaderUpdateFunction = param;
	}

	///렌더링 스타트 / 업데이트 중에 호출되는 함수들.
	void BaseRenderable::InitializeShaderVariables(BaseRenderable* renderable)
	{
		//미리 배정된 함수 호출.
		_shaderInitFunction(renderable);
	}

	void BaseRenderable::UpdateShaderVariables(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		//미리 배정된 함수 호출.
		_shaderUpdateFunction(worldTM, interval);
	}

	void BaseRenderable::InitializeShaderFunctions(std::string filepath, eRenderType renderType)
	{
		std::filesystem::path tPath(filepath);
		std::string tShaderName = tPath.filename().string();

		if (renderType == eRenderType::FORWARD)
		{
			if (tShaderName == "BasicTex.cso")
			{
				///Shader System Change
				///나중에 호출할 Shader Initializer Function 세팅.
				//여기가 나중에 오브젝트 딴에 들어갈 수 있도록 설정이 바뀌어야 하는데.. 이미 오브젝트를 받고, Assign할 수 있게 할 수 있어야 한다.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<BasicTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<BasicTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
			else if (tShaderName == "SkinnedMeshTex.cso")
			{
				///Shader System Change
				///나중에 호출할 Shader Initializer Function 세팅.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<SkinnedMeshTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<SkinnedMeshTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
			else if (tShaderName == "PosNormalTex.cso")
			{
				///Shader System Change
				///나중에 호출할 Shader Initializer Function 세팅.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<PosNormalTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<PosNormalTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
		}
		else if (renderType == eRenderType::DEFERRED_PHONG)
		{
			///여기서 디퍼드 Bind를 진행해야 함.
			///일단 구조적으로, 현재 여러개를 곱할 수가 없는 상황.. Deferred인 애들은 하나로 한정하자. 
			//왜냐? => Render가 호출될 때, 한꺼번에 여러개를 그리고, 이에 대한 라이팅을 일괄적으로 처리하기 때문에, 하나 이상이면 그 별도의 구조가 있어야 한다.

			if (tShaderName == "DeferredPhongShader.cso")
			{
				//Deferred Shader는 일단 하나를 두고 진행.
				//이렇게 된다면, EndDeferred 함수에서 일괄적으로 함수를 호출하는게 맞다! (Basic함수 호출하는 것)
				///Shader System Change
				///나중에 호출할 Shader Initializer Function 세팅.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<DeferredPhongLegacy>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<DeferredPhongLegacy>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
		}
		else if (renderType == eRenderType::DEFERRED_PBR)
		{
			if (tShaderName == "BasicPBR.cso")
			{
				//Deferred Shader는 일단 하나를 두고 진행.
				//이렇게 된다면, EndDeferred 함수에서 일괄적으로 함수를 호출하는게 맞다! (Basic함수 호출하는 것)
				///Shader System Change
				///나중에 호출할 Shader Initializer Function 세팅.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<DeferredPBRType>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<DeferredPBRType>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
			else
			{
				assert(false);
			}
		}
		else if (renderType == eRenderType::DEFERRED_PBR_SKINNED)
		{
			if (tShaderName == "BasicPBRSkinned.cso")
			{
				//Deferred Shader는 일단 하나를 두고 진행.
				//이렇게 된다면, EndDeferred 함수에서 일괄적으로 함수를 호출하는게 맞다! (Basic함수 호출하는 것)
				///Shader System Change
				///나중에 호출할 Shader Initializer Function 세팅.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<DeferredSkinnedPBRType>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<DeferredSkinnedPBRType>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
			else //나중에 여기서 PBR을 쓰면, 그만큼 Begin-End가 늘어나겠지만, Future Business!
			{
				assert(false);
			}
		}
		else
		{
			assert(false);
		}
		
	
		//이제, 실제 함수를 실행하여 Initialize해주는 코드 역시 실행한다.
		///Rendering System 업데이트 (셰이더)에 따라서 새로운 호출.
		// 여기서 ShaderMaterial에 경로를 부여한 것.. 그래서 같아지는 문제가 생겼었다.
		//셰이더 매터리얼 객체 자체가 있으면 기록되기에, 마지막 것이 기록되었던 것.
		//이걸 나중에 기록해두고 쓰자.
		this->InitializeShaderVariables(this);
	}

	void BaseRenderable::GetOutMinMaxBounds(DirectX::XMFLOAT3& minF, DirectX::XMFLOAT3& maxF)
	{
		minF = this->_minBoundingBox;
		maxF = this->_maxBoundingBox;
	}
	///
}

