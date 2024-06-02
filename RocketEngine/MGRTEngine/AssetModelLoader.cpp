#include "AssetModelLoader.h"
#include "RenderPipeline.h"
#include "AssetModelData.h" //ModelData�� ������ ���̴�.
#include "AssetDataHelper.h" //Custom �ڷ����� ��ȯ�� ��
#include "AssetDataDefine.h"
#include "SkinnedMeshFormation.h"
#include <filesystem>
#include <algorithm>
#include "VertexStructHelper.h"
#include "ResourceManager.h"
#include "ByteBuffer.hpp"
#include "RenderAnimation.h"

///Mesh -> Assimp ���� ��ü�� �ϳ��� ����.
///SkinnedMesh -> ���� ������ Mesh 1��. => Culling�� Mesh[0]�� AABB��!

namespace RocketCore::Graphics
{
	AssetModelLoader::AssetModelLoader()
	{

	}

	void AssetModelLoader::InitAssimp()
	{

	}

	void AssetModelLoader::FinAssimp()
	{

	}

	void AssetModelLoader::LoadMeshFile(const std::string& filePath)
	{
		//Resource ���� w/ ��ü ����.
		AssetModelData* tModelData = ResourceManager::GetInstance().AppendGetModelData();

		Assimp::Importer importer;
		// �׳� Mesh ��¿� �ε�!
		const aiScene* pScene = importer.ReadFile(filePath,
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices | aiProcess_SortByPType |
			aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_GenBoundingBoxes);

		assert(pScene != nullptr);

		tModelData->_directory = filePath.substr(0, filePath.find_last_of("/\\"));

		std::filesystem::path tPath(filePath);
		tModelData->_fileName = tPath.filename().string();
		tModelData->_extension = tPath.extension().string();
		tModelData->_sceneData->m_Directory = tModelData->_directory;
		tModelData->_meshType = eModelMeshType::MESH;

		AssetDataHelper::CopyAssimpToAssetScene(pScene, tModelData->_sceneData);
		AssetDataHelper::FinalizeDataHelper();
	}

	void AssetModelLoader::LoadSkinnedMeshFile(const std::string& filePath)
	{
		//Resource ���� w/ ��ü ����.
		AssetModelData* tModelData = ResourceManager::GetInstance().AppendGetModelData();

		Assimp::Importer importer;
		//const aiScene* pScene = importer.ReadFile(filePath,
		//	aiProcess_Triangulate |
		//	aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices |
		//	aiProcess_PopulateArmatureData | aiProcess_GenBoundingBoxes |
		//	aiProcess_CalcTangentSpace | 
		//	aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights);

		const aiScene* pScene = importer.ReadFile(filePath,
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
			aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
			aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_LimitBoneWeights);

		assert(pScene != nullptr);

		tModelData->_directory = filePath.substr(0, filePath.find_last_of("/\\"));

		std::filesystem::path tPath(filePath);
		tModelData->_fileName = tPath.filename().string();
		tModelData->_extension = tPath.extension().string();
		tModelData->_sceneData->m_Directory = tModelData->_directory;
		tModelData->_meshType = eModelMeshType::SKINNEDMESH;

		AssetDataHelper::CopyAssimpToAssetScene(pScene, tModelData->_sceneData);
		AssetDataHelper::FinalizeDataHelper();
	}

	void AssetModelLoader::LoadAnimation(SkinnedMeshFormation* form, const AnimDesc& animDesc)
	{
		//Resource ���� w/ ��ü ����.
		RenderAnimation* tAnimationData = ResourceManager::GetInstance().AppendGetAnimationData(form, animDesc);

		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(animDesc.animPath,
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
			aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
			aiProcess_GenSmoothNormals | aiProcess_SortByPType | aiProcess_LimitBoneWeights);

		assert(pScene != nullptr);

		AssetDataHelper::CopyAssimpToRenderAnimation(pScene, tAnimationData);
		AssetDataHelper::FinalizeDataHelper();
	}








	
}
