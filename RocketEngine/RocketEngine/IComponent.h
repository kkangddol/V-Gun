#pragma once

namespace RocketEngine
{
	class GameObject;
}

/// <summary>
/// ������Ʈ�� �������̽�.
/// Component Ŭ������ �̰��� ��ӹް� �߻��Լ��� �󲮵���� �����Ѵ�.
/// �׸��� �ٸ� ������Ʈ�� Component Ŭ������ ��ӹ޴´�.
/// �̰� �³�? -> ��ɺ��� �������̽��� ����?
/// 
/// 23.06.29 ������ �����.
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
