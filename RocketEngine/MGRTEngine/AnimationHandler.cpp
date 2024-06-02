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
		//animData의 name이 nullptr면 아예 애니메이션을 실행하지 않을 것이다.
		if (anim.name == "")
		{
			return;
		}

		//지금 들어온 animData가 기존에 쓰이던 animData의 이름과 같은지 확인해야 함.
		std::string tNewAnimDataName = anim.name;

		this->_nowAnimDataName = tNewAnimDataName;
		_nowPlayingAnim = animMap.at(tNewAnimDataName);

		//막 들어왔으니, 시작시간 설정해야.
		_nowPlayingAnim->_startTime = std::chrono::steady_clock::now();

		//Tick을 Chrono로 가져와야 한다. -> End Time 정하기!
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

		//Elapsed Time 리셋.
		_nowPlayingAnim->_elapseTime = std::chrono::steady_clock::duration::zero();
		_nowPlayingAnim->_isLoopingMode = anim.isLoop;
	}


	void AnimationHandler::InsertAnimationWithName(const std::string& _name, RenderAnimation* _anim)
	{
		animMap.insert(std::make_pair(_name, _anim));
	}







}