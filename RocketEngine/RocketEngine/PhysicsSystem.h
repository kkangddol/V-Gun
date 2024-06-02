#pragma once
#include "Singleton.h"
#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>
#include <physx/extensions/PxDefaultAllocator.h>
#include <physx/extensions/PxDefaultErrorCallback.h>
#include <vector>

#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketEngine
{
	class GameObject;
	class Collider;
	class DynamicCollider;
	class BoxCollider;
	class SphereCollider;
	class Raycast;
}

namespace RocketCore
{
	class PhysicsSystem : public Singleton<PhysicsSystem>
	{
		friend Singleton;
	private:
		PhysicsSystem() = default;		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

	public:
		void Initialize();
		void Finalize();

	public:
		void PhysicsUpdate(float deltaTime);
		void UpdateToPhysics();
		void Flush();

	public:
		void CreatePhysXScene();

	public:
		void MakeAllCollider();
		void MakePlaneCollider(RocketEngine::GameObject* object);
		void MakeBoxCollider(RocketEngine::GameObject* object);
		void MakeSphereCollider(RocketEngine::GameObject* object);
		void MakeCapsuleCollider(RocketEngine::GameObject* object);
		void MakeStaticBoxCollider(RocketEngine::GameObject* object);
		physx::PxFixedJoint* MakeFixedJoint(physx::PxRigidDynamic* actor1, physx::PxRigidDynamic* actor2);
		void MakeFixedJoint();
		void AddActorsToScene();

	public:
		RocketEngine::Collider* RayCast(RocketEngine::RMFLOAT3 original, RocketEngine::RMFLOAT3 direction, float length, int* type = nullptr);

	public:
		physx::PxScene* GetPxScene();

	public:
		std::vector<physx::PxRigidDynamic*>& GetRigidDynamicsList();
		std::vector<physx::PxRigidStatic*>& GetRigidStaticsList();

	private:
		std::vector<physx::PxRigidDynamic*> _rigidDynamics;
		std::vector<physx::PxRigidStatic*> _rigidStatics;
		std::vector<physx::PxFixedJoint*> _fixedJoints;

	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _pxScene;
		physx::PxMaterial* _material;
		physx::PxPvd* _pvd;

	};
}
