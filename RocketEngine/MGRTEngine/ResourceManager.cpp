#include "ResourceManager.h"
#include "AssetDataDefine.h"
#include "AssetDataHelper.h"
#include "AssetModelData.h"
#include "Shader.h"
#include "ShaderMaterial.h"
#include <filesystem>
#include <algorithm>
#include <cassert>
#include "d3dx11effect.h"
#include <d3d11.h>
#include <vector>
#include "VertexStructHelper.h"
#include "ByteBuffer.hpp"
#include "SkinnedMeshFormation.h"
#include "Texture.h"
#include "LowLevelDX11.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#include "RenderUsageMesh.h"

#include "RenderAnimation.h"
#include "RenderCubeMap.h"
#include "CameraMatrices.h"

#include "CustomFXInstances.h"

#include "../RocketGraphicsInterface/CameraData.h"
#include "../RocketGraphicsInterface/LightData.h"
#include "../RocketGraphicsInterface/ModelData.h"
#include "../RocketGraphicsInterface/RenderConstantData.h"

namespace RocketCore::Graphics
{
	unsigned int ResourceManager::recordModelHandleID = 0;


	void ResourceManager::Initialize()
	{
		//Default Pixel SRV Setting (Debug2DBox)
		this->_defaultPixelSRV = MakeTextureFromFilePath("../GraphicsResources/DefaultTextures/Pixel1.png");
		assert(this->_defaultPixelSRV != nullptr);

		//Default Diffuse SRV Setting.
		this->_defaultDiffuseSRV = MakeTextureFromFilePath("../GraphicsResources/DefaultTextures/DefaultDiffuseMap.dds");
		assert(this->_defaultDiffuseSRV != nullptr);

		//Default Normal SRV Setting.
		this->_defaultNormalSRV = MakeTextureFromFilePath("../GraphicsResources/DefaultTextures/DefaultNormalMap.dds");
		assert(this->_defaultNormalSRV != nullptr);

		//Default Arm SRV Setting. (기본 텍스쳐 자체는 Diffuse랑 같이 하얀색 텍스쳐를 받아올 것이다)
		this->_defaultArmSRV = MakeTextureFromFilePath("../GraphicsResources/DefaultTextures/DefaultDiffuseMap.dds");
		assert(this->_defaultArmSRV != nullptr);
	}

	RocketCore::Graphics::AssetModelData* ResourceManager::FindModelByHandle(unsigned int handleID)
	{
		auto res = modelDataMap.find(handleID);
		if (res == modelDataMap.end())
		{
			return nullptr;
		}
		else
		{
			return res->second;
		}
	}

	RocketCore::Graphics::AssetModelData* ResourceManager::FindModelByFileName(const std::string& fileName)
	{
		auto res = std::find_if(modelDataMap.begin(), modelDataMap.end(),
			[&fileName](const std::pair<unsigned int, AssetModelData*>& val)
			-> bool {return val.second->_fileName == fileName; });

		if (res == modelDataMap.end())
		{
			return nullptr;
		}
		else
		{
			return res->second;
		}
	}

	AssetModelData* ResourceManager::AppendGetModelData()
	{
		//ModelData 생성, m_ModelDataMap에 집어넣는 역할이다.
		AssetModelData* tModelData = new AssetModelData(recordModelHandleID);
		modelDataMap.insert(std::make_pair(recordModelHandleID, tModelData));

		//Handle이 다음에는 하나씩 증가할 것.
		recordModelHandleID++;
		return tModelData;
	}

	RocketCore::Graphics::RenderAnimation* ResourceManager::AppendGetAnimationData(SkinnedMeshFormation* form, AnimDesc animDesc)
	{
		RenderAnimation* tRenderAnim = new RenderAnimation(form, animDesc.name, animDesc.animPath, animDesc.ownerModelPath);
		std::string tAnimName = animDesc.name;
		renderAnimationMap.insert(std::make_pair(tAnimName, tRenderAnim));
		return tRenderAnim;
	}

	void ResourceManager::Release()
	{
		//Resources에 있는거 싹 다 Release.
		for (auto& it : modelDataMap)
		{
			if (it.second != nullptr)
			{
				delete (it.second);
			}
		}
	}

	bool ResourceManager::FoundExistingFXFile(const std::string& fxName)
	{
		//[TW] 긴급
		auto res = std::find_if(fxShaderMap.begin(), fxShaderMap.end(),
			[&fxName](const std::pair<std::string, ShaderInputSet>& val)
			-> bool {return val.first == fxName; });

		bool isFound = (res != fxShaderMap.end()) ? true : false;
		return isFound;
	}

	//구조 상의 문제로 어쩔 수 없다! 
	void ResourceManager::AddShaderDataWithResource(const std::string& filePath, eRenderType renderType)
	{
		///여기에서 InputVariable, Constant Buffer 등을 기록/체크하는, 엄청난 IF/ELSE 문이 돌 예정.
		///또한, 이제 ShaderName을 기반으로 밑과 같이 If문을 돌면서 정보를 골라서 initialize하는 코드가 돌것이니, 주의.
		/// (BaseRenderable의 InitializeShaderFunctions.)
		std::filesystem::path tPath(filePath);
		std::string tShaderName = tPath.filename().string();

		std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> it;

		if (renderType == eRenderType::FORWARD)
		{
			if (tShaderName == "BasicTex.cso")
			{
				it = CustomFXInstances::MakeShaderInput_BasicTexFX(filePath, tShaderName);
			}
			else if (tShaderName == "SkinnedMeshTex.cso")
			{
				it = CustomFXInstances::MakeShaderInput_SkinnedMeshTex(filePath, tShaderName);
			}
			else if (tShaderName == "PosNormalTex.cso")
			{
				it = CustomFXInstances::MakeShaderInput_PosNormalTex(filePath, tShaderName);
			}
			else
			{
				assert(false);
			}
		}
		else if (renderType == eRenderType::DEFERRED_PHONG)
		{
			if (tShaderName == "DeferredPhongShader.cso")
			{
				it = CustomFXInstances::MakeShaderInput_DeferredPhongShader(filePath, tShaderName);
			}
			else if (tShaderName == "FinalCombination.cso")
			{
				//최종적으로 Quad랑 합하기!
				it = CustomFXInstances::MakeShaderInput_FinalCombinationShader(filePath, tShaderName);
			}
			else
			{
				assert(false);
			}
		}
		else if (renderType == eRenderType::DEFERRED_PBR)
		{
			if (tShaderName == "BasicPBR.cso")
			{
				it = CustomFXInstances::MakeShaderInput_DeferredPBRShader(filePath, tShaderName);
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
				it = CustomFXInstances::MakeShaderInput_DeferredSkinnedPBRShader(filePath, tShaderName);
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			assert(false);
		}

		//다 만들었으면 넣는다.
		this->fxShaderMap.insert(std::make_pair(tShaderName, it));
	}

	RocketCore::Graphics::ShaderInputSet ResourceManager::FindShaderInputSetByName(const std::string& fxName)
	{
		//[TW] 긴급
		auto res = std::find_if(fxShaderMap.begin(), fxShaderMap.end(),
			[&fxName](const std::pair<std::string, ShaderInputSet>& val)
			-> bool {return val.first == fxName; });

		if (res == fxShaderMap.end())
		{
			//이 시점에서 못 찾으면 안된다. 로직상.
			assert(false);
		}

		return res->second;
	}

	Texture* ResourceManager::MakeTextureFromFilePath(const std::string& filePath)
	{
		std::filesystem::path tPath(filePath);
		std::string ext = tPath.extension().string();

		std::wstring wFilePath = std::wstring().assign(filePath.begin(), filePath.end());

		Texture* tTexture = new Texture;

		if (ext == ".dds")
		{
			ID3D11Resource* res = tTexture->AssignSRVFromDDSPath(wFilePath);
			if (res == nullptr)
			{
				std::wstring tBasicError = L"MakeTextureFromFilePath(DDS) 중 오류 - 인식 시도 Path : ";
				tBasicError.append(wFilePath);
				MessageBox(LowLevelDX11::GetInstance()->m_hWnd, tBasicError.c_str(), L"오류...", MB_ICONERROR | MB_OK);
			}
			tTexture->m_FilePath = wFilePath;
		}
		else
		{
			ID3D11Resource* res = tTexture->AssignSRVFromWICPath(wFilePath);
			if (res == nullptr)
			{
				std::wstring tBasicError = L"MakeTextureFromFilePath(WIC) 중 오류 - 인식 시도 Path : ";
				tBasicError.append(wFilePath);
				MessageBox(LowLevelDX11::GetInstance()->m_hWnd, tBasicError.c_str(), L"오류...", MB_ICONERROR | MB_OK);
			}
			tTexture->m_FilePath = wFilePath;
		}

		//일단 무조건 저장한 Texture : Unordered Map에 저장. (이미 있으면 필터링됨)
		StoreTextureDataWithFilePath(std::string(filePath), tTexture);

		assert(tTexture != nullptr);
		return tTexture;
	}

	RocketCore::Graphics::RenderAnimation* ResourceManager::FindAnimationByName(const std::string& name)
	{
		auto res = std::find_if(renderAnimationMap.begin(), renderAnimationMap.end(),
			[&name](const std::pair<std::string, RenderAnimation*>& val)
			-> bool {return val.second->_animName == name; });

		if (res == renderAnimationMap.end())
		{
			return nullptr;
		}
		else
		{
			return res->second;
		}
	}

	void ResourceManager::InsertNameFormation(const std::string& _modelPath, BaseFormation* _form)
	{
		this->modelFormationMap.insert(std::make_pair(_modelPath, _form));
	}

	RocketCore::Graphics::BaseFormation* ResourceManager::FindFormationByModelPath(const std::string& modelPath)
	{
		auto res = std::find_if(modelFormationMap.begin(), modelFormationMap.end(),
			[modelPath](const std::pair<std::string, BaseFormation*>& val)
			-> bool {return val.first == modelPath; });

		if (res == modelFormationMap.end())
		{
			return nullptr;
		}
		else
		{
			return res->second;
		}
	}

	void ResourceManager::StoreTextureDataWithFilePath(const std::string filePath, Texture* texture)
	{
		_textureImageMap[filePath] = texture;
	}

	RocketCore::Graphics::Texture* ResourceManager::FindTextureByFilePath(const std::string& filePath)
	{
		auto res = std::find_if(_textureImageMap.begin(), _textureImageMap.end(),
			[filePath](const std::pair<std::string, Texture*>& val)
			-> bool {return val.first == filePath; });

		if (res == _textureImageMap.end())
		{
			return nullptr;
		}
		else
		{
			return res->second;
		}
	}

	RocketCore::Graphics::RenderCubeMap* ResourceManager::MakeCubeMapFromFilePath(const std::string& filePath, CameraMatrices* camMatrix)
	{
		std::filesystem::path tPath(filePath);
		std::string ext = tPath.extension().string();

		std::wstring wFilePath = std::wstring().assign(filePath.begin(), filePath.end());

		Texture* tTexture = FindTextureByFilePath(filePath);

		if (tTexture == nullptr)
		{
			tTexture = MakeTextureFromFilePath(filePath);
		}
		assert(tTexture != nullptr);
		RenderCubeMap* tCubeMap = new RenderCubeMap(tTexture, camMatrix);

		//일단 무조건 저장한 Texture : Unordered Map에 저장. (이미 있으면 필터링됨)
		StoreCubeMapWithFilePath(std::string(filePath), tCubeMap);

		assert(tCubeMap != nullptr);
		return tCubeMap;
	}

	void ResourceManager::StoreCubeMapWithFilePath(const std::string filePath, RenderCubeMap* cubeMap)
	{
		_renderCubeMapMap[filePath] = cubeMap;
	}

	RocketCore::Graphics::RenderCubeMap* ResourceManager::FindCubeMapByFilePath(const std::string& filePath)
	{
		auto res = std::find_if(_renderCubeMapMap.begin(), _renderCubeMapMap.end(),
			[filePath](const std::pair<std::string, RenderCubeMap*>& val)
			-> bool {return val.first == filePath; });

		if (res == _renderCubeMapMap.end())
		{
			return nullptr;
		}
		else
		{
			return res->second;
		}
	}

	RocketCore::Graphics::Texture* ResourceManager::GetDefaultDiffuseTexture()
	{
		return _defaultDiffuseSRV;
	}

	RocketCore::Graphics::Texture* ResourceManager::GetDefaultNormalTexture()
	{
		return _defaultNormalSRV;
	}

	RocketCore::Graphics::Texture* ResourceManager::GetDefaultArmTexture()
	{
		return _defaultArmSRV;
	}

	RocketCore::Graphics::Texture* ResourceManager::GetDefaultPixelTexture()
	{
		return _defaultPixelSRV;
	}

	void ResourceManager::InsertNameRenderable(const std::string& objectname, BaseRenderable* ren)
	{
		_nameObjectMap.insert(std::make_pair(objectname, ren));
	}

	RocketCore::Graphics::BaseRenderable* ResourceManager::FindRenderableByObjectName(const std::string& objectName)
	{
		auto res = std::find_if(_nameObjectMap.begin(), _nameObjectMap.end(),
			[objectName](const std::pair<std::string, BaseRenderable*>& val)
			-> bool {return val.first == objectName; });

		if (res == _nameObjectMap.end())
		{
			return nullptr;
		}
		else
		{
			return res->second;
		}
	}



}
