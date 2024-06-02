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
		//���̴� �������� ���� ���� �Ҵ�.
		this->_shaderUpdater = new ShaderUpdater;

		//Legacy Material �⺻ Property ����.
		_legacyMaterialData._ambient.x = 0.7f;
		_legacyMaterialData._ambient.y = 0.7f;
		_legacyMaterialData._ambient.z = 0.7f;
		_legacyMaterialData._ambient.w = 0.7f;

		_legacyMaterialData._diffuse.x = 1.0f; //�갡 1.0f�̴�, ������ ������.
		_legacyMaterialData._diffuse.y = 1.0f;
		_legacyMaterialData._diffuse.z = 1.0f;
		_legacyMaterialData._diffuse.w = 1.0f;

		_legacyMaterialData._specular.w = 1.0f;
		_legacyMaterialData._specular.x = 1.0f;
		_legacyMaterialData._specular.y = 1.0f; ///���� �̼� : ����Ʈ �ϵ��ڵ��ؼ� �۵����Ѻ���.
		_legacyMaterialData._specular.z = 1.0f;

		_legacyMaterialData._reflect.x = 0.0f;
		_legacyMaterialData._reflect.y = 0.0f;
		_legacyMaterialData._reflect.z = 0.0f;
		_legacyMaterialData._reflect.w = 0.0f;

		//PBR Material Property �⺻ ����.
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
		//IRenderable�� ����.
	}

	void BaseRenderable::Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m)
	{
		////IRenderable�� ����.
	}

	void BaseRenderable::Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)
	{
		////IRenderable�� ����.
	}

	void BaseRenderable::PlayAnim(AnimData animData)
	{
		//IRenderable�� ����.
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

	///������ ��ŸƮ / ������Ʈ �߿� ȣ��Ǵ� �Լ���.
	void BaseRenderable::InitializeShaderVariables(BaseRenderable* renderable)
	{
		//�̸� ������ �Լ� ȣ��.
		_shaderInitFunction(renderable);
	}

	void BaseRenderable::UpdateShaderVariables(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		//�̸� ������ �Լ� ȣ��.
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
				///���߿� ȣ���� Shader Initializer Function ����.
				//���Ⱑ ���߿� ������Ʈ ���� �� �� �ֵ��� ������ �ٲ��� �ϴµ�.. �̹� ������Ʈ�� �ް�, Assign�� �� �ְ� �� �� �־�� �Ѵ�.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<BasicTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<BasicTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
			else if (tShaderName == "SkinnedMeshTex.cso")
			{
				///Shader System Change
				///���߿� ȣ���� Shader Initializer Function ����.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<SkinnedMeshTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<SkinnedMeshTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
			else if (tShaderName == "PosNormalTex.cso")
			{
				///Shader System Change
				///���߿� ȣ���� Shader Initializer Function ����.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<PosNormalTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<PosNormalTexLegacy>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
		}
		else if (renderType == eRenderType::DEFERRED_PHONG)
		{
			///���⼭ ���۵� Bind�� �����ؾ� ��.
			///�ϴ� ����������, ���� �������� ���� ���� ���� ��Ȳ.. Deferred�� �ֵ��� �ϳ��� ��������. 
			//�ֳ�? => Render�� ȣ��� ��, �Ѳ����� �������� �׸���, �̿� ���� �������� �ϰ������� ó���ϱ� ������, �ϳ� �̻��̸� �� ������ ������ �־�� �Ѵ�.

			if (tShaderName == "DeferredPhongShader.cso")
			{
				//Deferred Shader�� �ϴ� �ϳ��� �ΰ� ����.
				//�̷��� �ȴٸ�, EndDeferred �Լ����� �ϰ������� �Լ��� ȣ���ϴ°� �´�! (Basic�Լ� ȣ���ϴ� ��)
				///Shader System Change
				///���߿� ȣ���� Shader Initializer Function ����.
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
				//Deferred Shader�� �ϴ� �ϳ��� �ΰ� ����.
				//�̷��� �ȴٸ�, EndDeferred �Լ����� �ϰ������� �Լ��� ȣ���ϴ°� �´�! (Basic�Լ� ȣ���ϴ� ��)
				///Shader System Change
				///���߿� ȣ���� Shader Initializer Function ����.
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
				//Deferred Shader�� �ϴ� �ϳ��� �ΰ� ����.
				//�̷��� �ȴٸ�, EndDeferred �Լ����� �ϰ������� �Լ��� ȣ���ϴ°� �´�! (Basic�Լ� ȣ���ϴ� ��)
				///Shader System Change
				///���߿� ȣ���� Shader Initializer Function ����.
				auto tInitFunc = std::bind(&ShaderUpdater::Initialize<DeferredSkinnedPBRType>, this->GetShaderUpdater(), std::placeholders::_1);
				this->SetShaderInitFunction(tInitFunc);

				auto tUpdateFunc = std::bind(&ShaderUpdater::UpdateVariables<DeferredSkinnedPBRType>, this->GetShaderUpdater(), std::placeholders::_1, std::placeholders::_2);
				this->SetShaderUpdateFunction(tUpdateFunc);
				///
			}
			else //���߿� ���⼭ PBR�� ����, �׸�ŭ Begin-End�� �þ������, Future Business!
			{
				assert(false);
			}
		}
		else
		{
			assert(false);
		}
		
	
		//����, ���� �Լ��� �����Ͽ� Initialize���ִ� �ڵ� ���� �����Ѵ�.
		///Rendering System ������Ʈ (���̴�)�� ���� ���ο� ȣ��.
		// ���⼭ ShaderMaterial�� ��θ� �ο��� ��.. �׷��� �������� ������ �������.
		//���̴� ���͸��� ��ü ��ü�� ������ ��ϵǱ⿡, ������ ���� ��ϵǾ��� ��.
		//�̰� ���߿� ����صΰ� ����.
		this->InitializeShaderVariables(this);
	}

	void BaseRenderable::GetOutMinMaxBounds(DirectX::XMFLOAT3& minF, DirectX::XMFLOAT3& maxF)
	{
		minF = this->_minBoundingBox;
		maxF = this->_maxBoundingBox;
	}
	///
}

