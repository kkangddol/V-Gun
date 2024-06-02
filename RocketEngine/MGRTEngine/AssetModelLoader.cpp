#include "AssetModelLoader.h"
#include "RenderPipeline.h"
#include "AssetModelData.h" //ModelData를 저장할 것이다.
#include "AssetDataHelper.h" //Custom 자료형을 반환할 것
#include "AssetDataDefine.h"
#include "SkinnedMeshFormation.h"
#include <filesystem>
#include <algorithm>
#include "VertexStructHelper.h"
#include "ResourceManager.h"
#include "ByteBuffer.hpp"
#include "RenderAnimation.h"

///Mesh -> Assimp 설정 자체로 하나로 통일.
///SkinnedMesh -> 들어올 때부터 Mesh 1개. => Culling은 Mesh[0]의 AABB로!

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
		//Resource 관리 w/ 객체 생성.
		AssetModelData* tModelData = ResourceManager::GetInstance().AppendGetModelData();

		Assimp::Importer importer;
		// 그냥 Mesh 출력용 로드!
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
		//Resource 관리 w/ 객체 생성.
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
		//Resource 관리 w/ 객체 생성.
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
