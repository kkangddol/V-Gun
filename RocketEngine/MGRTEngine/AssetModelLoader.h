#pragma once
#include <string>
#include "UseASSIMP.h"
#include <windows.h>
#include <variant>
#include "SimpleMath.h"
#include "TypedefGraphicHelper.h"
#include "RenderUsageMesh.h"
#include "../RocketGraphicsInterface/AnimDesc.h"


//ASSIMP API 사용 위해.
//class aiScene;

namespace RocketCore::Graphics
{
	class SkinnedMeshFormation;
	class AssetModelData;
	class AssetSceneData;
	class AssetMeshData;

	//개별적인 Instance를 유지할 계획.
	class AssetModelLoader
	{
	public:
		AssetModelLoader();
		//Assimp관련 미리 작업 하기. (TBA)
		void InitAssimp();
		//Assimp 관련 할당 해제 하기. (TBA)
		void FinAssimp();

		// (일반 Mesh) FBX 파일에서 파일을 로드하고, ResourceManager에 추가해줌.
		void LoadMeshFile(const std::string& filePath);

		// (Skinned Mesh) FBX 파일에서 파일을 로드하고, ResourceManager에 추가해줌.
		void LoadSkinnedMeshFile(const std::string& filePath);

		//FBX 파일에서 애니메이션을 로드하고, ResourceManager에 추가해줌.
		void LoadAnimation(SkinnedMeshFormation* form, const AnimDesc& animDesc);
		
		//void NodeLessDataToJustMesh(AssetModelData* const _modelData, Formation* _form);

	private:
		//AssignDataToFormation이 내부적 사용, 
		//적법한 값 내부 조건, 돌리는 함수. (Vertex Shader Semantics)
		//std::variant<unsigned int, float, DirectX::SimpleMath::Vector3> ReturnValueSemantic(const std::string& _semName);

	
		//
		//void FillBoneMappingListToMeshes(MGRTModelData* _modelData);
		//void AssignWeightIndexToMeshes(MGRTModelData* _modelData);
		//void LoadBonesToMeshes(MGRTModelData* _modelData);
		//void LoadSingleBoneToMesh(UINT MeshIndex, const MGRTMeshData* pMesh, std::vector<RenderUsageMesh::RenderUsageVertex>& _bones);
		
		
	};
}



