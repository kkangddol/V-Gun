#include "AssetDataHelper.h"
#include "UseASSIMP.h"
#include "assimp/color4.h"
#include "AssetDataDefine.h"
#include "Define3D.h"
#include "LowLevelDX11.h"
#include <cmath>
#include <limits>
#include <memory>
#include <cassert>
#include <d3d11.h>
#include <limits>
#include <numeric>
#include "RenderAnimation.h"


using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;
using DirectX::SimpleMath::Quaternion;

namespace RocketCore::Graphics
{
	AssetSceneData* AssetDataHelper::s_CurrentDataScene = nullptr;

	//ASSIMP�� aiScene�� ������ �� �ʿ��� �����͸� MGRTXXXData �����ͷ� �ű��.
	void AssetDataHelper::CopyAssimpToAssetScene(const aiScene* _assimpScene, AssetSceneData* _mgrtScene)
	{
		//Node�� ��������� ���� �Űܾ� �Ѵ�.
		//RootNode �ű��.
		///�޸� ����/������, ��Ȳ�� ���� �˾Ƽ� �Ǵ�����! �ϰ������� �Ҵ��� ���� ���� ��Ȳ�̴�.
		///��, �Ҵ��� �޸𸮸� �ϰ������� �����ؼ� ������ �ʿ�� �ִ�. 

		s_CurrentDataScene = _mgrtScene;

		//�˾Ƽ� nullptr ó��. Root/Rest �����̱⿡ ��� X.
		_mgrtScene->m_RootNode = new AssetNodeData(nullptr);
		CopyRootNodeToAsset(_assimpScene->mRootNode, _mgrtScene->m_RootNode);

		//Global Inverse �Ҵ�!
		DirectX::SimpleMath::Matrix tGlobalTrans = _mgrtScene->m_RootNode->m_ParRelativeTransform;
		_mgrtScene->m_GlobalTransform = tGlobalTrans;
		DirectX::XMVECTOR tDet = DirectX::XMVectorZero();
		_mgrtScene->m_GlobalInverseTransform = DirectX::XMMatrixInverse(&tDet, tGlobalTrans);

		//_mgrtScene->m_GlobalInverseTransform =_mgrtScene->m_RootNode->m_ParRelativeTransform;
		DirectX::XMVECTOR tRes = DirectX::XMVectorEqual(tDet, DirectX::XMVectorZero());
		DirectX::XMFLOAT4 tResF;
		DirectX::XMStoreFloat4(&tResF, tRes);
		//Inverse�� ���� ���� ��Ľ��� 0�� �ƴ��� �˻�.
		assert(tResF.x != 0xFFFFFFFF && tResF.y != 0xFFFFFFFF
			&& tResF.z != 0xFFFFFFFF && tResF.w != 0xFFFFFFFF);

		_mgrtScene->m_NumMesh = _assimpScene->mNumMeshes;
		_mgrtScene->m_MeshList.reserve(_mgrtScene->m_NumMesh);
		for (size_t i = 0; i < _mgrtScene->m_NumMesh; i++)
		{
			AssetMeshData* tMesh = new AssetMeshData;
			CopyMeshToAsset(_assimpScene->mMeshes[i], tMesh);
			_mgrtScene->m_MeshList.push_back(tMesh);
		}

		_mgrtScene->m_NumMaterials = _assimpScene->mNumMaterials;
		_mgrtScene->m_MaterialList.reserve(_mgrtScene->m_NumMaterials);

		///FBX�� �������� Texture ��δ� ���� �ޱ�� �ߴ�. ModelData���� �޾Ƽ� �ݿ�����. 
		///�����Ͱ� ����� ��, �������� ����� �� �� ���� ��찡 �����.
		//for (size_t i = 0; i < _mgrtScene->m_NumMaterials; i++)
		//{
		//	AssetMaterialData* tMat = new AssetMaterialData;
		//	///��� Material�� �׽����ϱ� ���� ����.
		//	//CopyMaterialToAsset(_assimpScene->mMaterials[i], tMat);
		//	for (size_t j = 0; j <= ASSET_MAXIMUM_TEXTURE_PROP_CNT; j++)
		//	{
		//		eAssetTextureType tTexType = static_cast<eAssetTextureType>(j);
		//		int tTexTypeTexCnt = _assimpScene->mMaterials[i]->GetTextureCount((aiTextureType)tTexType);
		//		if (tTexTypeTexCnt > 0)
		//		{
		//			std::vector<AssetTextureSRV> tATS = GetTexturesFromMaterial(_assimpScene->mMaterials[i], static_cast<aiTextureType>(j), 
		//				static_cast<eAssetTextureType>(j), _assimpScene);
		//		}
		//	}
		//	_mgrtScene->m_MaterialList.push_back(tMat);
		//}

		_mgrtScene->m_NumAnimation = _assimpScene->mNumAnimations;
		_mgrtScene->m_AnimationList.reserve(_mgrtScene->m_NumAnimation);
		for (size_t i = 0; i < _mgrtScene->m_NumAnimation; i++)
		{
			AssetAnimationData* tAnim = new AssetAnimationData;
			CopyAnimationToAsset(_assimpScene->mAnimations[i], tAnim);
			_mgrtScene->m_AnimationList.push_back(tAnim);
		}

		_mgrtScene->m_NumTexture = _assimpScene->mNumTextures;
		_mgrtScene->m_TextureList.reserve(_mgrtScene->m_NumTexture);
		for (size_t i = 0; i < _mgrtScene->m_NumTexture; i++)
		{
			AssetTextureData* tTexture = new AssetTextureData;
			CopyTextureToAsset(_assimpScene->mTextures[i], tTexture);
			_mgrtScene->m_TextureList.push_back(tTexture);
		}

		_mgrtScene->mMetaData = new AssetMetaData;
		CopyMetadataToAsset(_assimpScene->mMetaData, _mgrtScene->mMetaData);

		_mgrtScene->m_SceneName = _assimpScene->mName.C_Str();

		_mgrtScene->m_NumSkeleton = _assimpScene->mNumSkeletons;
		_mgrtScene->m_SkeletonList.reserve(_mgrtScene->m_NumSkeleton);
		for (size_t i = 0; i < _mgrtScene->m_NumSkeleton; i++)
		{
			AssetSkeletonData* tSkele = new AssetSkeletonData;
			CopySkeletonToAsset(_assimpScene->mSkeletons[i], tSkele);
			_mgrtScene->m_SkeletonList.push_back(tSkele);
		}

		//Scene ������ Has~ �ø��� �ޱ�.
		_mgrtScene->m_SceneHasAnimations = _assimpScene->HasAnimations();
		_mgrtScene->m_SceneHasSkeletons = _assimpScene->hasSkeletons();
		_mgrtScene->m_SceneHasMaterials = _assimpScene->HasMaterials();
		_mgrtScene->m_SceneHasMeshes = _assimpScene->HasMeshes();
		_mgrtScene->m_SceneHasTextures = _assimpScene->HasTextures();
	}

	void AssetDataHelper::FinalizeDataHelper()
	{
		s_CurrentDataScene = nullptr;
	}

	///��� Root Node�� �����ϴ� ��Ŀ����, �����鼭 ���������� Node ������ �޴� ���� ���� ������� ������ ���� ��.
	void AssetDataHelper::CopyRootNodeToAsset(const aiNode* _assimp, AssetNodeData* _mgrt)
	{
		_mgrt->m_NodeName = _assimp->mName.C_Str();
		CopyAssimpMatrixToAsset(&(_assimp->mTransformation), _mgrt->m_ParRelativeTransform);

		///Surgery
		/////[TRY] Matrix Animation�� ���� (+ ȣȯ), Transpose�ؼ� �޾ƿ´�.
		//_mgrt->m_ParRelativeTransform = _mgrt->m_ParRelativeTransform.Transpose();

		DirectX::XMVECTOR ttScale;
		DirectX::XMVECTOR ttRotQuat;
		DirectX::XMVECTOR ttTranslate;
		DirectX::XMMatrixDecompose(&ttScale, &ttRotQuat, &ttTranslate, _mgrt->m_ParRelativeTransform);

		DirectX::XMFLOAT3 ttScaleFF;
		DirectX::XMFLOAT4 ttRotQuatFF;
		DirectX::XMFLOAT3 ttTranslateFF;

		DirectX::XMStoreFloat3(&ttScaleFF, ttScale);
		DirectX::XMStoreFloat4(&ttRotQuatFF, ttRotQuat);
		DirectX::XMStoreFloat3(&ttTranslateFF, ttTranslate);

		XMVECTOR translation = XMVectorSet(ttTranslateFF.x, ttTranslateFF.y, ttTranslateFF.z, 1.0f);
		XMVECTOR rotationQuaternion = XMVectorSet(ttRotQuatFF.z, ttRotQuatFF.x, ttRotQuatFF.y, ttRotQuatFF.w);

		XMVECTOR scale = XMVectorSet(ttScaleFF.x, ttScaleFF.y, ttScaleFF.z, 1.0f);

		// Create the transformation matrix for the bone
		XMMATRIX tBoneTransform = XMMatrixScalingFromVector(scale) *
			XMMatrixRotationQuaternion(rotationQuaternion) *
			XMMatrixTranslationFromVector(translation);

		_mgrt->m_ParRelativeTransform = DirectX::XMMatrixTranspose(tBoneTransform);

		///TRY
		_mgrt->m_ParRelativeTransform = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(180.0f));
		///
		
		//������ �� Ȯ���ϰ� : Metadata�� ���� �޾ƿ´�.
		_mgrt->m_NumMesh = _assimp->mNumMeshes;
		for (unsigned int i = 0; i < _mgrt->m_NumMesh; i++)
		{
			_mgrt->m_MeshIndex.push_back(_assimp->mMeshes[i]);
		}

		IF_NOT_NULL(_assimp->mMetaData,
			_mgrt->m_MetaData = new AssetMetaData;
		CopyMetadataToAsset(_assimp->mMetaData, _mgrt->m_MetaData);
		);

		//��带 �ٽ� ������ ���ε�, �̸� �ܼ��� ������ ���� �ƴ�, 
		//��带 ������� ��������� ���鼭 ���� �Ҵ��ؾ� �Ѵ�.

		//�ڽ��� ����, ����� �����̳��� m_NodePlainContainer�� ����.
		if (s_CurrentDataScene != nullptr)
		{
			s_CurrentDataScene->m_NodePlainContainer.insert(std::make_pair(_mgrt->m_NodeName, _mgrt));
		}

		//RootNode�̴�, nullptr. (�����ڿ��� �̹� �� �ִ�)
		_mgrt->m_NumChildren = _assimp->mNumChildren;

		IF_NOT_ZERO(_mgrt->m_NumChildren,
			_mgrt->m_ChildrenList.reserve(_mgrt->m_NumChildren);
		for (size_t i = 0; i < _mgrt->m_NumChildren; i++)
		{
			//������� ��������� ���� �Ѵ�. (Parent�� Root Node�� ���� ��, ��� ���� ���� ����)
			AssetNodeData* tNode = new AssetNodeData(_mgrt);
			CopyRestNodeToAsset(_assimp->mChildren[i], tNode);
			_mgrt->m_ChildrenList.push_back(tNode);
		});
	}

	void AssetDataHelper::CopyRestNodeToAsset(const aiNode* _assimp, AssetNodeData* _mgrt)
	{
		//MGRTNode 
		_mgrt->m_NodeName = _assimp->mName.C_Str();
		CopyAssimpMatrixToAsset(&(_assimp->mTransformation), _mgrt->m_ParRelativeTransform);

		///Surgery
		/////[TRY] Matrix Animation�� ���� (+ ȣȯ), Transpose�ؼ� �޾ƿ´�.
		//_mgrt->m_ParRelativeTransform = _mgrt->m_ParRelativeTransform.Transpose();

		DirectX::XMVECTOR ttScale;
		DirectX::XMVECTOR ttRotQuat;
		DirectX::XMVECTOR ttTranslate;
		DirectX::XMMatrixDecompose(&ttScale, &ttRotQuat, &ttTranslate, _mgrt->m_ParRelativeTransform);

		DirectX::XMFLOAT3 ttScaleFF;
		DirectX::XMFLOAT4 ttRotQuatFF;
		DirectX::XMFLOAT3 ttTranslateFF;

		DirectX::XMStoreFloat3(&ttScaleFF, ttScale);
		DirectX::XMStoreFloat4(&ttRotQuatFF, ttRotQuat);
		DirectX::XMStoreFloat3(&ttTranslateFF, ttTranslate);

		XMVECTOR translation = XMVectorSet(ttTranslateFF.x, ttTranslateFF.y, ttTranslateFF.z, 1.0f);
		XMVECTOR rotationQuaternion = XMVectorSet(ttRotQuatFF.x, ttRotQuatFF.y, ttRotQuatFF.z, ttRotQuatFF.w);

		XMVECTOR scale = XMVectorSet(ttScaleFF.x, ttScaleFF.y, ttScaleFF.z, 1.0f);

		// Create the transformation matrix for the bone
		XMMATRIX tBoneTransform = XMMatrixScalingFromVector(scale) *
			XMMatrixRotationQuaternion(rotationQuaternion) *
			XMMatrixTranslationFromVector(translation);

		_mgrt->m_ParRelativeTransform = DirectX::XMMatrixTranspose(tBoneTransform);
		
		///[TO_FIX_LATER] �̰� �� �Ǵ����� �𸣰�����, �ȴ�. ���߿� �ٸ� �𵨿��� ���� ����� ��ġ��!
		_mgrt->m_ParRelativeTransform = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(180.0f));
		///
		_mgrt->m_NumMesh = _assimp->mNumMeshes;
		for (unsigned int i = 0; i < _mgrt->m_NumMesh; i++)
		{
			_mgrt->m_MeshIndex.push_back(_assimp->mMeshes[i]);
		}

		IF_NOT_NULL(_assimp->mMetaData,
			_mgrt->m_MetaData = new AssetMetaData;
		CopyMetadataToAsset(_assimp->mMetaData, _mgrt->m_MetaData);
		);

		//�ڽ��� ����, ����� �����̳��� m_NodePlainContainer�� ����.
		if (s_CurrentDataScene != nullptr)
		{
			s_CurrentDataScene->m_NodePlainContainer.insert(std::make_pair(_mgrt->m_NodeName, _mgrt));
		}

		//Parent Node�� �̹� �����ڿ��� �� �ִ� ����.
		_mgrt->m_NumChildren = _assimp->mNumChildren;
		if (_mgrt->m_NumChildren == 0)
		{
			//�ڱⰡ Node�� �ǳ��� ��ġ�� �ִٴ� ���.
			return;
		}
		else
		{
			///NULL Check �Ϸ�.
			_mgrt->m_ChildrenList.reserve(_mgrt->m_NumChildren);

			//Children�� �� ���Ҵٴ� ���.
			for (size_t i = 0; i < _mgrt->m_NumChildren; i++)
			{
				//������� ��������� ���� �Ѵ�. (Parent�� Root Node�� ���� ��, ��� ���� ���� ����)
				AssetNodeData* tNode = new AssetNodeData(_mgrt);
				CopyRestNodeToAsset(_assimp->mChildren[i], tNode);
				_mgrt->m_ChildrenList.push_back(tNode);
			}
		}
	}

	void AssetDataHelper::CopyFaceToAsset(const aiFace* _assimp, AssetFaceData* _mgrt)
	{
		_mgrt->m_NumIndice = _assimp->mNumIndices;
		_mgrt->m_IndiceList.reserve(_mgrt->m_NumIndice);
		for (unsigned int i = 0; i < _mgrt->m_NumIndice; i++)
		{
			_mgrt->m_IndiceList.push_back(_assimp->mIndices[i]);
		}
	}

	void AssetDataHelper::CopyAnimMeshToAsset(const aiAnimMesh* _assimp, MGRTAnimMeshData* _mgrt)
	{
		_mgrt->m_Name = _assimp->mName.C_Str();
		_mgrt->m_NumVertice = _assimp->mNumVertices;
		_mgrt->m_Weight = _assimp->mWeight;

		_mgrt->m_VerticeList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_NormalList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_TangentList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_BiTangentList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_ColorList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_TextureCoordList.reserve(_mgrt->m_NumVertice);
		///Null ��������! NULLPTR ������ �ֵ� �ذ��ؾ�.

		IF_NOT_NULL(_assimp->mVertices,
			for (size_t i = 0; i < _mgrt->m_NumVertice; i++)
			{
				_mgrt->m_VerticeList.push_back(Vector3(_assimp->mVertices[i].x, _assimp->mVertices[i].y, _assimp->mVertices[i].z));
			}
		);

		IF_NOT_NULL(_assimp->mNormals,
			for (size_t i = 0; i < _mgrt->m_NumVertice; i++)
			{
				_mgrt->m_NormalList.push_back(Vector3(_assimp->mNormals[i].x, _assimp->mNormals[i].y, _assimp->mNormals[i].z));
			}
		);

		IF_NOT_NULL(_assimp->mTangents,
			for (size_t i = 0; i < _mgrt->m_NumVertice; i++)
			{
				_mgrt->m_TangentList.push_back(Vector3(_assimp->mTangents[i].x, _assimp->mTangents[i].y, _assimp->mTangents[i].z));
			}
		);

		IF_NOT_NULL(_assimp->mBitangents,
			for (size_t i = 0; i < _mgrt->m_NumVertice; i++)
			{
				_mgrt->m_BiTangentList.push_back(Vector3(_assimp->mBitangents[i].x, _assimp->mBitangents[i].y, _assimp->mBitangents[i].z));
			}
		);

		///�ϴ� ����.

		///�ε� �ذ��ߴ�!
		if (_assimp->mColors[0])
		{
			for (unsigned int i = 0; i < _mgrt->m_NumVertice; i++)
			{
				Color tCol = Color(_assimp->mColors[0][i].r,
					_assimp->mColors[0][i].g,
					_assimp->mColors[0][i].b,
					_assimp->mColors[0][i].a);
				_mgrt->m_ColorList.push_back(tCol);
			}
		}

		if (_assimp->mTextureCoords[0])
		{
			for (unsigned int i = 0; i < _mgrt->m_NumVertice; i++)
			{
				Vector3 tTexCoord = Vector3(_assimp->mTextureCoords[0][i].x,
					_assimp->mTextureCoords[0][i].y,
					_assimp->mTextureCoords[0][i].z);
				_mgrt->m_TextureCoordList.push_back(tTexCoord);
			}
		}

		///Ư�� ��� ���� ���� �Ǵ�.
		_mgrt->m_MeshHasTextureCoords = _assimp->HasTextureCoords(0);
	}

	void AssetDataHelper::CopyMeshToAsset(const aiMesh* _assimp, AssetMeshData* _mgrt)
	{
		_mgrt->m_PrimitiveTypes = _assimp->mPrimitiveTypes;
		_mgrt->m_NumVertice = _assimp->mNumVertices;
		_mgrt->m_NumFace = _assimp->mNumFaces;

		_mgrt->m_VerticeList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_NormalList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_TangentList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_BiTangentList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_ColorList.reserve(_mgrt->m_NumVertice);
		_mgrt->m_TextureCoordList.reserve(_mgrt->m_NumVertice);

		IF_NOT_NULL(_assimp->mVertices,
			for (size_t i = 0; i < _mgrt->m_NumVertice; i++)
			{
				_mgrt->m_VerticeList.push_back(Vector3(_assimp->mVertices[i].x, _assimp->mVertices[i].y, _assimp->mVertices[i].z));
			}
		);

		IF_NOT_NULL(_assimp->mNormals,
			for (size_t i = 0; i < _mgrt->m_NumVertice; i++)
			{
				_mgrt->m_NormalList.push_back(Vector3(_assimp->mNormals[i].x, _assimp->mNormals[i].y, _assimp->mNormals[i].z));
			}
		);

		IF_NOT_NULL(_assimp->mTangents,
			for (size_t i = 0; i < _mgrt->m_NumVertice; i++)
			{
				_mgrt->m_TangentList.push_back(Vector3(_assimp->mTangents[i].x, _assimp->mTangents[i].y, _assimp->mTangents[i].z));
			}
		);

		IF_NOT_NULL(_assimp->mBitangents,
			for (size_t i = 0; i < _mgrt->m_NumVertice; i++)
			{
				_mgrt->m_BiTangentList.push_back(Vector3(_assimp->mBitangents[i].x, _assimp->mBitangents[i].y, _assimp->mBitangents[i].z));
			}
		);

		///�ε� �ذ��ߴ�!
		if (_assimp->mColors[0])
		{
			for (unsigned int i = 0; i < _mgrt->m_NumVertice; i++)
			{
				Color tCol = Color(_assimp->mColors[0][i].r,
					_assimp->mColors[0][i].g,
					_assimp->mColors[0][i].b,
					_assimp->mColors[0][i].a);
				_mgrt->m_ColorList.push_back(tCol);
			}
		}

		if (_assimp->mTextureCoords[0])
		{
			for (unsigned int i = 0; i < _mgrt->m_NumVertice; i++)
			{
				Vector3 tTexCoord = Vector3(_assimp->mTextureCoords[0][i].x,
					_assimp->mTextureCoords[0][i].y,
					_assimp->mTextureCoords[0][i].z);
				_mgrt->m_TextureCoordList.push_back(tTexCoord);
			}
		}

		for (int i = 0; i < ASSET_MAX_NUMBER_OF_TEXTURECOORDS; i++)
		{
			_mgrt->m_NumUVComponent[i] = _assimp->mNumUVComponents[i];
		}

		_mgrt->m_FaceList.reserve(_mgrt->m_NumFace);
		for (unsigned int i = 0; i < _mgrt->m_NumFace; i++)
		{
			AssetFaceData tFaceData;
			CopyFaceToAsset(&(_assimp->mFaces[i]), &tFaceData);
			_mgrt->m_FaceList.push_back(tFaceData);
		}

		////�̸� ������ ���� Custom ����ü ���� (Vertex ������ ����)
		//_mgrt->m_CustomPerVertexBlendList.resize(_mgrt->m_NumVertice);

		_mgrt->m_NumBone = _assimp->mNumBones;
		_mgrt->m_BoneList.reserve(_mgrt->m_NumBone);

		IF_NOT_NULL(_assimp->mBones,
			for (unsigned int i = 0; i < _mgrt->m_NumBone; i++)
			{
				AssetBoneData* tBoneData = new AssetBoneData;
				CopyBoneToAsset(_assimp->mBones[i], _mgrt, tBoneData);
				_mgrt->m_BoneList.push_back(tBoneData);
			});




		//Material�� ������ ����.

		_mgrt->m_Name = _assimp->mName.C_Str();
		_mgrt->m_NumAnimMeshes = _assimp->mNumAnimMeshes;
		_mgrt->m_AnimMeshList.reserve(_mgrt->m_NumAnimMeshes);

		IF_NOT_NULL(_assimp->mAnimMeshes,
			for (unsigned int i = 0; i < _mgrt->m_NumAnimMeshes; i++)
			{
				MGRTAnimMeshData* tAnimMeshData = new MGRTAnimMeshData;
				CopyAnimMeshToAsset(_assimp->mAnimMeshes[i], tAnimMeshData);
				_mgrt->m_AnimMeshList.push_back(tAnimMeshData);
			});

		_mgrt->m_MorphMethod = ConvertTo_eAssetMorphingMethod(_assimp->mMethod);

		CopyAABBToAsset(&(_assimp->mAABB), &(_mgrt->m_AABB));

		///���ſ��� Exception�� ��������, ���� �ذ�!
		if (_assimp->mTextureCoordsNames != nullptr)
		{
			for (unsigned int i = 0; i < ASSET_MAX_NUMBER_OF_TEXTURECOORDS; i++)
			{
				if (_assimp->mTextureCoordsNames[i] != nullptr)
				{
					_mgrt->mTextureCoordsNameList[i] = (_assimp->mTextureCoordsNames[i])->C_Str();
				}
			}
		}

		///Ư�� ��� ���� ���� �Ǵ�.
		_mgrt->m_MeshHasTextureCoords = _assimp->HasTextureCoords(0);

		if (_mgrt->m_MeshHasTextureCoords)
		{
			_mgrt->m_MeshTexCoordEachCnt = _assimp->GetNumUVChannels();
		}
		_mgrt->m_MeshHasBones = _assimp->HasBones();
	}

	void AssetDataHelper::CopyVertexWeightToAsset(const aiVertexWeight* _assimp, AssetVertexWeightData* _mgrt)
	{
		//����.
		//_mgrt->m_VertexID = _assimp->mVertexId;
		//_mgrt->m_Weight = _assimp->mWeight;

		//������Ʈ ���� FBX Glitch �߻�. -> FIX �õ�!
		_mgrt->m_VertexID = _assimp->mVertexId;
		if (_assimp->mWeight < std::numeric_limits<float>::epsilon())
		{
			_mgrt->m_Weight = std::numeric_limits<float>::epsilon();
		}
		else
		{
			_mgrt->m_Weight = _assimp->mWeight;
		}
	}

	void AssetDataHelper::CopyBoneToAsset(const aiBone* _assimp, AssetMeshData* _mgrtMesh, AssetBoneData* _mgrt)
	{
		_mgrt->m_Name = _assimp->mName.C_Str();
		_mgrt->m_NumWeight = _assimp->mNumWeights;

		///���⼭ ���� ���� ������ �����ؾ�! 
		///�ϴ��� ASSIMP �������� �ٽ� �ѹ� ������ ���� ��������,
		///�� �� �ٽ� ���� ����� �ް�, ������ �ٽ� �����غ���.

		///���� �ߺ����� �ʵ��� �����ߴ�!
		IF_NOT_NULL(_assimp->mArmature,
			std::string tName = (_assimp->mArmature)->mName.C_Str();
		_mgrt->m_Armature = s_CurrentDataScene->FindNode(tName);
		);

		IF_NOT_NULL(_assimp->mNode,
			std::string tName = (_assimp->mNode)->mName.C_Str();
		_mgrt->m_Node = s_CurrentDataScene->FindNode(tName);
		);
		///

		_mgrt->m_WeightList.reserve(_mgrt->m_NumWeight);

		IF_NOT_NULL(_assimp->mWeights,
			for (unsigned int i = 0; i < _mgrt->m_NumWeight; i++)
			{
				AssetVertexWeightData tVertexWeight;
				CopyVertexWeightToAsset(&(_assimp->mWeights[i]), &tVertexWeight);
				_mgrt->m_WeightList.push_back(tVertexWeight);
			}
		);

		if (s_CurrentDataScene != nullptr)
		{
			s_CurrentDataScene->m_BonePlainContainer.insert(std::make_pair(_mgrt->m_Name, _mgrt));
		}

		CopyAssimpMatrixToAsset(&(_assimp->mOffsetMatrix), _mgrt->m_InverseBindPoseMatrix);
		/////[TRY] Matrix Animation�� ���� (+ ȣȯ), Transpose�ؼ� �޾ƿ´�.
		_mgrt->m_InverseBindPoseMatrix = _mgrt->m_InverseBindPoseMatrix.Transpose();

	}

	void AssetDataHelper::CopySkeletonBoneToAsset(const aiSkeletonBone* _assimp, AssetSkeletonBoneData* _mgrt)
	{
		_mgrt->m_ParentBoneIndex = _assimp->mParent;

		///���� �ߺ����� �ʵ��� �����ߴ�!
		IF_NOT_NULL(_assimp->mArmature,
			std::string tName = (_assimp->mArmature)->mName.C_Str();
		_mgrt->m_Armature = s_CurrentDataScene->FindNode(tName);
		);

		IF_NOT_NULL(_assimp->mNode,
			std::string tName = (_assimp->mNode)->mName.C_Str();
		_mgrt->m_Node = s_CurrentDataScene->FindNode(tName);
		);
		///

		_mgrt->m_NumnWeights = _assimp->mNumnWeights;

		///�굵 ����ϰ�, �߸��� �� ����. ������ؼ� ������ ã�� �Ʒ����� �긦 �־��.
		_mgrt->mMeshId = new AssetMeshData;
		CopyMeshToAsset(_assimp->mMeshId, _mgrt->mMeshId);

		_mgrt->m_WeightList.reserve(_mgrt->m_NumnWeights);
		for (unsigned int i = 0; i < _mgrt->m_NumnWeights; i++)
		{
			AssetVertexWeightData tVertexWeight;
			CopyVertexWeightToAsset(&(_assimp->mWeights[i]), &tVertexWeight);
			_mgrt->m_WeightList.push_back(tVertexWeight);
		}

		CopyAssimpMatrixToAsset(&(_assimp->mOffsetMatrix), _mgrt->m_InverseBindPoseMatrix);
		CopyAssimpMatrixToAsset(&(_assimp->mLocalMatrix), _mgrt->m_LocalMatrix);
	}

	void AssetDataHelper::CopySkeletonToAsset(const aiSkeleton* _assimp, AssetSkeletonData* _mgrt)
	{
		_mgrt->m_Name = _assimp->mName.C_Str();
		_mgrt->m_NumBone = _assimp->mNumBones;

		_mgrt->m_BoneList.reserve(_mgrt->m_NumBone);
		for (unsigned int i = 0; i < _mgrt->m_NumBone; i++)
		{
			AssetSkeletonBoneData* tSkeleBone = new AssetSkeletonBoneData;
			CopySkeletonBoneToAsset(_assimp->mBones[i], tSkeleBone);
			_mgrt->m_BoneList.push_back(tSkeleBone);
		}
	}

	///Metadata�� Scene, Node�� �ݵ�� ����. 
	///�̹� Scene, Node�� m_Name�� �ֱ⿡, Metadata�� �ļ�����!
	void AssetDataHelper::CopyMetadataEntryBoneToAsset(const aiMetadataEntry* _assimp, AssetMetaDataEntry* _mgrt)
	{


	}

	void AssetDataHelper::CopyMetadataToAsset(const aiMetadata* _assimp, AssetMetaData* _mgrt)
	{

	}
	///

	void AssetDataHelper::CopyAABBToAsset(const aiAABB* _assimp, AssetAABBData* _mgrt)
	{
		_mgrt->m_MaxVec = Vector3(_assimp->mMax.x, _assimp->mMax.y, _assimp->mMax.z);
		_mgrt->m_MinVec = Vector3(_assimp->mMin.x, _assimp->mMin.y, _assimp->mMin.z);
	}

	void AssetDataHelper::CopyVectorKeyToAsset(const aiVectorKey* _assimp, AssetVectorKeyData* _mgrt)
	{
		_mgrt->m_Time = _assimp->mTime;
		_mgrt->m_Value = Vector3(_assimp->mValue.x, _assimp->mValue.y, _assimp->mValue.z);

	}

	void AssetDataHelper::CopyQuatKeyToAsset(const aiQuatKey* _assimp, AssetQuatKeyData* _mgrt)
	{
		_mgrt->m_Time = _assimp->mTime;
		_mgrt->m_Value = Quaternion(_assimp->mValue.x, _assimp->mValue.y, _assimp->mValue.z, _assimp->mValue.w);

		/////Surgery
		//Quaternion tSwitch = XMQuaternionRotationRollPitchYaw(0.0f, XM_PI, 0.0f);
		//_mgrt->m_Value = XMQuaternionMultiply(_mgrt->m_Value, tSwitch);
		//_mgrt->m_Value = Quaternion(_mgrt->m_Value.x, _mgrt->m_Value.y, _mgrt->m_Value.z, _mgrt->m_Value.w);
		/////
	}

	void AssetDataHelper::CopyMeshKeyToAsset(const aiMeshKey* _assimp, AssetMeshKeyData* _mgrt)
	{
		_mgrt->m_Time = _assimp->mTime;
		_mgrt->m_Value = _assimp->mValue;
	}

	void AssetDataHelper::CopyMeshMorphKeyToAsset(const aiMeshMorphKey* _assimp, AssetMeshMorphKeyData* _mgrt)
	{
		_mgrt->m_Time = _assimp->mTime;
		_mgrt->m_NumValuesAndWeights = _assimp->mNumValuesAndWeights;

		_mgrt->m_ValueList.reserve(_mgrt->m_NumValuesAndWeights);
		_mgrt->mWeightList.reserve(_mgrt->m_NumValuesAndWeights);

		for (unsigned int i = 0; i < _mgrt->m_NumValuesAndWeights; i++)
		{
			_mgrt->m_ValueList.push_back(_assimp->mValues[i]);
			_mgrt->mWeightList.push_back(_assimp->mWeights[i]);
		}
	}

	void AssetDataHelper::CopyNodeAnimToAsset(const aiNodeAnim* _assimp, AssetNodeAnimData* _mgrt)
	{
		_mgrt->m_NodeName = _assimp->mNodeName.C_Str();

		_mgrt->m_NumPositionKey = _assimp->mNumPositionKeys;
		_mgrt->mPositionKeyList.reserve(_mgrt->m_NumPositionKey);
		for (unsigned int i = 0; i < _mgrt->m_NumPositionKey; i++)
		{
			AssetVectorKeyData tVecKeyData;
			CopyVectorKeyToAsset(&(_assimp->mPositionKeys[i]), &tVecKeyData);
			_mgrt->mPositionKeyList.push_back(tVecKeyData);
		}

		_mgrt->mNumRotationKey = _assimp->mNumRotationKeys;
		_mgrt->mRotationKeyList.reserve(_mgrt->mNumRotationKey);
		for (unsigned int i = 0; i < _mgrt->mNumRotationKey; i++)
		{
			AssetQuatKeyData tQuatKeyData;
			CopyQuatKeyToAsset(&(_assimp->mRotationKeys[i]), &tQuatKeyData);
			_mgrt->mRotationKeyList.push_back(tQuatKeyData);
		}

		_mgrt->mNumScalingKey = _assimp->mNumScalingKeys;
		_mgrt->mScalingKeyList.reserve(_mgrt->mNumScalingKey);
		for (unsigned int i = 0; i < _mgrt->mNumScalingKey; i++)
		{
			AssetVectorKeyData tVecKeyData;
			CopyVectorKeyToAsset(&(_assimp->mScalingKeys[i]), &tVecKeyData);
			_mgrt->mScalingKeyList.push_back(tVecKeyData);
		}

		_mgrt->m_PreState = ConvertTo_eAssetAnimBehaviour(_assimp->mPreState);
		_mgrt->m_PostState = ConvertTo_eAssetAnimBehaviour(_assimp->mPostState);
	}

	void AssetDataHelper::CopyMeshMorphAnimToAsset(const aiMeshMorphAnim* _assimp, AssetMeshMorphAnimData* _mgrt)
	{
		_mgrt->m_Name = _assimp->mName.C_Str();
		_mgrt->m_NumKey = _assimp->mNumKeys;

		_mgrt->m_KeyList.reserve(_mgrt->m_NumKey);

		IF_NOT_NULL(_assimp->mKeys,
			for (unsigned int i = 0; i < _mgrt->m_NumKey; i++)
			{
				AssetMeshMorphKeyData tKeyData;
				CopyMeshMorphKeyToAsset(&(_assimp->mKeys[i]), &tKeyData);
				_mgrt->m_KeyList.push_back(tKeyData);
			}
		);

	}

	void AssetDataHelper::CopyMeshAnimToAsset(const aiMeshAnim* _assimp, AssetMeshAnimData* _mgrt)
	{
		_mgrt->m_Name = _assimp->mName.C_Str();
		_mgrt->m_NumKeys = _assimp->mNumKeys;

		_mgrt->m_KeyList.reserve(_mgrt->m_NumKeys);

		IF_NOT_NULL(_assimp->mKeys,
			for (unsigned int i = 0; i < _mgrt->m_NumKeys; i++)
			{
				AssetMeshKeyData tData;
				CopyMeshKeyToAsset(&(_assimp->mKeys[i]), &tData);
				_mgrt->m_KeyList.push_back(tData);
			}
		);
	}

	void AssetDataHelper::CopyAnimationToAsset(const aiAnimation* _assimp, AssetAnimationData* _mgrt)
	{
		_mgrt->m_Name = _assimp->mName.C_Str();
		_mgrt->m_Duration = _assimp->mDuration;

		//�� ���� ������ �����ϴ�, ���� ����Ʈ�� TicksPerSecond�� �Ҵ��ϰ�!
		if (_assimp->mTicksPerSecond <= std::numeric_limits<double>::epsilon())
		{
			_mgrt->m_TicksPerSecond = 60;
		}
		else
		{
			_mgrt->m_TicksPerSecond = _assimp->mTicksPerSecond;
		}

		_mgrt->m_NumChannel = _assimp->mNumChannels;
		_mgrt->m_ChannelList.reserve(_mgrt->m_NumChannel);
		for (unsigned int i = 0; i < _mgrt->m_NumChannel; i++)
		{
			AssetNodeAnimData* tAnimData = new AssetNodeAnimData;
			tAnimData = new AssetNodeAnimData; //tAnimData = new AssetNodeAnimData; tAnimData = new AssetNodeAnimData;

			CopyNodeAnimToAsset(_assimp->mChannels[i], tAnimData);
			_mgrt->m_ChannelList.push_back(tAnimData);
		}

		_mgrt->m_NumMeshChannel = _assimp->mNumMeshChannels;
		_mgrt->m_MeshChannelList.reserve(_mgrt->m_NumMeshChannel);
		for (unsigned int i = 0; i < _mgrt->m_NumMeshChannel; i++)
		{
			AssetMeshAnimData* tAnimData = new AssetMeshAnimData;
			CopyMeshAnimToAsset(_assimp->mMeshChannels[i], tAnimData);
			_mgrt->m_MeshChannelList.push_back(tAnimData);
		}

		_mgrt->m_NumMorphMeshChannel = _assimp->mNumMorphMeshChannels;
		_mgrt->mMorphMeshChannelList.reserve(_mgrt->m_NumMorphMeshChannel);
		for (unsigned int i = 0; i < _mgrt->m_NumMorphMeshChannel; i++)
		{
			AssetMeshMorphAnimData* tAnimData = new AssetMeshMorphAnimData;
			CopyMeshMorphAnimToAsset(_assimp->mMorphMeshChannels[i], tAnimData);
			_mgrt->mMorphMeshChannelList.push_back(tAnimData);
		}
	}

	void AssetDataHelper::CopyTextureToAsset(const aiTexture* _assimp, AssetTextureData* _mgrt)
	{
		_mgrt->m_Width = _assimp->mWidth;
		_mgrt->m_Height = _assimp->mHeight;

		for (int i = 0; i < ASSET_HINTMAXTEXTURELEN; i++)
		{
			_mgrt->achFormatHint[i] = _assimp->achFormatHint[i];
		}

		size_t tReserveSize = 0;
		if (_mgrt->m_Height <= 0)
		{
			tReserveSize = _mgrt->m_Width;
		}
		else
		{
			tReserveSize = _mgrt->m_Width * _mgrt->m_Height;
		}

		//_mgrt->m_pcDataList.reserve(tReserveSize);
		unsigned int tReserveCount = static_cast<UINT>(ceil((float)tReserveSize / (float)(sizeof(aiTexel))));


		///[TW] ���� for���� ������ �޸𸮰� ���������� �̷���� ���̴�, ó���� ���� �����ϸ� �� ��.
		for (size_t i = 0; i < tReserveCount; i++)
		{

			AssetTexelData tTexelData;
			///[TW] CopyTexel �Լ���, ���⼭ �ٷ� �����Ѵ�.
			unsigned char tR = _assimp->pcData[i].r;
			unsigned char tG = _assimp->pcData[i].g;
			unsigned char tB = _assimp->pcData[i].b;
			unsigned char tA = _assimp->pcData[i].a;

			tTexelData.a = tA;
			tTexelData.r = tR;
			tTexelData.g = tG;
			tTexelData.b = tB;

			_mgrt->m_pcDataList.push_back(tTexelData);
		}

		_mgrt->m_FileName = _assimp->mFilename.C_Str();
	}

#pragma region OldMaterialCodeComments
	///Material�� �о������ �ߴ� ���� �ڵ�, ������ ��θ� �׳� �޾ƿ��� ������ �ٲ����.
	/*
	void AssetDataHelper::CopyMaterialToAsset(const aiMaterial* _assimp, AssetMaterialData* _mgrt)
	{
		typedef std::tuple<std::string, eAssetTextureMapping, unsigned int, float, eAssetTextureOp, eAssetTextureMapMode> MGRT_TexturePropTuple;

		_mgrt->m_NumAllocated = _assimp->mNumAllocated;
		_mgrt->m_NumProperty = _assimp->mNumProperties;

		for (unsigned int i = 0; i < _mgrt->m_NumProperty; i++)
		{
			AssetMaterialPropertyData* tMatProp = new AssetMaterialPropertyData;
			CopyMatPropertyToAsset(i, _assimp, _assimp->mProperties[i], tMatProp);
			_mgrt->m_PropertyList.push_back(tMatProp);
		}

		for (size_t i = 0; i <= ASSET_MAXIMUM_TEXTURE_PROP_CNT; i++)
		{
			eAssetTextureType tTexType = static_cast<eAssetTextureType>(i);

			int tTexTypeTexCnt = _assimp->GetTextureCount((aiTextureType)tTexType);
			if (tTexTypeTexCnt <= 0)
			{
				std::vector<MGRT_TexturePropTuple> tVec;
				tVec.clear();
				_mgrt->m_TexturePropContainer.insert(std::make_pair(tTexType, tVec));
			}
			else
			{
				std::vector<MGRT_TexturePropTuple> tVec;
				for (size_t j = 0; j < tTexTypeTexCnt; j++)
				{
					aiString tPathStr;
					aiTextureMapping tTexMapping;
					unsigned int tUVIndex;
					float tBlend;
					aiTextureOp tTexOp;
					aiTextureMapMode tTexMapMode;
					///[Problem] Embedded Texture, �Ϲ� ��� �����ؾ� �Ѵ�.
					_assimp->GetTexture((aiTextureType)tTexType, j, &tPathStr);
					std::string tPathStringCPP = tPathStr.C_Str();
					assert(&tPathStringCPP);

					//_assimp->GetTexture((aiTextureType)tTexType, (UINT)j, &tPathStr, &tTexMapping, &tUVIndex, &tBlend, &tTexOp, &tTexMapMode);
					//
					//std::string tPathStringCPP = tPathStr.C_Str();
					//tVec.push_back(std::make_tuple(tPathStringCPP, ConvertTo_eAssetTextureMapping(tTexMapping), tUVIndex,
					//	tBlend, ConvertTo_eAssetTextureOp(tTexOp), ConvertTo_eAssetTextureMode(tTexMapMode)));
				}
				_mgrt->m_TexturePropContainer.insert(std::make_pair(tTexType, tVec));
			}
		}
	}
	void AssetDataHelper::CopyMatPropertyToAsset(unsigned int index, const aiMaterial* _assimpMat, const aiMaterialProperty* _assimp, AssetMaterialPropertyData* _mgrt)
	{
		_mgrt->m_Key = _assimp->mKey.C_Str();
		_mgrt->m_Semantic = ConvertTo_eAssetTextureType(_assimp->mSemantic);
		_mgrt->m_Index = _assimp->mIndex;
		_mgrt->m_DataLength = _assimp->mDataLength;
		_mgrt->m_Type = ConvertTo_eAssetMatPropertyType(_assimp->mType);
		//char�� 1Byte�̴�, �� ��Ĵ�� �����ص� ������ �߻����� �ʴ´�.
		switch (_mgrt->m_Type)
		{
			///[Problem] �̰͵� ������ ���� ��... ���� ������ ������, �� ���� ���̴�!
			case MGRT_PTI_Float:
			{
				ai_real tVal = NULL;
				_assimpMat->Get(_assimp->mKey.C_Str(), (unsigned int)_assimp->mType, 0, tVal);
				_mgrt->m_DataSplit.m_FloatData = tVal;
			}
			break;
			case MGRT_PTI_Double:
			{
				double tVal = NULL;
				_assimpMat->Get(_assimp->mKey.C_Str(), (unsigned int)_assimp->mType, 0, tVal);
				_mgrt->m_DataSplit.m_DoubleData = tVal;
			}
			break;
			case MGRT_PTI_String:
			{
				aiString tVal;
				_assimpMat->Get(_assimp->mKey.C_Str(), (unsigned int)_assimp->mType, 0, tVal);
				std::string tValStr = tVal.C_Str();
				_mgrt->m_DataSplit.m_StringData = tValStr;
			}
			break;
			case MGRT_PTI_Integer:
			{
				int tVal = NULL;
				_assimpMat->Get(_assimp->mKey.C_Str(), (unsigned int)_assimp->mType, 0, tVal);
				_mgrt->m_DataSplit.m_IntData = tVal;
			}
			break;
			default:
			{
				//������� �Դٴ� ���� ������ ������ Binary Buffer�� �����ߴٴ� ���.
				_mgrt->m_ExceptionBufferData = new char[_mgrt->m_DataLength];
				memcpy(_mgrt->m_ExceptionBufferData, _assimp->mData, _mgrt->m_DataLength);
			}
			break;
		}
	}
	*/
	//Material���� Texture�� ������ ����.
	/*
	std::vector<AssetTextureSRV> AssetDataHelper::GetTexturesFromMaterial(aiMaterial* mat, aiTextureType type, eAssetTextureType typeName, const aiScene* scene)
	{
		std::vector<AssetTextureSRV> textures;
		for (UINT i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (UINT j = 0; j < s_CurrentDataScene->m_ATSMap.size(); j++)
			{
				if (std::strcmp(s_CurrentDataScene->m_ATSMap.at(static_cast<eAssetTextureType>(j)).path.c_str(), str.C_Str()) == 0) {
					textures.push_back(s_CurrentDataScene->m_ATSMap.at(static_cast<eAssetTextureType>(j)));
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip) {   // If texture hasn't been loaded already, load it
				HRESULT hr;
				AssetTextureSRV textureSRV;

				const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
				if (embeddedTexture != nullptr)
				{
					///[Structure Problem] : �� �� ������ �ڵ��� ���ռ��� ���� D3D�� Assimp�� ��ν��Ѿ� �Ѵٴ�...
					textureSRV.texture = LoadEmbeddedTextureFromMaterial(embeddedTexture);
				}
				else {
					std::string filename = std::string(str.C_Str());
					filename = s_CurrentDataScene->m_Directory + '/' + filename;
					std::wstring filenamews = std::wstring(filename.begin(), filename.end());
					hr = CreateWICTextureFromFile(LowLevelDX11::GetInstance()->m_d3dDevice.Get(),
						LowLevelDX11::GetInstance()->m_d3dImmediateContext.Get(),
						filenamews.c_str(), nullptr, &textureSRV.texture);
					if (FAILED(hr))
						MessageBox(LowLevelDX11::GetInstance()->m_hWnd, L"3D�� ���� Material SRV ���� �� ���� �߻�.", L"����...", MB_ICONERROR | MB_OK);
				}
				textureSRV.type = typeName;
				textureSRV.path = str.C_Str();
				textures.push_back(textureSRV);
				s_CurrentDataScene->m_ATSMap.insert(std::make_pair(typeName, textureSRV));  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}

	ID3D11ShaderResourceView* AssetDataHelper::LoadEmbeddedTextureFromMaterial(const aiTexture* _assimp)
	{
		HRESULT hr;
		ID3D11ShaderResourceView* texture = nullptr;

		if (_assimp->mHeight != 0)
		{
			// Load an uncompressed ARGB8888 embedded texture
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = _assimp->mWidth;
			desc.Height = _assimp->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA subresourceData;
			subresourceData.pSysMem = _assimp->pcData;
			subresourceData.SysMemPitch = _assimp->mWidth * 4;
			subresourceData.SysMemSlicePitch = _assimp->mWidth * _assimp->mHeight * 4;

			ID3D11Texture2D* texture2D = nullptr;
			hr = LowLevelDX11::GetInstance()->m_d3dDevice.Get()->CreateTexture2D(&desc, &subresourceData, &texture2D);
			if (FAILED(hr))
				MessageBox(LowLevelDX11::GetInstance()->m_hWnd, L"�Ӻ���� �ؽ��� �ε� �ȿ���, CreateTexture2D ����!", L"����", MB_ICONERROR | MB_OK);

			hr = LowLevelDX11::GetInstance()->m_d3dDevice.Get()->CreateShaderResourceView(texture2D, nullptr, &texture);
			if (FAILED(hr))
				MessageBox(LowLevelDX11::GetInstance()->m_hWnd, L"�Ӻ���� �ؽ��� �ε� �ȿ���, CreateShaderResourceView ����", L"����", MB_ICONERROR | MB_OK);

			return texture;
		}
	}
	*/
#pragma endregion OldMaterialCodeComments

	RocketCore::Graphics::eAssetMetaDataType AssetDataHelper::ConvertTo_eAssetMetaDataType(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetMetaDataType>(_assimp);
	}

	RocketCore::Graphics::eAssetMorphingMethod AssetDataHelper::ConvertTo_eAssetMorphingMethod(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetMorphingMethod>(_assimp);
	}

	RocketCore::Graphics::eAssetAnimBehaviour AssetDataHelper::ConvertTo_eAssetAnimBehaviour(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetAnimBehaviour>(_assimp);
	}

	RocketCore::Graphics::eAssetPropertyTypeInfoData AssetDataHelper::ConvertTo_eAssetMatPropertyType(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetPropertyTypeInfoData>(_assimp);
	}

	RocketCore::Graphics::eAssetTextureType AssetDataHelper::ConvertTo_eAssetTextureType(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetTextureType>(_assimp);
	}

	RocketCore::Graphics::eAssetShadingMode AssetDataHelper::ConvertTo_eAssetShadingMode(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetShadingMode>(_assimp);
	}

	RocketCore::Graphics::eAssetTextureFlags AssetDataHelper::ConvertTo_eAssetTextureFlags(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetTextureFlags>(_assimp);
	}

	RocketCore::Graphics::eAssetBlendMode AssetDataHelper::ConvertTo_eAssetBlendMode(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetBlendMode>(_assimp);
	}

	RocketCore::Graphics::eAssetTextureMapping AssetDataHelper::ConvertTo_eAssetTextureMapping(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetTextureMapping>(_assimp);
	}

	RocketCore::Graphics::eAssetTextureOp AssetDataHelper::ConvertTo_eAssetTextureOp(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetTextureOp>(_assimp);
	}

	RocketCore::Graphics::eAssetTextureMapMode AssetDataHelper::ConvertTo_eAssetTextureMode(int _assimp)
	{
		return static_cast<RocketCore::Graphics::eAssetTextureMapMode>(_assimp);
	}

	void AssetDataHelper::CopyAssimpMatrixToAsset(const aiMatrix4x4* _assimp, DirectX::SimpleMath::Matrix& _mgrt)
	{
		_mgrt._11 = _assimp->a1;
		_mgrt._12 = _assimp->a2;
		_mgrt._13 = _assimp->a3;
		_mgrt._14 = _assimp->a4;
		_mgrt._21 = _assimp->b1;
		_mgrt._22 = _assimp->b2;
		_mgrt._23 = _assimp->b3;
		_mgrt._24 = _assimp->b4;
		_mgrt._31 = _assimp->c1;
		_mgrt._32 = _assimp->c2;
		_mgrt._33 = _assimp->c3;
		_mgrt._34 = _assimp->c4;
		_mgrt._41 = _assimp->d1;
		_mgrt._42 = _assimp->d2;
		_mgrt._43 = _assimp->d3;
		_mgrt._44 = _assimp->d4;
	}

	void AssetDataHelper::CopyAssimpToRenderAnimation(const aiScene* _assimpScene, RenderAnimation* _renderAnimation)
	{
		//���� ���� �ƴϴ�. ������ 1 FBX = 1 Animation ���·� ���õǾ� ���� ��
		_renderAnimation->_assetAnimSceneData = std::make_unique<AssetSceneData>();
		assert(_assimpScene->mAnimations[0] != nullptr);
		CopyAssimpToAssetScene(_assimpScene, _renderAnimation->_assetAnimSceneData.get());
		assert(true);
	}






}