#pragma once
#include <string>
#include <memory>
#include "SimpleMath.h"
#include <chrono>
#include <vector>
#include <tuple>
#include "../RocketGraphicsInterface/AnimData.h"

/// <summary>
/// 게임엔진 측에서 만든 Animation 데이터를 가지고, Animation Handler에 의해 
/// </summary>

namespace RocketCore::Graphics
{
	//직접적으로 DataReference를 가지고 있다.
	class AssetAnimationData;
	class SkinnedMeshFormation;
	class RenderUsageNode;
	class ByteBuffer;

	class AssetSceneData;
	class AssetNodeData;
	class AssetBoneData;
	class AssetNodeAnimData;
	class AssetMeshData;

	class RenderAnimation
	{
		using Matrix = DirectX::SimpleMath::Matrix;
		using Quaternion = DirectX::SimpleMath::Quaternion;
		using time_point = std::chrono::steady_clock::time_point;
		using duration = std::chrono::duration<double>;
		friend class AnimationHandler;
		friend class SkinnedMeshObject;
	public:
		enum {MAXIMUM_BONE_COUNT = 100};
		static double _dTimeRecord; // 매번 RenderAnimation에 RenderPipeline이 저장해놓음 (DeltaTime!)
	public:
		RenderAnimation(SkinnedMeshFormation* form, const std::string& animationName, const std::string& animFilePath, const std::string& ownerModelPath);

		std::string _animName;
		std::string _animFilePath;
		std::string _ownerModelPath;
		
		std::unique_ptr<AssetSceneData> _assetAnimSceneData; 

		void Initialize();

		//애니메이션에서 Elapsed Time을 잰다! -> 만약 IsLoop일 경우, ElapsedTime을 자동으로 초기화 시킨다.
		void UpdateElapsedTime();

		//애니메이션을 업데이트할지를 정한다! (단순한 조건문!)
		bool CheckIfShouldPlay();

		//바이트 버퍼를 비운 뒤, 애니메이션 실제로 업데이트! (버퍼/사이즈)
		std::pair<ByteBuffer*, UINT> UpdateAnimationInfo(double interval);

		//실제로 BoneTransform을 반영!
		void BoneTransform(float timeInSeconds, std::vector<DirectX::SimpleMath::Matrix>& transforms);
		void ReadNodeHierarchy(float animationTime, const AssetNodeData* pNode, const DirectX::SimpleMath::Matrix& parentTransform);
		
		////AnimationHandler에 의해 자신이 플레이될 것이다. 보간 등 작용 준비.
		//void UpdateAnimMatrices(AnimData animData);
		//
		//void ResetAnimation();

		//Animation을 위한 ByteBuffer (bones)

		void CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& Out, float AnimationTime, const AssetNodeAnimData* pNodeAnim); //!< Calculates the interpolated quaternion between two keyframes. 
		void CalcInterpolatedScaling(DirectX::SimpleMath::Vector3& Out, float AnimationTime, const AssetNodeAnimData* pNodeAnim); //!< Calculates the interpolated scaling vector between two keyframes. 
		void CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& Out, float AnimationTime, const AssetNodeAnimData* pNodeAnim); //!< Calculates the interpolated translation vector between two keyframes. 

		unsigned int FindRotation(float AnimationTime, const AssetNodeAnimData* pNodeAnim);
		unsigned int FindScale(float AnimationTime, const AssetNodeAnimData* pNodeAnim);
		unsigned int FindTranslation(float AnimationTime, const AssetNodeAnimData* pNodeAnim);
	

		time_point _startTime;
		duration _potStopDuration;
		duration _elapseTime;
		bool _isLoopingMode = false;

		bool _isPlaying = false;

		//초기에, Update된 Bone들을 담아놓는다!
		std::vector<DirectX::SimpleMath::Matrix> _initBoneTransformList;
	private:

		//Animation 없이도 초기 상태를 가져오기 위해 (BONE)
		void InitAnimationBuffer();
		//자신이 소속되어 있는 Formation.
		SkinnedMeshFormation* _belongForm;

		//직전의 업데이트된 Variable (Bones)
		std::pair<ByteBuffer*, UINT> _prevUpdatedVal;

		double _durationInSeconds = -1.0;
	};


}



