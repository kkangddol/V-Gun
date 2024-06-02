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
		//�ڷᰡ �� ��Ұ� ������ ���� ���, ����Ʈ ������ ���� Texture�� ���.. ����!
		void Initialize();

		//Graphics�� ������ ResourceManager,
		//�Ϲ�ȭ X, �ڷ�����Ҹ� ���� �ξ� �����Ѵ�.

		///Shader, InputStruct
		//ã���� �װ� ������ �̹� ��������� ����, ������ ����.
		bool FoundExistingFXFile(const std::string& fxName);
		//Filepath, ModelData���� �޾Ƽ� ModelData�� Texture Path�δ� SRV, �������δ� Shader�� �����!
		///���⵵ eRenderType ��� ������ ũ�� ���� �� �����̴�!
		void AddShaderDataWithResource(const std::string& filePath, eRenderType renderType);
		ShaderInputSet FindShaderInputSetByName(const std::string& fxName);

		///3D Models
		static unsigned int recordModelHandleID; //���ݱ����� �ڵ� ���. (ModelData)

		//�� ������ �߰�. (�ܼ� �߰�, ������), LoadFile�� ���� ȣ���.
		AssetModelData* AppendGetModelData();

		//�ȿ� �ִ� ModelDataMap���� ���� ã�´�.
		AssetModelData* FindModelByHandle(unsigned int handleID);

		//�ȿ� �ִ� ModelDataMap���� ���� ã�´�.
		AssetModelData* FindModelByFileName(const std::string& fileName);

		///Formation

		//RenderPipeline���� Formation�� �ڷᱸ���� �־� Animation�� �����ϰ��� �Ѵ�.
		void InsertNameFormation(const std::string& _modelPath, BaseFormation* _form);

		BaseFormation* FindFormationByModelPath(const std::string& modelPath);

		///Animation
		//�� ������ �߰�. (animDesc ������ �ű��), LoadAnimation�� ���� ȣ���.
		RenderAnimation* AppendGetAnimationData(SkinnedMeshFormation* form, AnimDesc animDesc);
		RenderAnimation* FindAnimationByName(const std::string& name);
		
		//���������� Shader ���� �Լ����� �����, ��ģ �ڵ� ����� ������ �ʱ� ���ؼ� => CustomFXInstances.

		///Texture (���� + with ShaderMaterial)
		Texture* MakeTextureFromFilePath(const std::string& filePath);
		void StoreTextureDataWithFilePath(const std::string filePath, Texture* texture);
		Texture* FindTextureByFilePath(const std::string& filePath);

		///Cubemap �����.
		RenderCubeMap* MakeCubeMapFromFilePath(const std::string& filePath, CameraMatrices* camMatrix);
		void StoreCubeMapWithFilePath(const std::string filePath, RenderCubeMap* cubeMap);
		RenderCubeMap* FindCubeMapByFilePath(const std::string& filePath);

		///�������� Default SRV Textures, �ڷᱸ�� ��ġ�� �ʰ� ���� �� �ְ�.
		Texture* GetDefaultDiffuseTexture();
		Texture* GetDefaultNormalTexture();
		Texture* GetDefaultArmTexture();
		Texture* GetDefaultPixelTexture();

		///MakeRenderable, MakeSkinnedRenderable���� ������ ������� ������ ����.
		void InsertNameRenderable(const std::string& objectname, BaseRenderable* ren);
		BaseRenderable* FindRenderableByObjectName(const std::string& objectName);

		///Release.
		//Data Map�� �� �� Release. �Ҵ��� ���ҽ� ����� ��.
		void Release();

	private:
		std::unordered_map<std::string, ShaderInputSet> fxShaderMap;
		std::unordered_map<unsigned int, AssetModelData*> modelDataMap;
		std::unordered_map<std::string, RenderAnimation*> renderAnimationMap;
		std::unordered_map<std::string, BaseFormation*> modelFormationMap; //Formation�� ��������� ����ȴ�.
		// Texture File Path / Texture Class Pointer.
		std::unordered_map<std::string, Texture*> _textureImageMap;
		std::unordered_map<std::string, RenderCubeMap*> _renderCubeMapMap;
		// ���� Object Name / ���ݱ��� ������� GameObject�� BaseRenderable.
		std::unordered_map<std::string, BaseRenderable*> _nameObjectMap;
		
		//Animation ���� �̽���, ���ݱ��� ������� 
		//Pixel �ϳ�.
		Texture* _defaultPixelSRV = nullptr;

		//DefaultTextures ��ٷ� ����.
		// �߰��� ����, TextureStorage::AssignTextures���� �ڵ� ������ �þ�� �Ѵ�.
		Texture* _defaultDiffuseSRV = nullptr;
		Texture* _defaultNormalSRV = nullptr;
		Texture* _defaultArmSRV = nullptr;

	private:
		//�̱���
		ResourceManager() {}
		ResourceManager(const ResourceManager& ref) {}
		ResourceManager& operator=(const ResourceManager& ref) {}
		~ResourceManager() {}
		
	};
}


