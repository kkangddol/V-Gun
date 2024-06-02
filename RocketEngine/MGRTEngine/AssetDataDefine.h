#pragma once
///장기적으로 3D Modeling 활용을 기반으로, 커스텀 구조체 복사 노가다 시작. (ASSIMP 대부분 1대1 대응.)
///그래픽스 디버거에서 C식 이중 배열을 사용하기도 힘들고.
///ASSIMP식 ai구조체 -> MGRT 구조체로 옮겨 담아서 진행할 것이다.
/// 모델 로딩, 데이터 처리 과정 배울 수 있기도 하고.
/// 
/// 메모리 누수 안나게 특히 조심해라! <중요>
#include <vector>
#include <string>
#include <variant>
#include <tuple>
#include <map>
#include "SimpleMath.h"
#include <array>
#include "RenderUsageMesh.h"
#include "TypedefGraphicHelper.h"

namespace RocketCore::Graphics
{
	enum class eModelMeshType
	{
		NONE,
		MESH,
		SKINNEDMESH
	};

	const unsigned int ASSET_MAX_NUMBER_OF_COLOR_SETS = 0x8;
	const unsigned int ASSET_MAX_NUMBER_OF_TEXTURECOORDS = 0x8;
	const unsigned int ASSET_HINTMAXTEXTURELEN = 9;
	const unsigned int ASSET_MAXIMUM_TEXTURE_PROP_CNT = 21;

	class AssetNodeData;
	class AssetFaceData;
	class AssetMeshData; //이걸 진짜로 쓰자. MGRTMesh말고 이걸로!
	class AssetTextureData;
	class AssetMaterialData;
	class AssetAnimationData;
	class AssetMetaData;
	class AssetSkeletonData;
	class AssetBoneData;
	class AssetAABBData;

#pragma region MGRT_Metadata,Etc.
	//enum 사이 호환 -> 단순히 enum이기에 숫자로 캐스팅 이동 가능.
	enum eAssetMetaDataType
	{
		MGRT_BOOL = 0,
		MGRT_INT32 = 1,
		MGRT_UINT64 = 2,
		MGRT_FLOAT = 3,
		MGRT_DOUBLE = 4,
		MGRT_STRING = 5,
		MGRT_VECTOR3 = 6,
		MGRT_METADATA = 7,
		MGRT_INT64 = 8,
		MGRT_UINT32 = 9,
		MGRT_META_MAX = 10,

#ifndef SWIG
		FORCE_32BIT = INT_MAX
#endif //4바이트 처리를 위해.
	};

	inline eAssetMetaDataType GetAiType(bool) {
		return MGRT_BOOL;
	}
	inline eAssetMetaDataType GetAiType(int32_t) {
		return MGRT_INT32;
	}
	inline eAssetMetaDataType GetAiType(uint64_t) {
		return MGRT_UINT64;
	}
	inline eAssetMetaDataType GetAiType(float) {
		return MGRT_FLOAT;
	}
	inline eAssetMetaDataType GetAiType(double) {
		return MGRT_DOUBLE;
	}
	inline eAssetMetaDataType GetAiType(const std::string&) {
		return MGRT_STRING;
	}
	inline eAssetMetaDataType GetAiType(const DirectX::SimpleMath::Vector3&) {
		return MGRT_VECTOR3;
	}
	inline eAssetMetaDataType GetAiType(const AssetMetaData&) {
		return MGRT_METADATA;
	}
	inline eAssetMetaDataType GetAiType(int64_t) {
		return MGRT_INT64;
	}
	inline eAssetMetaDataType GetAiType(uint32_t) {
		return MGRT_UINT32;
	}

	///여기에는 void*, 다른 자료형을 기반으로 메모리의 양을 할당하고, 메모리를 가져오는 ASSIMP 고유 로직이 들어 있다.
	///나중에 옮길때 필요.
	class AssetMetaDataEntry
	{
	public:
		eAssetMetaDataType mType;
		void* mData;
	};

	class AssetMetaData
	{
	public:
		/** Length of the mKeys and mValues arrays, respectively */
		unsigned int m_NumProperties; //KeyValue Map 전체 사이즈.

		/** Arrays of keys, may not be NULL. Entries in this array may not be NULL as well. */
		/** Arrays of values, may not be NULL. Entries in this array may be NULL if the
		  * corresponding property key has no assigned value. */
		std::map<std::string*, AssetMetaDataEntry*> m_KeyValuePairs;
	};

	class AssetAABBData
	{
	public:
		DirectX::SimpleMath::Vector3 m_MinVec;
		DirectX::SimpleMath::Vector3 m_MaxVec;
	};
#pragma endregion MGRT_Metadata,Etc.




#pragma region MGRT_Node&Face
	class AssetNodeData
	{
	public:
		AssetNodeData(AssetNodeData* _parentNode);
		~AssetNodeData();

		std::string m_NodeName;
		DirectX::SimpleMath::Matrix m_ParRelativeTransform;
		AssetNodeData* m_Parent = nullptr;
		unsigned int m_NumChildren; //해당 Node의 Children 개수.
		std::vector<AssetNodeData*> m_ChildrenList; //이 Node의 Children Node들. (자식 노드 없으면 nullptr)
		unsigned int m_NumMesh; //해당 Node의 Mesh 개수.
		std::vector<unsigned int> m_MeshIndex; //Mesh Index 저장. (각자 aiScene의 MeshList에 대응)
		AssetMetaData* m_MetaData = nullptr; //Metadata 있으면, 저장.
	};

	class AssetFaceData
	{
	public:
		enum {DEFAULT_INDEX_COUNT = 3};

		unsigned int m_NumIndice; //해당 Face 안을 차지하고 있는 Index 수. (삼각형이면 3개)
		std::vector<unsigned int> m_IndiceList; //Indice 실제로 보관.
	};
#pragma endregion MGRT_Node&Face

#pragma region MGRT_Mesh

	class MGRTAnimMeshData
	{
	public:
		std::string m_Name;
		std::vector<DirectX::SimpleMath::Vector3> m_VerticeList; //얘가 Vertex Position 대응.
		std::vector<DirectX::SimpleMath::Vector3> m_NormalList;
		std::vector<DirectX::SimpleMath::Vector3> m_TangentList;
		std::vector<DirectX::SimpleMath::Vector3> m_BiTangentList;
		std::vector<DirectX::SimpleMath::Color> m_ColorList; //Color 정보 보관.
		std::vector<DirectX::SimpleMath::Vector3> m_TextureCoordList; //Texture Coord 정보.

		unsigned int m_NumVertice;
		float m_Weight;

		///특정 요소들을 가지고 있는지 판단 위해.
		bool m_MeshHasTextureCoords = false; //TextureCoord 적어도 하나 있음?
		//Color 기반은 쓰이지 않는다.
	};

	////AssetMeshData에서 다른 버텍스를 가져오는 것처럼 접근할 수 있게!
	//class CustomVertexBoneData
	//{
	//public:
	//	CustomVertexBoneData();
	//	unsigned int IDs[4]; //!< An array of 4 bone Ids that influence a single vertex.
	//	float Weights[4]; //!< An array of the weight influence per bone. 
	//};

	class AssetMeshData
	{
	public:
		unsigned int m_PrimitiveTypes; //Primitive Type 여러 개로 구성? Bitwise로 명시
		unsigned int m_NumVertice; //해당 Mesh 안에서 총 Vertex 개수.
		unsigned int m_NumFace; //해당 Mesh 안에서 총 Face 개수. 
		std::vector<DirectX::SimpleMath::Vector3> m_VerticeList; //Vertices 보관
		std::vector<DirectX::SimpleMath::Vector3> m_NormalList; //Normal 보관
		std::vector<DirectX::SimpleMath::Vector3> m_TangentList; //Tangent 보관.
		std::vector<DirectX::SimpleMath::Vector3> m_BiTangentList; //Bitangent 보관.

		///원본은 8개까지의 aiColor4D들의 배열 (C식 포인터)지만, 첫 번째 것만 저장하자.
		///Color값은 지원을 그냥 하지 않기로 한다. 너무 낡음! 호환 이득 <<<< 노력.
		std::vector<DirectX::SimpleMath::Color> m_ColorList; //Color 정보 보관.
		std::vector<DirectX::SimpleMath::Vector3> m_TextureCoordList; //Texture Coord 정보.

		///Vertex 별로 자료를 넣기 위해, Assimp 외적으로 처리할 정보.
		//std::vector<CustomVertexBoneData> m_CustomPerVertexBlendList;

		unsigned int m_NumUVComponent[8]; //UV(W), 그러니 2개 or 3개.
		std::vector<AssetFaceData> m_FaceList; //Face들.
		unsigned int m_NumBone; //Bone들의 개수.
		std::vector<AssetBoneData*> m_BoneList; //Bone 보관.
		unsigned int m_MaterialIndex; //Material 받아서 보관.
		std::string m_Name; //이름 저장.
		unsigned int m_NumAnimMeshes; //AnimMesh들의 개수
		std::vector<MGRTAnimMeshData*> m_AnimMeshList; //AnimMesh들 보관.
		enum eAssetMorphingMethod m_MorphMethod; //Morph하는 Method Enum.
		AssetAABBData m_AABB; //AABB (Bounding Box) 보관.

		//Vertex UV stream names. Pointer to array of size AI_MAX_NUMBER_OF_TEXTURECOORDS
		//가 원본인데, 나는 그냥 개별적으로 자료를 저장하는 것으로 바꿨다.
		std::array<std::string, ASSET_MAX_NUMBER_OF_TEXTURECOORDS> mTextureCoordsNameList; //Vertex UV Stream 이름. 

		///특정 요소들을 가지고 있는지 판단 위해.
		bool m_MeshHasTextureCoords = false; //TextureCoord 적어도 하나 있음?
		int m_MeshTexCoordEachCnt = 0; // Texture UV Coord이 몇 개인가? (여러 개 가능)
		bool m_MeshHasBones = false;

		//Color 기반은 쓰이지 않는다.

	};

#pragma endregion MGRT_Mesh

#pragma region MGRT_Skeleton&Bones

	class AssetVertexWeightData
	{
	public:
		unsigned int m_VertexID; //Bone에 의해 영향 받는 Vertex의 인덱스.
		float m_Weight; //0~1 사이 실제 가중치 (float)
	};

	class AssetBoneData
	{
	public:
		AssetBoneData();
		~AssetBoneData();

		std::string m_Name; //Bone 이름.
		unsigned int m_NumWeight; // 이 Bone에 의해 영향 받는 Vertex 개수.
		AssetNodeData* m_Armature = nullptr; //Bone Armature Node : Skeleton Conversion을 위해.
		AssetNodeData* m_Node = nullptr; //Bone Node : Skeleton Conversion을 위해.
		std::vector<AssetVertexWeightData> m_WeightList; //버텍스 인덱스에 따라서 영향을 주는 가중치들.
		DirectX::SimpleMath::Matrix m_InverseBindPoseMatrix; //Mesh Space->Bone Space로 바꿔주는 Inverse Bind Pose Matrix.
	};

	class AssetSkeletonBoneData
	{
	public:
		AssetSkeletonBoneData();
		~AssetSkeletonBoneData();

		int m_ParentBoneIndex; //Parent Bone Index.
		AssetNodeData* m_Armature = nullptr; //Armature
		AssetNodeData* m_Node = nullptr; //Node
		unsigned int m_NumnWeights; //가중치 개수.
		AssetMeshData* mMeshId = nullptr; //가중치 영향 받을 Mesh.
		std::vector<AssetVertexWeightData> m_WeightList; //해당 Bone에 영향을 가중치들.
		DirectX::SimpleMath::Matrix m_InverseBindPoseMatrix; //InverseBindPoseMatrix. (Bone Space -> Mesh Space로 바꿔줌)
		DirectX::SimpleMath::Matrix m_LocalMatrix; //Bone의 Local Matrix.
	};

	class AssetSkeletonData
	{
	public:
		std::string m_Name; //Skeleton 이름.
		unsigned int m_NumBone; //뼈 개수
		std::vector<AssetSkeletonBoneData*> m_BoneList; //뼈 저장.
	};
#pragma endregion MGRT_Skeleton&Bones


#pragma region MGRT_Animation

	enum eAssetMorphingMethod {
		/** Morphing method to be determined */
		MGRTMorphingMethod_UNKNOWN = 0x0,

		/** Interpolation between morph targets */
		MGRTMorphingMethod_VERTEX_BLEND = 0x1,

		/** Normalized morphing between morph targets  */
		MGRTMorphingMethod_MORPH_NORMALIZED = 0x2,

		/** Relative morphing between morph targets  */
		MGRTMorphingMethod_MORPH_RELATIVE = 0x3,

		//실제 사용되는 값 아님, 4바이트 단위로 컴파일러의 사용처를 맞추기 위해!
		//(출처 : ASSIMP)
#ifndef SWIG
		MGRTMorphingMethod_Force32Bit = INT_MAX
#endif
	};

	class AssetVectorKeyData
	{
	public:
		/** The time of this key */
		double m_Time;

		/** The value of this key */
		DirectX::SimpleMath::Vector3 m_Value;

		/// @brief  The default constructor.
		AssetVectorKeyData()  noexcept
			: m_Time(0.0),
			m_Value() {
			// empty
		}

		/// @brief  Construction from a given time and key value.

		AssetVectorKeyData(double time, const DirectX::SimpleMath::Vector3& value) :
			m_Time(time), m_Value(value)
		{
			// empty
		}

		using elem_type = DirectX::SimpleMath::Vector3;

		// Comparison operators. For use with std::find();
		bool operator==(const AssetVectorKeyData& rhs) const
		{
			return rhs.m_Value == this->m_Value;
		}
		bool operator!=(const AssetVectorKeyData& rhs) const {
			return rhs.m_Value != this->m_Value;
		}

		// Relational operators. For use with std::sort();
		bool operator<(const AssetVectorKeyData& rhs) const {
			return m_Time < rhs.m_Time;
		}

		bool operator>(const AssetVectorKeyData& rhs) const {
			return m_Time > rhs.m_Time;
		}
	};

	// ---------------------------------------------------------------------------
/** A time-value pair specifying a rotation for the given time.
 *  Rotations are expressed with quaternions. */
	class AssetQuatKeyData
	{
	public:
		/** The time of this key */
		double m_Time;

		/** The value of this key */
		DirectX::SimpleMath::Quaternion m_Value;

		AssetQuatKeyData() noexcept
			: m_Time(0.0), m_Value()
		{
			// empty
		}

		/** Construction from a given time and key value */
		AssetQuatKeyData(double time, const DirectX::SimpleMath::Quaternion& value) :
			m_Time(time), m_Value(value) {}

		using elem_type = DirectX::SimpleMath::Quaternion;

		// Comparison operators. For use with std::find();
		bool operator==(const AssetQuatKeyData& rhs) const {
			return rhs.m_Value == this->m_Value;
		}

		bool operator!=(const AssetQuatKeyData& rhs) const {
			return rhs.m_Value != this->m_Value;
		}

		// Relational operators. For use with std::sort();
		bool operator<(const AssetQuatKeyData& rhs) const {
			return m_Time < rhs.m_Time;
		}

		bool operator>(const AssetQuatKeyData& rhs) const {
			return m_Time > rhs.m_Time;
		}
	};

	// ---------------------------------------------------------------------------
/** Binds a anim-mesh to a specific point in time. */
	class AssetMeshKeyData
	{
	public:
		/** The time of this key */
		double m_Time;

		/** Index into the aiMesh::mAnimMeshes array of the
		 *  mesh corresponding to the #aiMeshAnim hosting this
		 *  key frame. The referenced anim mesh is evaluated
		 *  according to the rules defined in the docs for #aiAnimMesh.*/
		unsigned int m_Value;

		AssetMeshKeyData() noexcept
			: m_Time(0.0),
			m_Value(0) {
		}

		/** Construction from a given time and key value */
		AssetMeshKeyData(double time, const unsigned int value) :
			m_Time(time), m_Value(value) {}

		using elem_type = unsigned int;

		// Comparison operators. For use with std::find();
		bool operator==(const AssetMeshKeyData& o) const {
			return o.m_Value == this->m_Value;
		}
		bool operator!=(const AssetMeshKeyData& o) const {
			return o.m_Value != this->m_Value;
		}

		// Relational operators. For use with std::sort();
		bool operator<(const AssetMeshKeyData& o) const {
			return m_Time < o.m_Time;
		}
		bool operator>(const AssetMeshKeyData& o) const {
			return m_Time > o.m_Time;
		}
	};

	// ---------------------------------------------------------------------------
/** Binds a morph anim mesh to a specific point in time. */
	class AssetMeshMorphKeyData
	{
	public:
		/** The time of this key */
		double m_Time;

		/** The values and weights at the time of this key
		 *   - mValues: index of attachment mesh to apply weight at the same position in mWeights
		 *   - mWeights: weight to apply to the blend shape index at the same position in mValues
		 */
		std::vector<unsigned int> m_ValueList;
		std::vector<double> mWeightList;

		/** The number of values and weights */
		unsigned int m_NumValuesAndWeights;

		AssetMeshMorphKeyData() noexcept
			: m_Time(0.0),
			m_NumValuesAndWeights(0)
		{}

		~AssetMeshMorphKeyData()
		{

		}
	};

	enum eAssetAnimBehaviour {
		/** The value from the default node transformation is taken*/
		MGRTAnimBehaviour_DEFAULT = 0x0,

		/** The nearest key value is used without interpolation */
		MGRTAnimBehaviour_CONSTANT = 0x1,

		/** The value of the nearest two keys is linearly
		 *  extrapolated for the current time value.*/
		MGRTAnimBehaviour_LINEAR = 0x2,

		/** The animation is repeated.
		 *
		 *  If the animation key go from n to m and the current
		 *  time is t, use the value at (t-n) % (|m-n|).*/
		MGRTAnimBehaviour_REPEAT = 0x3,

		//4바이트 단위 메모리 정렬을 위해.
#ifndef SWIG
		MGRTAnimBehaviour_Force32Bit = INT_MAX
#endif
	};

	class AssetNodeAnimData
	{
	public:
		/** The name of the node affected by this animation. The node
	*  must exist and it must be unique.*/
		std::string m_NodeName;

		/** The number of position keys */
		unsigned int m_NumPositionKey = 0;

		/** The position keys of this animation channel. Positions are
		 * specified as 3D vector. The array is mNumPositionKeys in size.
		 *
		 * If there are position keys, there will also be at least one
		 * scaling and one rotation key.*/
		std::vector<AssetVectorKeyData> mPositionKeyList;

		/** The number of rotation keys */
		unsigned int mNumRotationKey = 0;

		/** The rotation keys of this animation channel. Rotations are
		 *  given as quaternions,  which are 4D vectors. The array is
		 *  mNumRotationKeys in size.
		 *
		 * If there are rotation keys, there will also be at least one
		 * scaling and one position key. */
		std::vector<AssetQuatKeyData> mRotationKeyList;

		/** The number of scaling keys */
		unsigned int mNumScalingKey = 0;

		/** The scaling keys of this animation channel. Scalings are
		 *  specified as 3D vector. The array is mNumScalingKeys in size.
		 *
		 * If there are scaling keys, there will also be at least one
		 * position and one rotation key.*/
		std::vector<AssetVectorKeyData> mScalingKeyList;

		/** Defines how the animation behaves before the first
		 *  key is encountered.
		 *
		 *  The default value is aiAnimBehaviour_DEFAULT (the original
		 *  transformation matrix of the affected node is used).*/
		eAssetAnimBehaviour m_PreState = MGRTAnimBehaviour_DEFAULT;

		/** Defines how the animation behaves after the last
		 *  key was processed.
		 *
		 *  The default value is aiAnimBehaviour_DEFAULT (the original
		 *  transformation matrix of the affected node is taken).*/
		eAssetAnimBehaviour m_PostState = MGRTAnimBehaviour_DEFAULT;
	};

	class AssetMeshMorphAnimData
	{
	public:
		/** Name of the mesh to be animated. An empty string is not allowed,
	*  animated meshes need to be named (not necessarily uniquely,
	*  the name can basically serve as wildcard to select a group
	*  of meshes with similar animation setup)*/
		std::string m_Name;

		/** Size of the #mKeys array. Must be 1, at least. */
		unsigned int m_NumKey;

		/** Key frames of the animation. May not be nullptr. */
		std::vector<AssetMeshMorphKeyData> m_KeyList;

	};

	/// AnimMesh != MeshAnim.
	/** Describes vertex-based animations for a single mesh or a group of
 *  meshes. Meshes carry the animation data for each frame in their
 *  aiMesh::mAnimMeshes array. The purpose of aiMeshAnim is to
 *  define keyframes linking each mesh attachment to a particular
 *  point in time. */
	class AssetMeshAnimData
	{
	public:
		/** Name of the mesh to be animated. An empty string is not allowed,
		 *  animated meshes need to be named (not necessarily uniquely,
		 *  the name can basically serve as wild-card to select a group
		 *  of meshes with similar animation setup)*/
		std::string m_Name;

		unsigned int m_NumKeys;

		/** Size of the #mKeys array. Must be 1, at least. */
		std::vector<AssetMeshKeyData> m_KeyList;
	};

	class AssetAnimationData
	{
	public:
		/** The name of the animation. If the modeling package this data was
	*  exported from does support only a single animation channel, this
	*  name is usually empty (length is zero). */
		std::string m_Name;

		/** Duration of the animation in ticks.  */
		double m_Duration;

		/** Ticks per second. 0 if not specified in the imported file */
		double m_TicksPerSecond;

		/** The number of bone animation channels. Each channel affects
		 *  a single node. */
		unsigned int m_NumChannel;

		/** The node animation channels. Each channel affects a single node.
		 *  The array is mNumChannels in size. */
		std::vector<AssetNodeAnimData*> m_ChannelList;

		/** The number of mesh animation channels. Each channel affects
		 *  a single mesh and defines vertex-based animation. */
		unsigned int m_NumMeshChannel;

		/** The mesh animation channels. Each channel affects a single mesh.
		 *  The array is mNumMeshChannels in size. */
		std::vector<AssetMeshAnimData*> m_MeshChannelList;

		/** The number of mesh animation channels. Each channel affects
		 *  a single mesh and defines morphing animation. */
		unsigned int m_NumMorphMeshChannel;

		/** The morph mesh animation channels. Each channel affects a single mesh.
		 *  The array is mNumMorphMeshChannels in size. */
		std::vector<AssetMeshMorphAnimData*> mMorphMeshChannelList;
	};
#pragma endregion MGRT_Animation

#pragma region MGRT_Texture
	///ASSIMP처럼, BGRA8888 형식으로 유지!
	class AssetTexelData {
	public:
		unsigned char b, g, r, a;

		//! Comparison operator
		bool operator== (const AssetTexelData& other) const {
			return b == other.b && r == other.r &&
				g == other.g && a == other.a;
		}

		//! Inverse comparison operator
		bool operator!= (const AssetTexelData& other) const {
			return b != other.b || r != other.r ||
				g != other.g || a != other.a;
		}

		//! Conversion to a floating-point 4d color
		operator DirectX::SimpleMath::Color() const
		{
			return DirectX::SimpleMath::Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
		}
	};

	// --------------------------------------------------------------------------------
	/** Helper structure to describe an embedded texture
	 *
	 * Normally textures are contained in external files but some file formats embed
	 * them directly in the model file. There are two types of embedded textures:
	 * 1. Uncompressed textures. The color data is given in an uncompressed format.
	 * 2. Compressed textures stored in a file format like png or jpg. The raw file
	 * bytes are given so the application must utilize an image decoder (e.g. DevIL) to
	 * get access to the actual color data.
	 *
	 * Embedded textures are referenced from materials using strings like "*0", "*1", etc.
	 * as the texture paths (a single asterisk character followed by the
	 * zero-based index of the texture in the aiScene::mTextures array).
	 */
	class AssetTextureData
	{
	public:
		/** Width of the texture, in pixels
		 *
		 * If mHeight is zero the texture is compressed in a format
		 * like JPEG. In this case mWidth specifies the size of the
		 * memory area pcData is pointing to, in bytes.
		 */
		unsigned int m_Width;

		/** Height of the texture, in pixels
		 *
		 * If this value is zero, pcData points to an compressed texture
		 * in any format (e.g. JPEG).
		 */
		unsigned int m_Height;

		/** A hint from the loader to make it easier for applications
		 *  to determine the type of embedded textures.
		 *
		 * If mHeight != 0 this member is show how data is packed. Hint will consist of
		 * two parts: channel order and channel bitness (count of the bits for every
		 * color channel). For simple parsing by the viewer it's better to not omit
		 * absent color channel and just use 0 for bitness. For example:
		 * 1. Image contain RGBA and 8 bit per channel, achFormatHint == "rgba8888";
		 * 2. Image contain ARGB and 8 bit per channel, achFormatHint == "argb8888";
		 * 3. Image contain RGB and 5 bit for R and B channels and 6 bit for G channel, achFormatHint == "rgba5650";
		 * 4. One color image with B channel and 1 bit for it, achFormatHint == "rgba0010";
		 * If mHeight == 0 then achFormatHint is set set to '\\0\\0\\0\\0' if the loader has no additional
		 * information about the texture file format used OR the
		 * file extension of the format without a trailing dot. If there
		 * are multiple file extensions for a format, the shortest
		 * extension is chosen (JPEG maps to 'jpg', not to 'jpeg').
		 * E.g. 'dds\\0', 'pcx\\0', 'jpg\\0'.  All characters are lower-case.
		 * The fourth character will always be '\\0'.
		 */
		char achFormatHint[ASSET_HINTMAXTEXTURELEN];// 8 for string + 1 for terminator.

		/** Data of the texture.
		 *
		 * Points to an array of mWidth * mHeight aiTexel's.
		 * The format of the texture data is always ARGB8888 to
		 * make the implementation for user of the library as easy
		 * as possible. If mHeight = 0 this is a pointer to a memory
		 * buffer of size mWidth containing the compressed texture
		 * data. Good luck, have fun!
		 */
		 ///나는 이게 RGBA8888이다! 바뀐 것을 고려해라.
		std::vector<AssetTexelData> m_pcDataList;

		/** Texture original filename
		*
		* Used to get the texture reference
		*/
		std::string m_FileName;

		//! For compressed textures (mHeight == 0): compare the
		//! format hint against a given string.
		//! @param s Input string. 3 characters are maximally processed.
		//!        Example values: "jpg", "png"
		//! @return true if the given string matches the format hint
		bool CheckFormat(const char* s) const {
			if (nullptr == s) {
				return false;
			}

			return (0 == ::strncmp(achFormatHint, s, sizeof(achFormatHint)));
		}

		// Construction
		AssetTextureData() noexcept :
			m_Width(0),
			m_Height(0),
			m_FileName()
		{
			memset(achFormatHint, 0, sizeof(achFormatHint));
		}

		// Destruction
		~AssetTextureData() {

		}
	};
#pragma endregion MGRT_Texture

#pragma region MGRT_Material 
	/// 당장 필요하다. Material 작업 중!
	enum eAssetPropertyTypeInfoData
	{
		/** Array of single-precision (32 Bit) floats
 *
 *  It is possible to use aiGetMaterialInteger[Array]() (or the C++-API
 *  aiMaterial::Get()) to query properties stored in floating-point format.
 *  The material system performs the type conversion automatically.
*/
		MGRT_PTI_Float = 0x1,

		/** Array of double-precision (64 Bit) floats
		 *
		 *  It is possible to use aiGetMaterialInteger[Array]() (or the C++-API
		 *  aiMaterial::Get()) to query properties stored in floating-point format.
		 *  The material system performs the type conversion automatically.
		*/
		MGRT_PTI_Double = 0x2,

		/** The material property is an aiString.
		 *
		 *  Arrays of strings aren't possible, aiGetMaterialString() (or the
		 *  C++-API aiMaterial::Get()) *must* be used to query a string property.
		*/
		MGRT_PTI_String = 0x3,

		/** Array of (32 Bit) integers
		 *
		 *  It is possible to use aiGetMaterialFloat[Array]() (or the C++-API
		 *  aiMaterial::Get()) to query properties stored in integer format.
		 *  The material system performs the type conversion automatically.
		*/
		MGRT_PTI_Integer = 0x4,

		/** Simple binary buffer, content undefined. Not convertible to anything.
		*/
		MGRT_PTI_Buffer = 0x5,

		/** This value is not used. It is just there to force the
			 *  compiler to map this enum to a 32 Bit integer.
			 */
#ifndef SWIG
		MGRT_PTI_Force32Bit = INT_MAX
#endif

	};

	enum eAssetTextureType {
		/** Dummy value.
		 *
		 *  No texture, but the value to be used as 'texture semantic'
		 *  (#aiMaterialProperty::mSemantic) for all material properties
		 *  *not* related to textures.
		 */
		MGRT_TextureType_NONE = 0,

		/** LEGACY API MATERIALS
		 * Legacy refers to materials which
		 * Were originally implemented in the specifications around 2000.
		 * These must never be removed, as most engines support them.
		 */

		 /** The texture is combined with the result of the diffuse
		  *  lighting equation.
		  *  OR
		  *  PBR Specular/Glossiness
		  */
		MGRT_TextureType_DIFFUSE = 1,

		/** The texture is combined with the result of the specular
		 *  lighting equation.
		 *  OR
		 *  PBR Specular/Glossiness
		 */
		MGRT_TextureType_SPECULAR = 2,

		/** The texture is combined with the result of the ambient
		 *  lighting equation.
		 */
		MGRT_TextureType_AMBIENT = 3,

		/** The texture is added to the result of the lighting
		 *  calculation. It isn't influenced by incoming light.
		 */
		MGRT_TextureType_EMISSIVE = 4,

		/** The texture is a height map.
		 *
		 *  By convention, higher gray-scale values stand for
		 *  higher elevations from the base height.
		 */
		MGRT_TextureType_HEIGHT = 5,

		/** The texture is a (tangent space) normal-map.
		 *
		 *  Again, there are several conventions for tangent-space
		 *  normal maps. Assimp does (intentionally) not
		 *  distinguish here.
		 */
		MGRT_TextureType_NORMALS = 6,

		/** The texture defines the glossiness of the material.
		 *
		 *  The glossiness is in fact the exponent of the specular
		 *  (phong) lighting equation. Usually there is a conversion
		 *  function defined to map the linear color values in the
		 *  texture to a suitable exponent. Have fun.
		*/
		MGRT_TextureType_SHININESS = 7,

		/** The texture defines per-pixel opacity.
		 *
		 *  Usually 'white' means opaque and 'black' means
		 *  'transparency'. Or quite the opposite. Have fun.
		*/
		MGRT_TextureType_OPACITY = 8,

		/** Displacement texture
		 *
		 *  The exact purpose and format is application-dependent.
		 *  Higher color values stand for higher vertex displacements.
		*/
		MGRT_TextureType_DISPLACEMENT = 9,

		/** Lightmap texture (aka Ambient Occlusion)
		 *
		 *  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
		 *  covered by this material property. The texture contains a
		 *  scaling value for the final color value of a pixel. Its
		 *  intensity is not affected by incoming light.
		*/
		MGRT_TextureType_LIGHTMAP = 10,

		/** Reflection texture
		 *
		 * Contains the color of a perfect mirror reflection.
		 * Rarely used, almost never for real-time applications.
		*/
		MGRT_TextureType_REFLECTION = 11,

		/** PBR Materials
		 * PBR definitions from maya and other modelling packages now use this standard.
		 * This was originally introduced around 2012.
		 * Support for this is in game engines like Godot, Unreal or Unity3D.
		 * Modelling packages which use this are very common now.
		 */

		MGRT_TextureType_BASE_COLOR = 12,
		MGRT_TextureType_NORMAL_CAMERA = 13,
		MGRT_TextureType_EMISSION_COLOR = 14,
		MGRT_TextureType_METALNESS = 15,
		MGRT_TextureType_DIFFUSE_ROUGHNESS = 16,
		MGRT_TextureType_AMBIENT_OCCLUSION = 17,

		/** PBR Material Modifiers
		* Some modern renderers have further PBR modifiers that may be overlaid
		* on top of the 'base' PBR materials for additional realism.
		* These use multiple texture maps, so only the base type is directly defined
		*/

		/** Sheen
		* Generally used to simulate textiles that are covered in a layer of microfibers
		* eg velvet
		* https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_materials_sheen
		*/
		MGRT_TextureType_SHEEN = 19,

		/** Clearcoat
		* Simulates a layer of 'polish' or 'lacquer' layered on top of a PBR substrate
		* https://autodesk.github.io/standard-surface/#closures/coating
		* https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_materials_clearcoat
		*/
		MGRT_TextureType_CLEARCOAT = 20,

		/** Transmission
		* Simulates transmission through the surface
		* May include further information such as wall thickness
		*/
		MGRT_TextureType_TRANSMISSION = 21,

		/** Unknown texture
		 *
		 *  A texture reference that does not match any of the definitions
		 *  above is considered to be 'unknown'. It is still imported,
		 *  but is excluded from any further post-processing.
		*/
		MGRT_TextureType_UNKNOWN = 18,

#ifndef SWIG
		MGRT_TextureType_Force32Bit = INT_MAX
#endif
	};

	enum eAssetShadingMode {
		/** Flat shading. Shading is done on per-face base,
		 *  diffuse only. Also known as 'faceted shading'.
		 */
		MGRT_ShadingMode_Flat = 0x1,

		/** Simple Gouraud shading.
		 */
		MGRT_ShadingMode_Gouraud = 0x2,

		/** Phong-Shading -
		 */
		MGRT_ShadingMode_Phong = 0x3,

		/** Phong-Blinn-Shading
		 */
		MGRT_ShadingMode_Blinn = 0x4,

		/** Toon-Shading per pixel
		 *
		 *  Also known as 'comic' shader.
		 */
		MGRT_ShadingMode_Toon = 0x5,

		/** OrenNayar-Shading per pixel
		 *
		 *  Extension to standard Lambertian shading, taking the
		 *  roughness of the material into account
		 */
		MGRT_ShadingMode_OrenNayar = 0x6,

		/** Minnaert-Shading per pixel
		 *
		 *  Extension to standard Lambertian shading, taking the
		 *  "darkness" of the material into account
		 */
		MGRT_ShadingMode_Minnaert = 0x7,

		/** CookTorrance-Shading per pixel
		 *
		 *  Special shader for metallic surfaces.
		 */
		MGRT_ShadingMode_CookTorrance = 0x8,

		/** No shading at all. Constant light influence of 1.0.
		* Also known as "Unlit"
		*/
		MGRT_ShadingMode_NoShading = 0x9,
		MGRT_ShadingMode_Unlit = MGRT_ShadingMode_NoShading, // Alias

		/** Fresnel shading
		 */
		MGRT_ShadingMode_Fresnel = 0xa,

		/** Physically-Based Rendering (PBR) shading using
		* Bidirectional scattering/reflectance distribution function (BSDF/BRDF)
		* There are multiple methods under this banner, and model files may provide
		* data for more than one PBR-BRDF method.
		* Applications should use the set of provided properties to determine which
		* of their preferred PBR rendering methods are likely to be available
		* eg:
		* - If AI_MATKEY_METALLIC_FACTOR is set, then a Metallic/Roughness is available
		* - If AI_MATKEY_GLOSSINESS_FACTOR is set, then a Specular/Glossiness is available
		* Note that some PBR methods allow layering of techniques
		*/
		MGRT_ShadingMode_PBR_BRDF = 0xb,

#ifndef SWIG
		MGRT_ShadingMode_Force32Bit = INT_MAX
#endif
	};

	enum eAssetTextureFlags {
		/** The texture's color values have to be inverted (component-wise 1-n)
		 */
		MGRT_TextureFlags_Invert = 0x1,

		/** Explicit request to the application to process the alpha channel
		 *  of the texture.
		 *
		 *  Mutually exclusive with #aiTextureFlags_IgnoreAlpha. These
		 *  flags are set if the library can say for sure that the alpha
		 *  channel is used/is not used. If the model format does not
		 *  define this, it is left to the application to decide whether
		 *  the texture alpha channel - if any - is evaluated or not.
		 */
		MGRT_TextureFlags_UseAlpha = 0x2,

		/** Explicit request to the application to ignore the alpha channel
		 *  of the texture.
		 *
		 *  Mutually exclusive with #aiTextureFlags_UseAlpha.
		 */
		MGRT_TextureFlags_IgnoreAlpha = 0x4,

#ifndef SWIG
		MGRT_TextureFlags_Force32Bit = INT_MAX
#endif
	};

	enum eAssetBlendMode {
		/**
		 *  Formula:
		 *  @code
		 *  SourceColor*SourceAlpha + DestColor*(1-SourceAlpha)
		 *  @endcode
		 */
		MGRT_BlendMode_Default = 0x0,

		/** Additive blending
		 *
		 *  Formula:
		 *  @code
		 *  SourceColor*1 + DestColor*1
		 *  @endcode
		 */
		MGRT_BlendMode_Additive = 0x1,

		// we don't need more for the moment, but we might need them
		// in future versions ...

#ifndef SWIG
		MGRT_BlendMode_Force32Bit = INT_MAX
#endif
	};

	enum eAssetTextureMapping {
		/** The mapping coordinates are taken from an UV channel.
		 *
		 *  #AI_MATKEY_UVWSRC property specifies from which UV channel
		 *  the texture coordinates are to be taken from (remember,
		 *  meshes can have more than one UV channel).
		*/ 
		MGRT_TextureMapping_UV = 0x0,

		/** Spherical mapping */
		MGRT_TextureMapping_SPHERE = 0x1,

		/** Cylindrical mapping */
		MGRT_TextureMapping_CYLINDER = 0x2,

		/** Cubic mapping */
		MGRT_TextureMapping_BOX = 0x3,

		/** Planar mapping */
		MGRT_TextureMapping_PLANE = 0x4,

		/** Undefined mapping. Have fun. */
		MGRT_TextureMapping_OTHER = 0x5,

#ifndef SWIG
		MGRT_TextureMapping_Force32Bit = INT_MAX
#endif
	};

	enum eAssetTextureOp {
		/** T = T1 * T2 */
		MGRT_TextureOp_Multiply = 0x0,

		/** T = T1 + T2 */
		MGRT_TextureOp_Add = 0x1,

		/** T = T1 - T2 */
		MGRT_TextureOp_Subtract = 0x2,

		/** T = T1 / T2 */
		MGRT_TextureOp_Divide = 0x3,

		/** T = (T1 + T2) - (T1 * T2) */
		MGRT_TextureOp_SmoothAdd = 0x4,

		/** T = T1 + (T2-0.5) */
		MGRT_TextureOp_SignedAdd = 0x5,

#ifndef SWIG
		MGRT_TextureOp_Force32Bit = INT_MAX
#endif
	};

	/** @brief Defines how UV coordinates outside the [0...1] range are handled.
 *
 *  Commonly referred to as 'wrapping mode'.
 */
	enum eAssetTextureMapMode {
		/** A texture coordinate u|v is translated to u%1|v%1
		 */
		MGRT_TextureMapMode_Wrap = 0x0,

		/** Texture coordinates outside [0...1]
		 *  are clamped to the nearest valid value.
		 */
		MGRT_TextureMapMode_Clamp = 0x1,

		/** If the texture coordinates for a pixel are outside [0...1]
		 *  the texture is not applied to that pixel
		 */
		MGRT_TextureMapMode_Decal = 0x3,

		/** A texture coordinate u|v becomes u%1|v%1 if (u-(u%1))%2 is zero and
		 *  1-(u%1)|1-(v%1) otherwise
		 */
		MGRT_TextureMapMode_Mirror = 0x2,

#ifndef SWIG
		MGRT_TextureMapMode_Force32Bit = INT_MAX
#endif
	};


	class AssetMaterialPropertyData
	{
	public:
		AssetMaterialPropertyData() noexcept
			: m_Index(0),
			m_Semantic(eAssetTextureType::MGRT_TextureType_UNKNOWN),
			m_DataLength(0),
			m_Type(MGRT_PTI_Float),
			m_ExceptionBufferData(nullptr),
			m_DataSplit(MatPropStruct())
		{
			// empty
		}

		~AssetMaterialPropertyData() {
			delete[] m_ExceptionBufferData;
			m_ExceptionBufferData = nullptr;
		}

		std::string m_Key;
		eAssetTextureType m_Semantic;
		unsigned int m_Index;
		unsigned int m_DataLength;
		eAssetPropertyTypeInfoData m_Type;
		MatPropStruct m_DataSplit;
		char* m_ExceptionBufferData = nullptr; //바이너리 형식밖에 안 될 때, Property를 저장하는 Binary Buffer. 평소에 nullptr.
	};

	class AssetMaterialData
	{
	public:
		unsigned int m_NumAllocated;
		unsigned int m_NumProperty;
		std::vector<AssetMaterialPropertyData*> m_PropertyList;

		// GetTextureCount 이후 GetTexture()를 써야 함., (GetEmbedded Texture?)
		//					  (인덱스 = 자체 인덱스 그냥)
		//		텍스쳐 타입 // (벡터의 사이즈 = 전체 크기) // TexturePath // TexMapping  // UVIndex // BlendFactor  // Texture Operation // TextureMapMode
		std::map<eAssetTextureType, std::vector<std::tuple<std::string, eAssetTextureMapping, unsigned int, float, eAssetTextureOp, eAssetTextureMapMode>>> m_TexturePropContainer;
		//값을 못읽어와서 얘네들한테 쓰레기값이 들어있는 상황도 생각해야 한다.
	};

#pragma endregion MGRT_Material


#pragma region MGRT_Scene
	class AssetSceneData
	{
		friend class AssetDataHelper;
	public:
		AssetSceneData();
		~AssetSceneData();

		std::string m_Directory; //Material Load를 위해 보관.

		AssetNodeData* m_RootNode = nullptr;
		unsigned int m_NumMesh = 0;
		std::vector<AssetMeshData*> m_MeshList;
		unsigned int m_NumMaterials = 0; ///Material은 다음에!
		std::vector<AssetMaterialData*> m_MaterialList;
		unsigned int m_NumAnimation = 0; //Animation들의 숫자
		std::vector<AssetAnimationData*> m_AnimationList; //Animation 보관.
		unsigned int m_NumTexture = 0; //Texture들 숫자 보관.
		std::vector <AssetTextureData*> m_TextureList; // Texture들 보관.
		AssetMetaData* mMetaData = nullptr;; //씬 자체의 Metadata 보관.
		std::string m_SceneName; //씬의 이름.
		unsigned int m_NumSkeleton = 0; // Skeleton들의 숫자. 
		std::vector<AssetSkeletonData*> m_SkeletonList; //Skeleton들의 보관함.

		//AssetScene에 고유, 개별적으로 AssetTexturePath(Enum w/ 경로)를 저장한다.
		std::map<eAssetTextureType, AssetTextureSRV> m_ATSMap;

		//Node-Based를 위해 Assimp와 독립적으로 따로 보관.

		DirectX::SimpleMath::Matrix m_GlobalTransform;
		DirectX::SimpleMath::Matrix m_GlobalInverseTransform;

		bool m_SceneHasAnimations = false;
		bool m_SceneHasSkeletons = false;
		bool m_SceneHasMaterials = false;
		bool m_SceneHasMeshes = false;
		bool m_SceneHasTextures = false;

		AssetNodeData* FindNode(const std::string& _nodeName);

	private:
		//재귀/종속 부모 관계 연관 X, 단지 어떤 노드가 있는지 확인 / FindNode용.
		std::unordered_map<std::string, AssetNodeData*> m_NodePlainContainer;
		std::unordered_map<std::string, AssetBoneData*> m_BonePlainContainer;
	};
#pragma endregion MGRT_Scene

}