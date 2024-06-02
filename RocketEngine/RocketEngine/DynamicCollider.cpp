#include "DynamicCollider.h"
#include "GameObject.h"
#include "Transform.h"

#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>

namespace RocketEngine
{
	DynamicCollider::DynamicCollider(GameObject* owner)
		: Collider(owner),
		_isJumping(false),
		_isCollided(false),
		_wasCollided(false),
		_servant(),
		_prevParentWorldTM(),
		_prevParentRot()
	{

	}

	void DynamicCollider::Start()
	{
		_isJumping = false;
		_isCollided = false;
		_wasCollided = false;
		_prevParentWorldTM = {};
		_prevParentRot = {};
	}

	void DynamicCollider::SetPhysXRigid(physx::PxRigidDynamic* rigid)
	{
		_physXRigid = rigid;
	}

	std::vector<DynamicCollider*>& DynamicCollider::GetServants()
	{
		return _servant;
	}

	void DynamicCollider::SetFixedJoint(DynamicCollider* rhs)
	{
		_servant.push_back(rhs);
	}

	void DynamicCollider::UpdateFromPhysics(RMFLOAT3 pos, RMQuaternion quat)
	{
		RMFLOAT4 localPos = RMFloat4MultiplyMatrix({ pos,1.0f }, GetOffsetTM().Inverse());
		RMQuaternion localQuat = RMQuaternionMultiply(quat, GetRotationOffset().conjugate());

		if (!(gameObject->objName == "player"
			|| gameObject->objName == "playerBody"
			|| gameObject->objName == "playerHead"
			|| gameObject->objName == "player1"
			|| gameObject->objName == "player2"
			|| gameObject->objName == "player3"
			|| gameObject->objName == "player4"
			|| gameObject->objName == "playerBody1"
			|| gameObject->objName == "playerBody2"
			|| gameObject->objName == "playerBody3"
			|| gameObject->objName == "playerBody4"
			|| gameObject->objName == "playerHead1"
			|| gameObject->objName == "playerHead2"
			|| gameObject->objName == "playerHead3"
			|| gameObject->objName == "playerHead4"))
		{
			// 회전 각도에 따라 y좌표 내려간 만큼만 올려주기
			float r = GetHeight() / 2.0f;
			float tempAngle = 2 * acos(quat.w);
			localPos.y += r * (1 - cos(tempAngle));
		}

		gameObject->transform.SetPosition(localPos.x, localPos.y, localPos.z);
		gameObject->transform.SetRotation(localQuat);
	}

	void DynamicCollider::UpdateToPhysics()
	{
		if (!(gameObject->objName == "player"
			|| gameObject->objName == "playerBody"
			|| gameObject->objName == "playerHead"
			|| gameObject->objName == "player1"
			|| gameObject->objName == "player2"
			|| gameObject->objName == "player3"
			|| gameObject->objName == "player4"
			|| gameObject->objName == "playerBody1"
			|| gameObject->objName == "playerBody2"
			|| gameObject->objName == "playerBody3"
			|| gameObject->objName == "playerBody4"
			|| gameObject->objName == "playerHead1"
			|| gameObject->objName == "playerHead2"
			|| gameObject->objName == "playerHead3"
			|| gameObject->objName == "playerHead4"))
		{
			return;
		}

		RMFLOAT4 pos = RMFLOAT4(GetPositionOffset(), 1.0f) * gameObject->transform.GetWorldTM();
		RMQuaternion rot = RMQuaternionMultiply(GetRotationOffset(), gameObject->transform.GetRotation());

		physx::PxTransform pxTransform;

		pxTransform.p.x = pos.x;
		pxTransform.p.y = pos.y;
		pxTransform.p.z = pos.z;

		pxTransform.q.x = rot.x;
		pxTransform.q.y = rot.y;
		pxTransform.q.z = rot.z;
		pxTransform.q.w = rot.w;

		_physXRigid->setGlobalPose(pxTransform);
	}

	void DynamicCollider::LockRotationXZ()
	{
		physx::PxTransform pxTransform = _physXRigid->getGlobalPose();
		pxTransform.q.x = 0;
		pxTransform.q.z = 0;
		_physXRigid->setGlobalPose(pxTransform);
	}

	void DynamicCollider::AddForce(RMFLOAT3 direction, eForceMode forceMode)
	{
		physx::PxVec3 dir;
		dir.x = direction.x;
		dir.y = direction.y;
		dir.z = direction.z;

		switch (forceMode)
		{
			case eForceMode::eFORCE:
				_physXRigid->addForce(dir, physx::PxForceMode::eFORCE);
				break;
			case eForceMode::eIMPULSE:
				_physXRigid->addForce(dir, physx::PxForceMode::eIMPULSE);
				break;
			case eForceMode::eVELOCITY_CHANGE:
				_physXRigid->addForce(dir, physx::PxForceMode::eVELOCITY_CHANGE);
				break;
			case eForceMode::eACCELERATION:
				_physXRigid->addForce(dir, physx::PxForceMode::eACCELERATION);
				break;
		}
	}

	RocketEngine::RMFLOAT3 DynamicCollider::GetVelocity() const
	{
		physx::PxVec3 temp = _physXRigid->getLinearVelocity();
		return { temp.x, temp.y, temp.z };
	}

	void DynamicCollider::SetVelocity(RMFLOAT3 velocity)
	{
		_physXRigid->setLinearVelocity({ velocity.x,velocity.y,velocity.z });
	}

	void DynamicCollider::AddVelocity(RMFLOAT3 velocity)
	{
		physx::PxVec3 temp{ velocity.x, velocity.y, velocity.z };
		temp += _physXRigid->getLinearVelocity();
		_physXRigid->setLinearVelocity(temp);
	}

	void DynamicCollider::SetIsJumping(bool jump)
	{
		_isJumping = jump;
	}

	bool DynamicCollider::GetIsJumping() const
	{
		return _isJumping;
	}

	physx::PxRigidDynamic* DynamicCollider::GetRigidBody()
	{
		return _physXRigid;
	}

	void DynamicCollider::Collide()
	{
		_isCollided = true;
	}

	bool DynamicCollider::GetIsCollided() const
	{
		return _isCollided;
	}

	bool DynamicCollider::GetWasCollided() const
	{
		return _wasCollided;
	}

	void DynamicCollider::Flush()
	{
		_wasCollided = _isCollided;
		_isCollided = false;
	}

	void DynamicCollider::DisableSimulation()
	{
		_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
		for (auto& col : _servant)
		{
			col->_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
		}
	}

	void DynamicCollider::EnableSimulation()
	{
		_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);
		for (auto& col : _servant)
		{
			col->_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);
		}
	}

}
