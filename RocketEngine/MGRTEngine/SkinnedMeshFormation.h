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

//여기에서있는 VStruct 내부 형태만 사용해야 한다!
///변경 : ID3D11Buffer*가 VB, IB를 받는 방식이다.

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
		//개별적으로 다른 TM/특징을 갖고 있는 Node들을 위한 NestedClass (MGRTNode와 1대1 대응)
		//Node들이 Mesh들을 갖고 있음.
		//한 Formation 안에서 ModelLoader와 동일한 노드 구조를 복사해 올 것.

		//Layout과, 쓰이는 VertexStruct의 VB 단위 하나의 Byte Size가 들어가야 한다.
		SkinnedMeshFormation(const AssetInputStruct& _inputStruct, AssetModelData* _modelData);
		virtual ~SkinnedMeshFormation();

		///얘는 Assimp에 고유하지 X. Render를 위해서 설정하는 값들.
		///이제, RenderUsageNode -> RenderUsage Mesh로 바꿔서 가지고 있어야 한다. (Hierarchical + )
		///Animation 연동도 고려해서 생각하자.

		std::vector<RenderUsageMesh> m_MeshEntries;
		unsigned int m_RenderNumBone = 0;
		std::vector<RenderUsageBoneInfo> m_RenderBoneInfo;
		//Vertex랑 일대일 대응.
		std::vector<RenderUsageVertexBone> m_VertexBoneList;
		//이미 정보를 로드할 때 ( == Vertex Buffer Input을 설정할 때) 값을 넣었음.
		//그러므로, 이는 디버그 참고용으로만 사용될 예정.
		//Bone 행렬만 제대로 입력하면 되는 것인데,
		std::map<std::string, unsigned int> m_BoneMapping;
		
		///이제 얘가 여기 있으면 안된다..! => 이걸 어떻게 해결하지..
		///이 문제는 m_AnimHandler를 싹 다 SkinnedMeshObject로 옮기면 되는 문제이다. ㄱㄱ.

		//다시 여기에서 VB/IB가 들어와야 한다.
		ID3D11Buffer* m_VB = nullptr;
		ID3D11Buffer* m_IB = nullptr;
		UINT m_IndexCount = 0;

		//VB 받아들이는 용도.
		ByteBuffer* m_ByteBufferForVB = nullptr;
		//IB 받아들이는 용도.
		std::vector<unsigned int> m_IBVec;
		//여기서 Indice 구별을 해줘야 하는데,

		//Animation 데이터 받기 위해(행렬)
		ByteBuffer* m_ByteBufferForBones = nullptr;

		//Bone들의 최대 행렬은 100개로 제한한다. (이제부터)
	private:
		//RenderUsageVertexBone : 실제로 
		void InitAnimDataFromScene(std::vector<RenderUsageVertexBone>& outBone, std::vector<unsigned int>& outIndice, AssetModelData* const modelData);
		void InitRenderMesh(unsigned int index, AssetMeshData* meshData, 
			std::vector<unsigned int>& indicesList, std::vector<RenderUsageVertexBone>& vBonesList);
		void LoadRenderBones(unsigned int meshIndex, AssetMeshData* mesh,
			std::vector<RenderUsageVertexBone>& vBonesList);
		
		AssetModelData* _modelData = nullptr;

		void PutSingleBufferElement(void* _buffer, size_t _bufLenInBytes);
		void ReadSingleBufferElement(void* _buffer, size_t _bufLenInBytes);
		
		//Formation에 할당된 Bone들의 숫자.
		unsigned int _formationNumBone;

		
	};
}


