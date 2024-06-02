#pragma once
#include <vector>
#include <cstdint>
#include <stdarg.h>
#include <memory>
#include <map>
#include <tuple>
#include <string>
#include "TypedefGraphicHelper.h"
#include "BaseFormation.h"
#include "SimpleMath.h"
#include <DirectXMath.h>

#include "RenderUsageMesh.h"
#include "RenderUsageBoneInfo.h"
#include "RenderUsageVertexBone.h"

//���⿡���ִ� VStruct ���� ���¸� ����ؾ� �Ѵ�!
///���� : ID3D11Buffer*�� VB, IB�� �޴� ����̴�.

struct ID3D11InputLayout;
struct ID3D11Buffer;

namespace RocketCore::Graphics
{
	class AssetSceneData;
	class AssetNodeData;
	class AssetBoneData;
	class AssetNodeAnimData;
	class AssetMeshData;

	class ByteBuffer;
	class AssetModelData;
	class AnimationHandler;

	class SkinnedMeshFormation : public BaseFormation
	{
		friend class AssetModelLoader;
		friend class SkinnedMeshObject;
		friend class RenderAnimation;
	public:

		virtual void AssignData(AssetModelData* const modelData) override;
		//���������� �ٸ� TM/Ư¡�� ���� �ִ� Node���� ���� NestedClass (MGRTNode�� 1��1 ����)
		//Node���� Mesh���� ���� ����.
		//�� Formation �ȿ��� ModelLoader�� ������ ��� ������ ������ �� ��.

		//Layout��, ���̴� VertexStruct�� VB ���� �ϳ��� Byte Size�� ���� �Ѵ�.
		SkinnedMeshFormation(const AssetInputStruct& _inputStruct, AssetModelData* _modelData);
		virtual ~SkinnedMeshFormation();

		///��� Assimp�� �������� X. Render�� ���ؼ� �����ϴ� ����.
		///����, RenderUsageNode -> RenderUsage Mesh�� �ٲ㼭 ������ �־�� �Ѵ�. (Hierarchical + )
		///Animation ������ ����ؼ� ��������.

		std::vector<RenderUsageMesh> m_MeshEntries;
		unsigned int m_RenderNumBone = 0;
		std::vector<RenderUsageBoneInfo> m_RenderBoneInfo;
		//Vertex�� �ϴ��� ����.
		std::vector<RenderUsageVertexBone> m_VertexBoneList;
		//�̹� ������ �ε��� �� ( == Vertex Buffer Input�� ������ ��) ���� �־���.
		//�׷��Ƿ�, �̴� ����� ��������θ� ���� ����.
		//Bone ��ĸ� ����� �Է��ϸ� �Ǵ� ���ε�,
		std::map<std::string, unsigned int> m_BoneMapping;
		
		///���� �갡 ���� ������ �ȵȴ�..! => �̰� ��� �ذ�����..
		///�� ������ m_AnimHandler�� �� �� SkinnedMeshObject�� �ű�� �Ǵ� �����̴�. ����.

		//�ٽ� ���⿡�� VB/IB�� ���;� �Ѵ�.
		ID3D11Buffer* m_VB = nullptr;
		ID3D11Buffer* m_IB = nullptr;
		UINT m_IndexCount = 0;

		//VB �޾Ƶ��̴� �뵵.
		ByteBuffer* m_ByteBufferForVB = nullptr;
		//IB �޾Ƶ��̴� �뵵.
		std::vector<unsigned int> m_IBVec;
		//���⼭ Indice ������ ����� �ϴµ�,

		//Animation ������ �ޱ� ����(���)
		ByteBuffer* m_ByteBufferForBones = nullptr;

		//Bone���� �ִ� ����� 100���� �����Ѵ�. (��������)
	private:
		//RenderUsageVertexBone : ������ 
		void InitAnimDataFromScene(std::vector<RenderUsageVertexBone>& outBone, std::vector<unsigned int>& outIndice, AssetModelData* const modelData);
		void InitRenderMesh(unsigned int index, AssetMeshData* meshData, 
			std::vector<unsigned int>& indicesList, std::vector<RenderUsageVertexBone>& vBonesList);
		void LoadRenderBones(unsigned int meshIndex, AssetMeshData* mesh,
			std::vector<RenderUsageVertexBone>& vBonesList);
		
		AssetModelData* _modelData = nullptr;

		void PutSingleBufferElement(void* _buffer, size_t _bufLenInBytes);
		void ReadSingleBufferElement(void* _buffer, size_t _bufLenInBytes);
		
		//Formation�� �Ҵ�� Bone���� ����.
		unsigned int _formationNumBone;

		
	};
}


