#include "AnimationHandler.h"
#include "SkinnedMeshFormation.h"
#include "RenderAnimation.h"
#include "AssetDataDefine.h"
#include <chrono>
#include <limits>

namespace RocketCore::Graphics
{
	AnimationHandler::AnimationHandler(SkinnedMeshObject* obj) : _belongSkinnedObject(obj), _nowAnimDataName("NOT INITIALIZED! LITERALLY NOTHING"), _nowPlayingAnim(nullptr)
	{
		animMap.clear();
	}

	AnimationHandler::~AnimationHandler()
	{

	}

	void AnimationHandler::Play(AnimData anim)
	{
		//animData�� name�� nullptr�� �ƿ� �ִϸ��̼��� �������� ���� ���̴�.
		if (anim.name == "")
		{
			return;
		}

		//���� ���� animData�� ������ ���̴� animData�� �̸��� ������ Ȯ���ؾ� ��.
		std::string tNewAnimDataName = anim.name;

		this->_nowAnimDataName = tNewAnimDataName;
		_nowPlayingAnim = animMap.at(tNewAnimDataName);

		//�� ��������, ���۽ð� �����ؾ�.
		_nowPlayingAnim->_startTime = std::chrono::steady_clock::now();

		//Tick�� Chrono�� �����;� �Ѵ�. -> End Time ���ϱ�!
		double tAnimTickDuration = _nowPlayingAnim->_assetAnimSceneData->m_AnimationList[0]->m_Duration;
		double tAnimTickPerSec = _nowPlayingAnim->_assetAnimSceneData->m_AnimationList[0]->m_TicksPerSecond;

		if (tAnimTickDuration <= std::numeric_limits<double>::epsilon())
		{
			assert(false);
		}
		else
		{
			using duration = std::chrono::steady_clock::duration;

			double tSPT = (double)1.0f / (double)tAnimTickPerSec;
			double tDurInSec = tAnimTickDuration * tSPT;
			_nowPlayingAnim->_potStopDuration = std::chrono::duration_cast<duration>(std::chrono::duration<double>(tDurInSec));
		}

		//Elapsed Time ����.
		_nowPlayingAnim->_elapseTime = std::chrono::steady_clock::duration::zero();
		_nowPlayingAnim->_isLoopingMode = anim.isLoop;
	}


	void AnimationHandler::InsertAnimationWithName(const std::string& _name, RenderAnimation* _anim)
	{
		animMap.insert(std::make_pair(_name, _anim));
	}







}