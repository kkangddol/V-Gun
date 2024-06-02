#pragma once

namespace RocketEngine
{
	class GameObject;
}

/// <summary>
/// 컴포넌트의 인터페이스.
/// Component 클래스가 이것을 상속받고 추상함수를 빈껍데기로 구현한다.
/// 그리고 다른 컴포넌트가 Component 클래스를 상속받는다.
/// 이게 맞냐? -> 기능별로 인터페이스를 나눠?
/// 
/// 23.06.29 강석원 인재원.
/// </summary>
namespace RocketCore
{
	class IComponent
	{
		friend class RocketEngine::GameObject;

	public:
		//virtual ~IComponent() = default;

	protected:
		virtual void Awake() abstract;
		virtual void Start() abstract;
		virtual void FixedUpdate() abstract;
		virtual void Update() abstract;
		virtual void LateUpdate() abstract;
		virtual void Finalize() abstract;

	protected:
		virtual void OnDestroy() abstract;
		virtual void OnCollisionEnter() abstract;
		virtual void OnCollisionStay() abstract;
		virtual void OnCollisionExit() abstract;
	};
}
