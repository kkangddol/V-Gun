#pragma once
#include "DLLExporter.h"
#include "Collider.h"
#include <vector>
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketEngine
{
	enum class eForceMode : int
	{
		eFORCE,				//!< parameter has unit of mass * length / time^2, i.e., a force
		eIMPULSE,			//!< parameter has unit of mass * length / time, i.e., force * time
		eVELOCITY_CHANGE,	//!< parameter has unit of length / time, i.e., the effect is mass independent: a velocity change.
		eACCELERATION		//!< parameter has unit of length/ time^2, i.e., an acceleration. It gets treated just like a force except the mass is not divided out before integration.
	};
}

namespace physx
{
	class PxRigidDynamic;
}

namespace RocketEngine
{
	class GameObject;

	class ROCKET_API DynamicCollider : public Collider
	{
	public:
		DynamicCollider(GameObject* owner);

	protected:
		virtual void Start() override;

	public:
		void Collide();
		bool GetIsCollided() const;

		bool GetWasCollided() const;

		void Flush();

	public:
		void DisableSimulation();
		void EnableSimulation();

	public:
		void SetPhysXRigid(physx::PxRigidDynamic* rigid);

	public:
		std::vector<DynamicCollider*>& GetServants();
		void SetFixedJoint(DynamicCollider* rhs);

	private:
		std::vector<DynamicCollider*> _servant;

	public:
		void UpdateFromPhysics(RMFLOAT3 pos, RMQuaternion quat);
		virtual void UpdateToPhysics() override;
		void LockRotationXZ();

	public:
		virtual float GetWidth() const abstract;
		virtual float GetHeight() const abstract;
		virtual float GetDepth() const abstract;

	public:
		void AddForce(RMFLOAT3 direction, eForceMode forceMode);

	public:
		RMFLOAT3 GetVelocity() const;
		void SetVelocity(RMFLOAT3 velocity);
		void AddVelocity(RMFLOAT3 velocity);
		void SetIsJumping(bool jump);
		bool GetIsJumping() const;
		physx::PxRigidDynamic* GetRigidBody();


	private:
		physx::PxRigidDynamic* _physXRigid;

		// 점프 안할 때는 y좌표 잠궈버리기 위해 추가. 23.8.1.AJY.
		bool _isJumping;
		bool _isCollided;
		bool _wasCollided;

	private:
		RMFLOAT4X4 _prevParentWorldTM;
		RMQuaternion _prevParentRot;
	};
}
