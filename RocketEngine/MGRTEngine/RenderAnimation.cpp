#include "RenderAnimation.h"
#include <algorithm>
#include "ResourceManager.h"
#include "SkinnedMeshFormation.h"
#include "AssetDataDefine.h"
#include "AssetModelData.h"
#include "MathHelper.h"
#include "ByteBuffer.hpp"
#include "DebugHelper.h"
#include <cmath>

namespace RocketCore::Graphics
{
	double RenderAnimation::_dTimeRecord = 0.0016;

	RenderAnimation::RenderAnimation(SkinnedMeshFormation* form, const std::string& animationName, const std::string& animFilePath, const std::string& ownerModelPath)
	{
		this->_belongForm = form;
		this->_animName = animationName;
		this->_animFilePath = animFilePath;
		this->_ownerModelPath = ownerModelPath;

		_startTime = std::chrono::steady_clock::now();
		_potStopDuration = duration::zero();
		_elapseTime = duration::zero();

		_prevUpdatedVal.first = nullptr;
		_prevUpdatedVal.second = NULL;
	}

	void RenderAnimation::Initialize()
	{
		///PutBytes, GetBytes 문제.
		//애니메이션이 사용되기 전에 PrevUpdatedVal을 셋업. 이 코드 한번이라도 호출 안되면 스킨드 오브젝트 오류!
		InitAnimationBuffer();

		//나중의 편의를 위해 미리 Duration In Seconds를 반환받는다!
		double durInTick = this->_assetAnimSceneData->m_AnimationList[0]->m_Duration;
		double tAnimTickPerSec = this->_assetAnimSceneData->m_AnimationList[0]->m_TicksPerSecond;
		double tSPT = (double)1.0f / (double)tAnimTickPerSec;

		this->_durationInSeconds = durInTick * tSPT;

		assert(true);
	}

	std::pair<ByteBuffer*, UINT> RenderAnimation::UpdateAnimationInfo(double interval)
	{
		//시간 Loop Logic 관련되어 업데이트!
		UpdateElapsedTime();

		//애니메이션 플레이 로직.
		if (CheckIfShouldPlay())
		{
			
			//리셋은 여기서 된다!
			_belongForm->m_ByteBufferForBones->clear();
			//다시 로드.

			//RenderUsageBoneInfo의 인덱스랑 동일하다. 
			std::vector<DirectX::SimpleMath::Matrix> tTransformList;
			BoneTransform(_elapseTime.count(), tTransformList);

			for (UINT i = 0; i < tTransformList.size(); i++)
			{
				_belongForm->m_ByteBufferForBones->putBytes((uint8_t*)&tTransformList[i], (uint32_t)sizeof(DirectX::SimpleMath::Matrix));
			}

			_prevUpdatedVal = std::make_pair(_belongForm->m_ByteBufferForBones, tTransformList.size());
			return _prevUpdatedVal;
		}
		else
		{
			_belongForm->m_ByteBufferForBones->clear();

			//루프가 없을 때, 값을 사용해서 마지막 Tick으로 유지시키기?
		
			///일단 이 상태에서 구조안정화는 이게 맞지만.. 오류나면 다시 보자.
			//BoneTransform(_durationInSeconds - 0.01, _initBoneTransformList);
			BoneTransform(1.0f, _initBoneTransformList);
			assert(true);

			for (UINT i = 0; i < _initBoneTransformList.size(); i++)
			{
				_belongForm->m_ByteBufferForBones->putBytes((uint8_t*)&_initBoneTransformList[i], (uint32_t)sizeof(DirectX::SimpleMath::Matrix));
			}

			_prevUpdatedVal = std::make_pair(_belongForm->m_ByteBufferForBones, _initBoneTransformList.size());
			return _prevUpdatedVal;
		}
	}

	void RenderAnimation::BoneTransform(float timeInSeconds, std::vector<DirectX::SimpleMath::Matrix>& transforms)
	{
		using Matrix = DirectX::SimpleMath::Matrix;

		assert(_belongForm->_modelData != nullptr);
		assert(this->_assetAnimSceneData != nullptr);

		float TicksPerSecond = _assetAnimSceneData->m_AnimationList[0]->m_TicksPerSecond;
		float TimeInTicks = timeInSeconds * TicksPerSecond;
		//float AnimationTime = fmod(TimeInTicks, _assetAnimSceneData->m_AnimationList[0]->m_Duration);
		float AnimationTime = TimeInTicks;
		
		//[TW] 응급 수술
		AssetAnimationData* it = _assetAnimSceneData->m_AnimationList[0];
		double recordMinTime = min(it->m_Duration, AnimationTime);
		//double recordMinTime = it->AnimationTime;
		/////코드의 문제를 파악하기 위한 Debug.
		//static unsigned long long tNum = 0;
		//tNum += 1;

		//현재 지나간 시간.
		//float elapsedTicks = fmod((this->_elapseTime).count() * TicksPerSecond, TimeInTicks);

		//Matrix의 스케일을 줄이기.
		//Matrix tMat = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);
		Matrix tMat = Matrix::Identity;

		///Skinning Debugging을 위해 임시 강제로 시간을 늘림!
		//ReadNodeHierarchy(AnimationTime, _assetAnimSceneData->m_RootNode, tMat/*Matrix::Identity*/);
		ReadNodeHierarchy(recordMinTime, _assetAnimSceneData->m_RootNode, tMat/*Matrix::Identity*/);



		transforms.resize(_belongForm->_formationNumBone);

		// Populates transforms vector with new bone transformation matrices. 
		for (unsigned int i = 0; i < _belongForm->_formationNumBone; i++)
		{
			transforms[i] = _belongForm->m_RenderBoneInfo[i].FinalTransformation;
		}

	}

	void RenderAnimation::ReadNodeHierarchy(float animationTime, const AssetNodeData* pNode, const DirectX::SimpleMath::Matrix& parentTransform)
	{
		using Matrix = DirectX::SimpleMath::Matrix;
		using Quaternion = DirectX::SimpleMath::Quaternion;

		// Obtain the name of the current node 
		std::string NodeName(pNode->m_NodeName);

		// Use the first animation 
		const AssetAnimationData* pAnimation = _assetAnimSceneData->m_AnimationList[0];

		// Obtain transformation relative to node's parent. 
		Matrix NodeTransformation(pNode->m_ParRelativeTransform);

		const AssetNodeAnimData* pNodeAnim = nullptr;

		// Find the animation channel of the current node. 
		for (unsigned i = 0; i < pAnimation->m_NumChannel; i++)
		{
			const AssetNodeAnimData* pNodeAnimIndex = pAnimation->m_ChannelList[i];

			// If there is a match for a channel with the current node's name, then we've found the animation channel. 
			if (pNodeAnimIndex->m_NodeName.compare(NodeName) == 0)
			{
				pNodeAnim = pNodeAnimIndex;
			}
		}

		if (pNodeAnim)
		{
			//// Interpolate scaling and generate scaling transformation matrix
			//aiVector3D Scaling;
			//CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			//Matrix4f ScalingM;
			//ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

			//Negative 1 Scale 마련!
			//XMMATRIX tNegative = XMMatrixScaling(-1.0f, -1.0f, -1.0f);
			//Matrix ttNegative(tNegative);

			// Interpolate rotation and generate rotation transformation matrix
			Quaternion RotationQ;
			//CalcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
			CalcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);

			Matrix RotationM = DirectX::XMMatrixRotationQuaternion(RotationQ);
			//RotationM = RotationM.Transpose(); 
			
			// Interpolate translation and generate translation transformation matrix
			DirectX::SimpleMath::Vector3 Translation;
			CalcInterpolatedTranslation(Translation, animationTime, pNodeAnim);
			Matrix TranslationM;
			TranslationM = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
			//TranslationM = TranslationM.Transpose();

			// Combine the above transformations
			NodeTransformation = RotationM * TranslationM;/* *ScalingM;*/
		}

		Matrix GlobalTransformation = NodeTransformation * parentTransform;

		// Apply the final transformation to the indexed bone in the array. 
		if (_belongForm->m_BoneMapping.find(NodeName) != _belongForm->m_BoneMapping.end())
		{
			unsigned int BoneIndex = _belongForm->m_BoneMapping[NodeName];
			//_belongForm->m_RenderBoneInfo[BoneIndex].FinalTransformation = _belongForm->_modelData->_sceneData->m_GlobalInverseTransform * GlobalTransformation *
			//	_belongForm->m_RenderBoneInfo[BoneIndex].BoneOffset;

			//1차 망가지기 전 Answer.
			_belongForm->m_RenderBoneInfo[BoneIndex].FinalTransformation =
				_belongForm->m_RenderBoneInfo[BoneIndex].BoneOffset * GlobalTransformation * _belongForm->_modelData->_sceneData->m_GlobalInverseTransform;
			
			
		}

		// Do the same for all the node's children. 
		for (unsigned i = 0; i < pNode->m_NumChildren; i++)
		{
			ReadNodeHierarchy(animationTime, pNode->m_ChildrenList[i], GlobalTransformation);
		}
	}

	void RenderAnimation::CalcInterpolatedRotation(DirectX::SimpleMath::Quaternion& Out, float AnimationTime, const AssetNodeAnimData* pNodeAnim)
	{
		using Matrix = DirectX::SimpleMath::Matrix;
		using Quaternion = DirectX::SimpleMath::Quaternion;

		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKey == 1)
		{
			Out = pNodeAnim->mRotationKeyList[0].m_Value;
			return;
		}
		// Obtain the current rotation keyframe. 
		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);

		// Calculate the next rotation keyframe and check bounds. 
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKey);

		// Calculate delta time, i.e time between the two keyframes.
		float DeltaTime = pNodeAnim->mRotationKeyList[NextRotationIndex].m_Time - pNodeAnim->mRotationKeyList[RotationIndex].m_Time;

		// Calculate the elapsed time within the delta time.  
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeyList[RotationIndex].m_Time) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);

		// Obtain the quaternions values for the current and next keyframe. 
		const Quaternion& StartRotationQ = pNodeAnim->mRotationKeyList[RotationIndex].m_Value;
		const Quaternion& EndRotationQ = pNodeAnim->mRotationKeyList[NextRotationIndex].m_Value;

		// Interpolate between them using the Factor. 
		Quaternion::Slerp(StartRotationQ, EndRotationQ, Factor, Out);

		// Normalise and set the reference. 
		Out.Normalize();
	}

	void RenderAnimation::CalcInterpolatedScaling(DirectX::SimpleMath::Vector3& Out, float AnimationTime, const AssetNodeAnimData* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumScalingKey == 1)
		{
			Out = pNodeAnim->mScalingKeyList[0].m_Value;
			return;
		}

		unsigned int ScalingIndex = FindScale(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKey);

		float DeltaTime = pNodeAnim->mScalingKeyList[NextScalingIndex].m_Time - pNodeAnim->mScalingKeyList[ScalingIndex].m_Time;
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeyList[ScalingIndex].m_Time) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const DirectX::SimpleMath::Vector3& Start = pNodeAnim->mScalingKeyList[ScalingIndex].m_Value;
		const  DirectX::SimpleMath::Vector3& End = pNodeAnim->mScalingKeyList[NextScalingIndex].m_Value;

		DirectX::SimpleMath::Vector3 Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void RenderAnimation::CalcInterpolatedTranslation(DirectX::SimpleMath::Vector3& Out, float AnimationTime, const AssetNodeAnimData* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->m_NumPositionKey == 1)
		{
			Out = pNodeAnim->mPositionKeyList[0].m_Value;
			return;
		}

		unsigned int PositionIndex = FindTranslation(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->m_NumPositionKey);

		float DeltaTime = pNodeAnim->mPositionKeyList[NextPositionIndex].m_Time - pNodeAnim->mPositionKeyList[PositionIndex].m_Time;
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeyList[PositionIndex].m_Time) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const DirectX::SimpleMath::Vector3& Start = pNodeAnim->mPositionKeyList[PositionIndex].m_Value;
		const DirectX::SimpleMath::Vector3& End = pNodeAnim->mPositionKeyList[NextPositionIndex].m_Value;

		DirectX::SimpleMath::Vector3 Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	unsigned int RenderAnimation::FindRotation(float AnimationTime, const AssetNodeAnimData* pNodeAnim)
	{
		// Check if there are rotation keyframes. 
		assert(pNodeAnim->mNumRotationKey > 0);

		// Find the rotation key just before the current animation time and return the index. 
		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKey - 1; i++) { ///FIX.
			if (AnimationTime <= (float)pNodeAnim->mRotationKeyList[i + 1].m_Time) {
				return i;
			}

			//if (AnimationTime < (float)pNodeAnim->mRotationKeyList[i + 1].m_Time) {
			//	return i;
			//}
		}
		assert(0);

		return 0;
	}

	unsigned int RenderAnimation::FindTranslation(float AnimationTime, const AssetNodeAnimData* pNodeAnim)
	{
		assert(pNodeAnim->m_NumPositionKey > 0);

		// Find the translation key just before the current animation time and return the index. 
		for (unsigned int i = 0; i < pNodeAnim->m_NumPositionKey - 1; i++) { ///FIX.
			if (AnimationTime <= (float)pNodeAnim->mPositionKeyList[i + 1].m_Time) {
				return i;
			}
			//if (AnimationTime < (float)pNodeAnim->mPositionKeyList[i + 1].m_Time) {
			//	return i;
			//}
		}
		assert(0);

		return 0;
	}

	unsigned int RenderAnimation::FindScale(float AnimationTime, const AssetNodeAnimData* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKey > 0);

		// Find the translation key just before the current animation time and return the index. 
		for (unsigned int i = 0; i < pNodeAnim->mNumScalingKey - 1; i++)
		{ 
			if (AnimationTime <= (float)pNodeAnim->mScalingKeyList[i + 1].m_Time) ///FIX.
			{
				return i;
			}

			//if (AnimationTime < (float)pNodeAnim->mScalingKeyList[i + 1].m_Time) ///FIX.
			//{
			//	return i;
			//}
		}
		assert(0);

		return 0;
	}

	bool RenderAnimation::CheckIfShouldPlay()
	{
		_isPlaying = _potStopDuration > _elapseTime;
		return _isPlaying;
	}

	void RenderAnimation::UpdateElapsedTime()
	{
		if (_isLoopingMode)
		{
			if (_elapseTime.count() - _potStopDuration.count() > std::numeric_limits<double>::epsilon())
			{
				_elapseTime = duration::zero();
				_startTime = std::chrono::steady_clock::now();
			}
			else
			{
				_elapseTime = std::chrono::steady_clock::now() - _startTime;
			}
		}
		else
		{
			if (_elapseTime.count() - _potStopDuration.count() < std::numeric_limits<double>::epsilon())
			{
				_elapseTime = std::chrono::steady_clock::now() - _startTime;
			}
		}
	}

	void RenderAnimation::InitAnimationBuffer()
	{
		_belongForm->m_ByteBufferForBones->clear();
		//RenderUsageBoneInfo의 인덱스랑 동일하다. 
		BoneTransform(1.0f, _initBoneTransformList);
		assert(true);

		for (UINT i = 0; i < _initBoneTransformList.size(); i++)
		{
			_belongForm->m_ByteBufferForBones->putBytes((uint8_t*)&_initBoneTransformList[i], (uint32_t)sizeof(DirectX::SimpleMath::Matrix));
		}

		_prevUpdatedVal = std::make_pair(_belongForm->m_ByteBufferForBones, _initBoneTransformList.size());
	}

	



}


