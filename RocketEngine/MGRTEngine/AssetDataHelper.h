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

//NULL이 아닐 때만 값을 시행하는 Macro 함수.
#define IF_NOT_NULL(_ptr, _action) \
    do { \
        if (_ptr != nullptr) { \
            _action; \
        } \
    } while (false)

//value가 0이 아닐 때만 값을 시행하는 Macro 함수.
#define IF_NOT_ZERO(_val, _action) \
    do { \
        if (_val != 0) { \
            _action; \
        } \
    } while (false)

//DataHelper 클래스! (3D Model Data를 처리/복사 편하게 하기 위해)
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
		//ASSIMP 구조체를 Asset 구조체로 변화시킨다.
		static void CopyAssimpToAssetScene(const aiScene* _assimpScene, AssetSceneData* _mgrtScene);

		static void CopyAssimpToRenderAnimation(const aiScene* _assimpScene, RenderAnimation* _renderAnimation);

		
		//복제 과정에서 더 이상 중복은 발생X. (FindNode 덕분)
		
		//SceneData를 Static으로 임시 보관 멤버변수 초기화.
		static void FinalizeDataHelper();
	private:
		//CopyAssimpToMGRTScene를 하면서,
		//Scene에 있는 정보를 세부 Node들이 가져와야 할 때, etc..를 위해 현재 사용하는 MGRTSceneData* 저장.
		static AssetSceneData* s_CurrentDataScene;

		///ASSIMP 클래스 -> Asset 데이터로 변환.
		///메모리 관리: 상황 보고, 알아서 넣자. 
		/// 일괄적으로 처리하기 어려운 상황이다.
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

		///Enum들 처리용. 모든 Enum의 Int 할당 수를 바꾸지 않았지만, Int만을 가지고 표출 가능.
		///Enum이니, Int 캐스팅해서 넣고 사용해서!
		/// (ASSIMP의 Enum들에 1대1 대응)
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

		///Animation 전용으로 새로운 구조체 만들어서, 자료 옮기기.


	};
}





