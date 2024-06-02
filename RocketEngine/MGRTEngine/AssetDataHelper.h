#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>

#include <DirectXMath.h>
#include "AssetDataDefine.h"
#include "assimp/matrix4x4.h"

//NULL�� �ƴ� ���� ���� �����ϴ� Macro �Լ�.
#define IF_NOT_NULL(_ptr, _action) \
    do { \
        if (_ptr != nullptr) { \
            _action; \
        } \
    } while (false)

//value�� 0�� �ƴ� ���� ���� �����ϴ� Macro �Լ�.
#define IF_NOT_ZERO(_val, _action) \
    do { \
        if (_val != 0) { \
            _action; \
        } \
    } while (false)

//DataHelper Ŭ����! (3D Model Data�� ó��/���� ���ϰ� �ϱ� ����)
struct aiScene;
struct aiNode;
struct aiFace;
struct aiAnimMesh;
struct aiMesh;
struct aiVertexWeight;
struct aiBone;
struct aiSkeletonBone;
struct aiSkeleton;
struct aiMetadataEntry;
struct aiMetadata;
struct aiAABB;
struct aiVectorKey;
struct aiQuatKey;
struct aiMeshKey;
struct aiMeshMorphKey;
struct aiNodeAnim;
struct aiMeshMorphAnim;
struct aiMeshAnim;
struct aiAnimation;
struct aiTexel;
struct aiTexture;

namespace RocketCore::Graphics
{
	class AssetSceneData;
	class RenderAnimation;

	class AssetDataHelper
	{
	public:
		//ASSIMP ����ü�� Asset ����ü�� ��ȭ��Ų��.
		static void CopyAssimpToAssetScene(const aiScene* _assimpScene, AssetSceneData* _mgrtScene);

		static void CopyAssimpToRenderAnimation(const aiScene* _assimpScene, RenderAnimation* _renderAnimation);

		
		//���� �������� �� �̻� �ߺ��� �߻�X. (FindNode ����)
		
		//SceneData�� Static���� �ӽ� ���� ������� �ʱ�ȭ.
		static void FinalizeDataHelper();
	private:
		//CopyAssimpToMGRTScene�� �ϸ鼭,
		//Scene�� �ִ� ������ ���� Node���� �����;� �� ��, etc..�� ���� ���� ����ϴ� MGRTSceneData* ����.
		static AssetSceneData* s_CurrentDataScene;

		///ASSIMP Ŭ���� -> Asset �����ͷ� ��ȯ.
		///�޸� ����: ��Ȳ ����, �˾Ƽ� ����. 
		/// �ϰ������� ó���ϱ� ����� ��Ȳ�̴�.
		static void CopyRootNodeToAsset(const aiNode* _assimp, AssetNodeData* _mgrt);
		static void CopyRestNodeToAsset(const aiNode* _assimp, AssetNodeData* _mgrt);

		static void CopyFaceToAsset(const aiFace* _assimp, AssetFaceData* _mgrt);
		static void CopyAnimMeshToAsset(const aiAnimMesh* _assimp, MGRTAnimMeshData* _mgrt);
		static void CopyMeshToAsset(const aiMesh* _assimp, AssetMeshData* _mgrt);
		static void CopyVertexWeightToAsset(const aiVertexWeight* _assimp, AssetVertexWeightData* _mgrt);
		static void CopyBoneToAsset(const aiBone* _assimp, AssetMeshData* _mgrtMesh, AssetBoneData* _mgrt);
		static void CopySkeletonBoneToAsset(const aiSkeletonBone* _assimp, AssetSkeletonBoneData* _mgrt);
		static void CopySkeletonToAsset(const aiSkeleton* _assimp, AssetSkeletonData* _mgrt);
		static void CopyMetadataEntryBoneToAsset(const aiMetadataEntry* _assimp, AssetMetaDataEntry* _mgrt);
		static void CopyMetadataToAsset(const aiMetadata* _assimp, AssetMetaData* _mgrt);
		static void CopyAABBToAsset(const aiAABB* _assimp, AssetAABBData* _mgrt);
		static void CopyVectorKeyToAsset(const aiVectorKey* _assimp, AssetVectorKeyData* _mgrt);
		static void CopyQuatKeyToAsset(const aiQuatKey* _assimp, AssetQuatKeyData* _mgrt);
		static void CopyMeshKeyToAsset(const aiMeshKey* _assimp, AssetMeshKeyData* _mgrt);
		static void CopyMeshMorphKeyToAsset(const aiMeshMorphKey* _assimp, AssetMeshMorphKeyData* _mgrt);
		static void CopyNodeAnimToAsset(const aiNodeAnim* _assimp, AssetNodeAnimData* _mgrt);
		static void CopyMeshMorphAnimToAsset(const aiMeshMorphAnim* _assimp, AssetMeshMorphAnimData* _mgrt);
		static void CopyMeshAnimToAsset(const aiMeshAnim* _assimp, AssetMeshAnimData* _mgrt);
		static void CopyAnimationToAsset(const aiAnimation* _assimp, AssetAnimationData* _mgrt);
		
		static void CopyTextureToAsset(const aiTexture* _assimp, AssetTextureData* _mgrt);

		///Enum�� ó����. ��� Enum�� Int �Ҵ� ���� �ٲ��� �ʾ�����, Int���� ������ ǥ�� ����.
		///Enum�̴�, Int ĳ�����ؼ� �ְ� ����ؼ�!
		/// (ASSIMP�� Enum�鿡 1��1 ����)
		static eAssetMetaDataType ConvertTo_eAssetMetaDataType(int _assimp);
		static eAssetMorphingMethod ConvertTo_eAssetMorphingMethod(int _assimp);
		static eAssetAnimBehaviour ConvertTo_eAssetAnimBehaviour(int _assimp);
		static eAssetPropertyTypeInfoData ConvertTo_eAssetMatPropertyType(int _assimp);

		static eAssetTextureType ConvertTo_eAssetTextureType(int _assimp);
		static eAssetShadingMode ConvertTo_eAssetShadingMode(int _assimp);
		static eAssetTextureFlags ConvertTo_eAssetTextureFlags(int _assimp);
		static eAssetBlendMode ConvertTo_eAssetBlendMode(int _assimp);

		static eAssetTextureMapping ConvertTo_eAssetTextureMapping(int _assimp);
		static eAssetTextureOp ConvertTo_eAssetTextureOp(int _assimp);
		static eAssetTextureMapMode ConvertTo_eAssetTextureMode(int _assimp);

		///OtherHelpers
		static void CopyAssimpMatrixToAsset(const aiMatrix4x4* _assimp, DirectX::SimpleMath::Matrix& _mgrt);

		///Animation �������� ���ο� ����ü ����, �ڷ� �ű��.


	};
}





