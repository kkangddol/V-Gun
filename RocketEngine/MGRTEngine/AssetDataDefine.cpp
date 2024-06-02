#include "AssetDataDefine.h"
#include "Define3D.h"

namespace RocketCore::Graphics
{
	///����/�Ҹ��� ������ �޸� �Ҵ� �� �ҰŴ�!
	///�ϰ������� ó�� �Ұ���.
	///�ٸ�, �Ҹ�, �ڷ� ���� ���� �� ���������� �ʼ����̴�.
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

	//���ϴ� Node�� ã�� �뵵. 
	RocketCore::Graphics::AssetNodeData* AssetSceneData::FindNode(const std::string& _nodeName)
	{
		return m_NodePlainContainer.at(_nodeName);
	}
	
	//---------------

	AssetNodeData::AssetNodeData(AssetNodeData* _parentNode)
	{
		//Parent �Ҵ�.
		this->m_Parent = _parentNode;
		//�׳� ����.
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
		///�����ߴ�! �ϴ�, ���� ���� ������ �ľ��ϱ�� ����� ��Ȳ.
		///�׷���, ASSIMP�� ���������� �� �� �����鼭 Bone-Node�� ������Ű����,
		///���� ������ �� ������ �ѹ� �� �����鼭 ���� �Ҵ��ϴ� ���� �´� �� ����!
		///�翬�� �� �ڽ�Ʈ�� �������, �ϴ��� ������ �ذ��ؾ� �ϰ�,
		///���踦 �Ϲ� ������ ���ؼ� ���� �Ѵ�.

		//�θ� node��, �־��ֱ�. The bone node in the scene 
		//m_Node = _nodeData;
		//���⼭ ����! m_Armature�� Scene�� �����Ǵ� ���ΰ�?
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
	//	//�ƹ��� Bone�� ������ ���� �ʴ���, �������� �ʰ� �Ǿ� �ִ�.
	//	memset(IDs, 0, 4 * sizeof(IDs[0]));
	//	memset(Weights, 0, 4 * sizeof(Weights[0]));
	//}

}
