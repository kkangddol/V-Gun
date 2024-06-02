#pragma once
#include "DLLExporter.h"
#include "IComponent.h"

namespace RocketEngine
{
	class GameObject;
}

namespace RocketCore
{
	class ObjectSystem;
}

/// <summary>
/// 컴포넌트의 베이스 클래스.
/// IComponent 인터페이스를 상속받고 빈 구현으로 둔다.
/// 다른 컴포넌트가 이 클래스를 상속받는다.
/// 이게 맞냐? -> 기능별로 인터페이스를 나눠?
/// 
/// 23.06.29 강석원 인재원.
/// </summary>
namespace RocketEngine
{
	class ROCKET_API Component : public RocketCore::IComponent
	{
		friend class GameObject;
		friend class RocketCore::ObjectSystem;
	
	public:
		Component(GameObject* owner)
			: gameObject(owner)
		{

		}

		//virtual ~Component() = default;

	protected:
		virtual void Awake() override {}
		virtual void Start() override {}
		virtual void FixedUpdate() override {}
		virtual void Update() override {}
		virtual void LateUpdate() override {}
		virtual void Finalize() override {}

	protected:
		virtual void OnDestroy() override {}
		virtual void OnCollisionEnter() override {}
		virtual void OnCollisionStay() override {}
		virtual void OnCollisionExit() override {}


	public:
		GameObject* gameObject;

	private:
		void SetRestart();

	private:
		bool _isStarted = false;
	};
}
