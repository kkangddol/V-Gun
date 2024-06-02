#include "AssetDataDefine.h"
#include "Define3D.h"

namespace RocketCore::Graphics
{
	///생성/소멸자 딴에서 메모리 할당 안 할거다!
	///일괄적으로 처리 불가능.
	///다만, 소멸, 자료 연결 등은 이 페이지에서 필수적이다.
	//---------------

	AssetSceneData::AssetSceneData()
	{
		//m_RootNode = new MGRTNodeData;
		//mMetaData = new MGRTMetaData;
	}

	AssetSceneData::~AssetSceneData()
	{
		//delete m_RootNode;
		//delete mMetaData;
	}

	//원하는 Node를 찾는 용도. 
	RocketCore::Graphics::AssetNodeData* AssetSceneData::FindNode(const std::string& _nodeName)
	{
		return m_NodePlainContainer.at(_nodeName);
	}
	
	//---------------

	AssetNodeData::AssetNodeData(AssetNodeData* _parentNode)
	{
		//Parent 할당.
		this->m_Parent = _parentNode;
		//그냥 리셋.
		this->m_ParRelativeTransform = DirectX::XMMatrixIdentity();
	}

	AssetNodeData::~AssetNodeData()
	{
		//delete m_Parent;
		//delete m_MetaData;
	}

	//---------------

	AssetBoneData::AssetBoneData()
	{
		///결정했다! 일단, 지금 당장 구조를 파악하기는 어려운 상황.
		///그러니, ASSIMP가 내부적으로 쫙 다 돌리면서 Bone-Node를 연관시키더라도,
		///나도 별개로 내 딴에서 한번 더 돌리면서 값을 할당하는 것이 맞는 것 같다!
		///당연히 더 코스트가 들겠으나, 일단은 문제를 해결해야 하고,
		///관계를 일반 과정을 통해서 봐야 한다.

		//부모 node로, 넣어주기. The bone node in the scene 
		//m_Node = _nodeData;
		//여기서 주의! m_Armature는 Scene에 연동되는 것인가?
		m_Name.clear();
	}

	AssetBoneData::~AssetBoneData()
	{
		//delete m_Armature;
		//delete m_Node;
	}

	//---------------

	AssetSkeletonBoneData::AssetSkeletonBoneData()
	{
		//m_Armature = new MGRTNodeData;
		//m_Node = new MGRTNodeData;
		//mMeshId = new MGRTMeshData;
	}

	AssetSkeletonBoneData::~AssetSkeletonBoneData()
	{
		//delete m_Armature;
		//delete m_Node;
		//delete mMeshId;
	}

	//CustomVertexBoneData::CustomVertexBoneData()
	//{
	//	//아무리 Bone의 영향을 받지 않더라도, 멈추지는 않게 되어 있다.
	//	memset(IDs, 0, 4 * sizeof(IDs[0]));
	//	memset(Weights, 0, 4 * sizeof(Weights[0]));
	//}

}
