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
/// ������Ʈ�� ���̽� Ŭ����.
/// IComponent �������̽��� ��ӹް� �� �������� �д�.
/// �ٸ� ������Ʈ�� �� Ŭ������ ��ӹ޴´�.
/// �̰� �³�? -> ��ɺ��� �������̽��� ����?
/// 
/// 23.06.29 ������ �����.
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
