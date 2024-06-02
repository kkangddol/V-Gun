#pragma once
#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>
#include "TypedefGraphicHelper.h"
#include "../RocketGraphicsInterface/AnimDesc.h"
#include "../RocketGraphicsInterface/RenderType.h"

struct ID3DX11EffectTechnique;
struct ID3D11ShaderResourceView;

namespace RocketCore::Graphics
{
	class AssetModelData;
	class Shader;
	class ShaderMaterial;
	class BaseFormation;
	class SkinnedMeshFormation;
	class ModelData;
	class RenderAnimation;
	class RenderCubeMap;
	class CameraMatrices;

	class ResourceManager
	{
	public:
		static ResourceManager& GetInstance()
		{
			static ResourceManager instance;
			return instance;
		}
		//자료가 제 장소가 들어오지 않을 경우, 디폴트 값으로 쓰일 Texture들 등등.. 세팅!
		void Initialize();

		//Graphics에 한정된 ResourceManager,
		//일반화 X, 자료저장소를 따로 두어 관리한다.

		///Shader, InputStruct
		//찾으면 그걸 가지고 이미 만들어진거 리턴, 없으면 새로.
		bool FoundExistingFXFile(const std::string& fxName);
		//Filepath, ModelData까지 받아서 ModelData의 Texture Path로는 SRV, 나머지로는 Shader를 만든다!
		///여기도 eRenderType 기록 때문에 크게 오류 날 예정이다!
		void AddShaderDataWithResource(const std::string& filePath, eRenderType renderType);
		ShaderInputSet FindShaderInputSetByName(const std::string& fxName);

		///3D Models
		static unsigned int recordModelHandleID; //지금까지의 핸들 기록. (ModelData)

		//모델 데이터 추가. (단순 추가, 껍데기), LoadFile에 의해 호출됨.
		AssetModelData* AppendGetModelData();

		//안에 있는 ModelDataMap에서 값을 찾는다.
		AssetModelData* FindModelByHandle(unsigned int handleID);

		//안에 있는 ModelDataMap에서 값을 찾는다.
		AssetModelData* FindModelByFileName(const std::string& fileName);

		///Formation

		//RenderPipeline에서 Formation을 자료구조에 넣어 Animation을 연동하고자 한다.
		void InsertNameFormation(const std::string& _modelPath, BaseFormation* _form);

		BaseFormation* FindFormationByModelPath(const std::string& modelPath);

		///Animation
		//모델 데이터 추가. (animDesc 정보만 옮기기), LoadAnimation에 의해 호출됨.
		RenderAnimation* AppendGetAnimationData(SkinnedMeshFormation* form, AnimDesc animDesc);
		RenderAnimation* FindAnimationByName(const std::string& name);
		
		//개별적으로 Shader 위한 함수들을 만들기, 뭉친 코드 덩어리를 만들지 않기 위해서 => CustomFXInstances.

		///Texture (별도 + with ShaderMaterial)
		Texture* MakeTextureFromFilePath(const std::string& filePath);
		void StoreTextureDataWithFilePath(const std::string filePath, Texture* texture);
		Texture* FindTextureByFilePath(const std::string& filePath);

		///Cubemap 만들기.
		RenderCubeMap* MakeCubeMapFromFilePath(const std::string& filePath, CameraMatrices* camMatrix);
		void StoreCubeMapWithFilePath(const std::string filePath, RenderCubeMap* cubeMap);
		RenderCubeMap* FindCubeMapByFilePath(const std::string& filePath);

		///개별적인 Default SRV Textures, 자료구조 거치지 않고 받을 수 있게.
		Texture* GetDefaultDiffuseTexture();
		Texture* GetDefaultNormalTexture();
		Texture* GetDefaultArmTexture();
		Texture* GetDefaultPixelTexture();

		///MakeRenderable, MakeSkinnedRenderable에서 옵젝이 만들어질 때마다 연동.
		void InsertNameRenderable(const std::string& objectname, BaseRenderable* ren);
		BaseRenderable* FindRenderableByObjectName(const std::string& objectName);

		///Release.
		//Data Map들 싹 다 Release. 할당한 리소스 놔줘야 함.
		void Release();

	private:
		std::unordered_map<std::string, ShaderInputSet> fxShaderMap;
		std::unordered_map<unsigned int, AssetModelData*> modelDataMap;
		std::unordered_map<std::string, RenderAnimation*> renderAnimationMap;
		std::unordered_map<std::string, BaseFormation*> modelFormationMap; //Formation도 만들어지면 저장된다.
		// Texture File Path / Texture Class Pointer.
		std::unordered_map<std::string, Texture*> _textureImageMap;
		std::unordered_map<std::string, RenderCubeMap*> _renderCubeMapMap;
		// 실제 Object Name / 지금까지 만들어진 GameObject의 BaseRenderable.
		std::unordered_map<std::string, BaseRenderable*> _nameObjectMap;
		
		//Animation 연동 이슈로, 지금까지 만들어진 
		//Pixel 하나.
		Texture* _defaultPixelSRV = nullptr;

		//DefaultTextures 곧바로 저장.
		// 추가될 수록, TextureStorage::AssignTextures에서 자동 연동이 늘어나야 한다.
		Texture* _defaultDiffuseSRV = nullptr;
		Texture* _defaultNormalSRV = nullptr;
		Texture* _defaultArmSRV = nullptr;

	private:
		//싱글턴
		ResourceManager() {}
		ResourceManager(const ResourceManager& ref) {}
		ResourceManager& operator=(const ResourceManager& ref) {}
		~ResourceManager() {}
		
	};
}


