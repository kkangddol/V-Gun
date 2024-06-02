#pragma once
#include <string>
#include <memory>
#include "SimpleMath.h"
#include <chrono>
#include <vector>
#include <tuple>
#include "../RocketGraphicsInterface/AnimData.h"

/// <summary>
/// ���ӿ��� ������ ���� Animation �����͸� ������, Animation Handler�� ���� 
/// </summary>

namespace RocketCore::Graphics
{
	//���������� DataReference�� ������ �ִ�.
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
		static double _dTimeRecord; // �Ź� RenderAnimation�� RenderPipeline�� �����س��� (DeltaTime!)
	public:
		RenderAnimation(SkinnedMeshFormation* form, const std::string& animationName, const std::string& animFilePath, const std::string& ownerModelPath);

		std::string _animName;
		std::string _animFilePath;
		std::string _ownerModelPath;
		
		std::unique_ptr<AssetSceneData> _assetAnimSceneData; 

		void Initialize();

		//�ִϸ��̼ǿ��� Elapsed Time�� ���! -> ���� IsLoop�� ���, ElapsedTime�� �ڵ����� �ʱ�ȭ ��Ų��.
		void UpdateElapsedTime();

		//�ִϸ��̼��� ������Ʈ������ ���Ѵ�! (�ܼ��� ���ǹ�!)
		bool CheckIfShouldPlay();

		//����Ʈ ���۸� ��� ��, �ִϸ��̼� ������ ������Ʈ! (����/������)
		std::pair<ByteBuffer*, UINT> UpdateAnimationInfo(double interval);

		//������ BoneTransform�� �ݿ�!
		void BoneTransform(float timeInSeconds, std::vector<DirectX::SimpleMath::Matrix>& transforms);
		void ReadNodeHierarchy(float animationTime, const AssetNodeData* pNode, const DirectX::SimpleMath::Matrix& parentTransform);
		
		////AnimationHandler�� ���� �ڽ��� �÷��̵� ���̴�. ���� �� �ۿ� �غ�.
		//void UpdateAnimMatrices(AnimData animData);
		//
		//void ResetAnimation();

		//Animation�� ���� ByteBuffer (bones)

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

		//�ʱ⿡, Update�� Bone���� ��Ƴ��´�!
		std::vector<DirectX::SimpleMath::Matrix> _initBoneTransformList;
	private:

		//Animation ���̵� �ʱ� ���¸� �������� ���� (BONE)
		void InitAnimationBuffer();
		//�ڽ��� �ҼӵǾ� �ִ� Formation.
		SkinnedMeshFormation* _belongForm;

		//������ ������Ʈ�� Variable (Bones)
		std::pair<ByteBuffer*, UINT> _prevUpdatedVal;

		double _durationInSeconds = -1.0;
	};


}



