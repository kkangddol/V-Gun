#pragma once
#include <string>
#include "UseASSIMP.h"
#include <windows.h>
#include <variant>
#include "SimpleMath.h"
#include "TypedefGraphicHelper.h"
#include "RenderUsageMesh.h"
#include "../RocketGraphicsInterface/AnimDesc.h"


//ASSIMP API ��� ����.
//class aiScene;

namespace RocketCore::Graphics
{
	class SkinnedMeshFormation;
	class AssetModelData;
	class AssetSceneData;
	class AssetMeshData;

	//�������� Instance�� ������ ��ȹ.
	class AssetModelLoader
	{
	public:
		AssetModelLoader();
		//Assimp���� �̸� �۾� �ϱ�. (TBA)
		void InitAssimp();
		//Assimp ���� �Ҵ� ���� �ϱ�. (TBA)
		void FinAssimp();

		// (�Ϲ� Mesh) FBX ���Ͽ��� ������ �ε��ϰ�, ResourceManager�� �߰�����.
		void LoadMeshFile(const std::string& filePath);

		// (Skinned Mesh) FBX ���Ͽ��� ������ �ε��ϰ�, ResourceManager�� �߰�����.
		void LoadSkinnedMeshFile(const std::string& filePath);

		//FBX ���Ͽ��� �ִϸ��̼��� �ε��ϰ�, ResourceManager�� �߰�����.
		void LoadAnimation(SkinnedMeshFormation* form, const AnimDesc& animDesc);
		
		//void NodeLessDataToJustMesh(AssetModelData* const _modelData, Formation* _form);

	private:
		//AssignDataToFormation�� ������ ���, 
		//������ �� ���� ����, ������ �Լ�. (Vertex Shader Semantics)
		//std::variant<unsigned int, float, DirectX::SimpleMath::Vector3> ReturnValueSemantic(const std::string& _semName);

	
		//
		//void FillBoneMappingListToMeshes(MGRTModelData* _modelData);
		//void AssignWeightIndexToMeshes(MGRTModelData* _modelData);
		//void LoadBonesToMeshes(MGRTModelData* _modelData);
		//void LoadSingleBoneToMesh(UINT MeshIndex, const MGRTMeshData* pMesh, std::vector<RenderUsageMesh::RenderUsageVertex>& _bones);
		
		
	};
}



