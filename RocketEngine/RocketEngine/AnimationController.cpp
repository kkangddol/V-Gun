#include "AnimationController.h"
#include "RenderSystem.h"
#include "MeshRendererBase.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "GameObject.h"
#include "Animator.h"
#include "State.h"
#include "..\\RocketGraphicsInterface\\AnimData.h"
#include "TimeSystem.h"

namespace RocketEngine
{
	void AnimationController::SetCurrentState(std::string stateName)
	{
		if (_prevStateName == stateName)
		{
			return;
		}

		_prevStateName = _animator->GetCurrentState()->GetStateName();

		_elapsedTime = 0.0f;
		_animator->SetCurrentState(stateName);
		PlayCurrentAnim();

		if (onStateChanged != nullptr)
		{
			onStateChanged();
		}
	}

	AnimationController::AnimationController(GameObject* owner)
		: Component(owner)
	{

	}

	AnimationController::~AnimationController()
	{

	}

	void AnimationController::Start()
	{
		_animator->Initialize();
	}

	void AnimationController::Update()
	{
		State* currentState = GetCurrentState();
		if (currentState->GetIsLoop())
		{
			if (currentState->GetDuration() < _elapsedTime)
			{
				_elapsedTime = 0.0f;
				PlayCurrentAnim();
			}
			_elapsedTime += RocketCore::TimeSystem::Instance().GetDeltaTime();
		}
	}

	// юс╫ц
	void AnimationController::PlayCurrentAnim()
	{
		RocketCore::MeshRendererBase* renderer = gameObject->GetComponentDynamic<RocketCore::MeshRendererBase>();
		
		if (!renderer)
		{
			return;
		}

		RocketCore::Graphics::AnimData currentAnimData = GetCurrentState()->GetAnimData();
		RocketCore::RenderSystem::Instance().PlayAnimation(renderer, currentAnimData);
	}

	void AnimationController::SetAnimator(Animator* animator)
	{
		_animator = animator;
		for (auto& iter : _animator->GetAllStates())
		{
			iter.second->SetAnimDescObjName(gameObject->objName);
		}
	}

	RocketEngine::Animator* AnimationController::GetAnimator()
	{
		return _animator;
	}

	void AnimationController::SetCondition(std::string conditionName, bool isOn)
	{
		if (_animator->SetCondition(conditionName, isOn))
		{
			PlayCurrentAnim();
		}
	}

	State* AnimationController::GetCurrentState()
	{
		return _animator->GetCurrentState();
	}
}
