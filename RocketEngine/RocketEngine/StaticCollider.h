#pragma once
#include "DLLExporter.h"
#include "Collider.h"
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace physx
{
	class PxRigidStatic;
}

namespace RocketEngine
{
	class GameObject;

	class ROCKET_API StaticCollider : public Collider
	{
	public:
		StaticCollider(GameObject* owner);

	public:
		void SetPhysXRigid(physx::PxRigidStatic* rigid);

	public:
		virtual void UpdateToPhysics() override;

	public:
		physx::PxRigidStatic* GetRigidBody();

	public:
		void Collide();
		bool GetIsCollided() const;

		bool GetWasCollided() const;

		void Flush();

	private:
		physx::PxRigidStatic* _physXRigid;

		bool _isCollided;
		bool _wasCollided;
	};
}
