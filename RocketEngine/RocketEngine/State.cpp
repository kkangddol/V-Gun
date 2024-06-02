#include "State.h"

namespace RocketEngine
{
	State::State()
		: _transitions(),
		_stateName(),
		_animDesc(),
		_animData()
	{

	}

	State::~State()
	{
		for (auto& trans : _transitions)
		{
			delete trans;
		}
	}

	void State::AddTranstiion(State* to, bool* condition, bool comp)
	{
		Transition* trans = new Transition;
		trans->_stateNext = to;
		trans->_isConditionOn = condition;
		trans->comparison = comp;
		_transitions.push_back(trans);
	}

	std::vector<Transition*>& State::GetAllTransitions()
	{
		return _transitions;
	}

	std::string State::GetStateName()
	{
		return _stateName;
	}

	float State::GetDuration()
	{
		return _animData.duration;
	}

	bool State::GetIsLoop()
	{
		return _animData.isLoop;
	}

	std::string State::GetAnimName()
	{
		return _animDesc.name;
	}

	std::string State::GetAnimPath()
	{
		return _animDesc.animPath;
	}

	std::string State::GetOwnerPath()
	{
		return _animDesc.ownerModelPath;
	}

	void State::SetStateName(std::string stateName)
	{
		_stateName = stateName;
	}

	void State::SetDuration(float duration)
	{
		_animData.duration = duration;
	}

	void State::SetLoop(bool isLoop)
	{
		_animData.isLoop = isLoop;
	}

	void State::SetAnimDescName(std::string name)
	{
		_animDesc.name = name;
	}

	void State::SetAnimDescAnimPath(std::string animPath)
	{
		_animDesc.animPath = animPath;
	}

	void State::SetAnimDescOwnerPath(std::string ownerPath)
	{
		_animDesc.ownerModelPath = ownerPath;
	}

	void State::SetAnimDescObjName(std::string objName)
	{
		_animDesc.objName = objName;
	}

	void State::SetAnimDataName(std::string name)
	{
		_animData.name = name;
	}

	void State::SetAnimDataLoop(bool isLoop)
	{
		_animData.isLoop = isLoop;
	}

	RocketCore::Graphics::AnimDesc State::GetAnimDesc()
	{
		return _animDesc;
	}

	RocketCore::Graphics::AnimData State::GetAnimData()
	{
		return _animData;
	}

	RocketCore::Graphics::AnimData& State::GetAnimDataRef()
	{
		return _animData;
	}

}
