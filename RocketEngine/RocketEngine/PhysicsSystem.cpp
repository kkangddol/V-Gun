#include "PhysicsSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PlaneCollider.h"
#include "StaticBoxCollider.h"
#include "..\\RocketMath\\RocketMath.h"
#include "Camera.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore
{
	void PhysicsSystem::Initialize()
	{
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		// visual debugger 세팅, 로컬에 연결
		//_pvd = PxCreatePvd(*_foundation);
		//physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		//_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eDEBUG);

		// 버전, 세팅, 단위 등의 정보를 담은 물리
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);

		//PxInitExtensions(*_physics, _pvd);

		// 물리 씬을 생성
		CreatePhysXScene();

		// 머티리얼 생성(임의)
		_material = _physics->createMaterial(0.25f, 0.2f, 0.4f);

		// collider 전부 만들기.
		MakeAllCollider();
	}

	void PhysicsSystem::Finalize()
	{
		// initPhysics에서 초기화해준 전역 변수들을 release
		PX_RELEASE(_pxScene);
		PX_RELEASE(_dispatcher);
		PX_RELEASE(_physics);
		// visual debugger도 release
		if (_pvd)
		{
			physx::PxPvdTransport* transport = _pvd->getTransport();
			_pvd->release();	_pvd = NULL;
			PX_RELEASE(transport);
		}
		PX_RELEASE(_foundation);
	}

	void PhysicsSystem::CreatePhysXScene()
	{
		// 씬에 대한 설정
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
		// 임의로 중력을 2배로 했다. 23.8.16.AJY.		3배로 늘림 ㅎㅎ
		sceneDesc.gravity = physx::PxVec3(0.0f, -29.43f, 0.0f);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		_pxScene = _physics->createScene(sceneDesc);

		// Pvd에 정보 보내기
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}


	void PhysicsSystem::MakeAllCollider()
	{
		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
		{
			if (sceneIter.second == nullptr)
			{
				return;
			}

			// TODO : Scene에 따라 physX Scene 도 개별적으로 만드는게 좋을 듯.

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// 각각의 객체가 본인들을 그리는 것이 아닌
				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
				// object->GetRenderData();

				MakePlaneCollider(object);
				MakeBoxCollider(object);
				MakeSphereCollider(object);
				MakeCapsuleCollider(object);
				MakeStaticBoxCollider(object);
			}
		// fixedJoint를 잠깐 추가해보자
		//_fixedJoints.push_back(MakeFixedJoint(player, plBody));
		//_fixedJoints.push_back(MakeFixedJoint(player, plHead));
		MakeFixedJoint();
		AddActorsToScene();
		}
	}

	void PhysicsSystem::MakePlaneCollider(RocketEngine::GameObject* object)
	{
		RocketEngine::Collider* temp = object->GetComponent<RocketEngine::PlaneCollider>();
		
		// PlaneCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<RocketEngine::PlaneCollider>();

			for (auto& collider : colvec)
			{
				RocketEngine::PlaneCollider* planeCol = dynamic_cast<RocketEngine::PlaneCollider*>(collider);
				RocketEngine::RMFLOAT3 normal = planeCol->GetNormalVector();
				physx::PxPlane pxPlane = { normal.x,normal.y,normal.z, planeCol->GetDistance() };

				physx::PxRigidStatic* planeRigid = PxCreatePlane(*_physics, pxPlane, *_material);
				//_scene->addActor(*planeRigid);

				planeCol->SetPhysXRigid(planeRigid);
				planeRigid->userData = planeCol;
				_rigidStatics.push_back(planeRigid);
			}
		}
	}

	void PhysicsSystem::MakeBoxCollider(RocketEngine::GameObject* object)
	{
		RocketEngine::Collider* temp = object->GetComponent<RocketEngine::BoxCollider>();

		// boxCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<RocketEngine::BoxCollider>();

			for (auto& collider : colvec)
			{
				RocketEngine::BoxCollider* box = dynamic_cast<RocketEngine::BoxCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2, box->GetHeight() / 2, box->GetDepth() / 2), *_material);

				RocketEngine::RMFLOAT3 pos = RMFloat3MultiplyMatrix(collider->GetPositionOffset(), object->transform.GetWorldTM());
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
				physx::PxRigidDynamic* body = _physics->createRigidDynamic(localTm);
				body->attachShape(*shape);
				body->setAngularDamping(0.5f);
				body->setLinearDamping(0.5f);

				if (box->gameObject->objName == "player")
				{
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				}
				//body->setMass(4.5f);
				//physx::PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
				//_scene->addActor(*body);

				box->SetPhysXRigid(body);
				body->userData = box;
				_rigidDynamics.push_back(body);

				shape->release();
			}
		}
	}

	void PhysicsSystem::MakeSphereCollider(RocketEngine::GameObject* object)
	{
		RocketEngine::Collider* temp = object->GetComponent<RocketEngine::SphereCollider>();

		// SphereCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<RocketEngine::SphereCollider>();

			for (auto& collider : colvec)
			{
				RocketEngine::SphereCollider* sphere = dynamic_cast<RocketEngine::SphereCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(sphere->GetRadius()), *_material);

				RocketEngine::RMFLOAT3 pos = RMFloat3MultiplyMatrix(collider->GetPositionOffset(), object->transform.GetWorldTM());
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
				physx::PxRigidDynamic* body = _physics->createRigidDynamic(localTm);
				body->attachShape(*shape);
				body->setAngularDamping(0.5f);
				body->setLinearDamping(10.0f);
				//physx::PxRigidBodyExt::updateMassAndInertia(*body, 1000.0f);
				//_scene->addActor(*body);

				if (sphere->gameObject->objName == "playerHead")
				{
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				}

				sphere->SetPhysXRigid(body);
				body->userData = sphere;
				_rigidDynamics.push_back(body);

				shape->release();
			}
		}
	}

	void PhysicsSystem::MakeCapsuleCollider(RocketEngine::GameObject* object)
	{
		RocketEngine::Collider* temp = object->GetComponent<RocketEngine::CapsuleCollider>();

		// CapsuleCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<RocketEngine::CapsuleCollider>();

			for (auto& collider : colvec)
			{
				RocketEngine::CapsuleCollider* capsuleCol = dynamic_cast<RocketEngine::CapsuleCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(capsuleCol->GetRadius(), capsuleCol->GetHalfHeight()), *_material);

				RocketEngine::RMQuaternion quat = RMQuaternionMultiply(collider->GetRotationOffset(), object->transform.GetRotation());
				physx::PxTransform shapeTransform(physx::PxIdentity);
				shapeTransform.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
				shape->setLocalPose(shapeTransform);

				RocketEngine::RMFLOAT3 pos = RMFloat3MultiplyMatrix(collider->GetPositionOffset(), object->transform.GetWorldTM());
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
				physx::PxRigidDynamic* body = _physics->createRigidDynamic(localTm);
				body->setAngularDamping(0.5f);
				body->setLinearDamping(0.5f);
				body->attachShape(*shape);

				//_scene->addActor(*body);

				capsuleCol->SetPhysXRigid(body);
				body->userData = capsuleCol;
				_rigidDynamics.push_back(body);

				shape->release();
			}
		}
	}

	void PhysicsSystem::MakeStaticBoxCollider(RocketEngine::GameObject* object)
	{
		RocketEngine::Collider* temp = object->GetComponent<RocketEngine::StaticBoxCollider>();

		// StaticBoxCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<RocketEngine::StaticBoxCollider>();

			for (auto& collider : colvec)
			{
				RocketEngine::StaticBoxCollider* staticBoxCol = dynamic_cast<RocketEngine::StaticBoxCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(staticBoxCol->GetWidth() / 2, staticBoxCol->GetHeight() / 2, staticBoxCol->GetDepth() / 2), *_material);

				RocketEngine::RMFLOAT3 pos = RMFloat3MultiplyMatrix(collider->GetPositionOffset(), object->transform.GetWorldTM());
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
				physx::PxRigidStatic* body = _physics->createRigidStatic(localTm);
				body->attachShape(*shape);

				//_scene->addActor(*body);

				staticBoxCol->SetPhysXRigid(body);
				body->userData = staticBoxCol;
				_rigidStatics.push_back(body);

				shape->release();

			}
		}

		//std::vector<RocketEngine::StaticBoxCollider*> colliders = object->GetComponents<RocketEngine::StaticBoxCollider>();
		//int i = 0;
		//if (colliders[0] != nullptr)
		//{
		//	// StaticBoxCollider 생성
		//	for (const auto& col : colliders)
		//	{
		//		RocketEngine::StaticBoxCollider* staticBoxCol = dynamic_cast<RocketEngine::StaticBoxCollider*>(colliders[i++]);

		//		physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(staticBoxCol->GetWidth() / 2, staticBoxCol->GetHeight() / 2, staticBoxCol->GetDepth() / 2), *_material);

		//		RocketEngine::RMFLOAT3 pos = object->transform.GetLocalPosition();
		//		physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
		//		physx::PxRigidStatic* body = _physics->createRigidStatic(localTm);
		//		body->attachShape(*shape);

		//		//_scene->addActor(*body);

		//		staticBoxCol->SetPhysXRigid(body);
		//		body->userData = staticBoxCol;
		//		_rigidStatics.push_back(body);

		//		shape->release();
		//	}
		//}
	}

	physx::PxFixedJoint* PhysicsSystem::MakeFixedJoint(physx::PxRigidDynamic* actor1, physx::PxRigidDynamic* actor2)
	{
		// actor1과 actor2의 collider를 받아온다.
		//RocketEngine::BoxCollider* tempBox1 = static_cast<RocketEngine::BoxCollider*>(actor1->userData);
		RocketEngine::CapsuleCollider* tempBox1 = static_cast<RocketEngine::CapsuleCollider*>(actor1->userData);
		RocketEngine::DynamicCollider* tempBox2 = static_cast<RocketEngine::DynamicCollider*>(actor2->userData);
		// actor2의 y좌표를 (자신의 localPosition + actor1의 높이 절반) 으로 고정한다. 나머지 정보는 Identity로 고정.
		physx::PxTransform tempTransform(physx::PxIdentity);
		tempTransform.p.y = (*tempBox2).gameObject->transform.GetLocalPosition().y + (*tempBox1).GetHalfHeight()
			+ (*tempBox1).GetRadius();
		//tempTransform.q.z = (*tempBox2).gameObject->transform.GetLocalRotation().z;
		return physx::PxFixedJointCreate(*_physics, actor1, tempTransform,
			actor2, physx::PxTransform(physx::PxIdentity));
	}

	void PhysicsSystem::MakeFixedJoint()
	{
		for (auto& dynamics : _rigidDynamics)
		{
			RocketEngine::DynamicCollider* col = static_cast<RocketEngine::DynamicCollider*>(dynamics->userData);
			for (auto& servant : col->GetServants())
			{
				std::string test2 = col->gameObject->objName;
				std::string serv = servant->gameObject->objName;

				RocketEngine::RMFLOAT4 masterPos = RocketEngine::RMFLOAT4{ col->GetPositionOffset() ,1.0f} *col->gameObject->transform.GetWorldTM();
				physx::PxTransform masterTR(physx::PxIdentity);
				masterTR.p.x = masterPos.x;
				masterTR.p.y = masterPos.y;
				masterTR.p.z = masterPos.z;

				RocketEngine::RMFLOAT3 servantPos = servant->gameObject->transform.GetPosition();
				physx::PxTransform servantTR(physx::PxIdentity);
				servantTR.p.x = servantPos.x;
				servantTR.p.y = servantPos.y;
				servantTR.p.z = servantPos.z;

				physx::PxFixedJoint* result = physx::PxFixedJointCreate(*_physics, dynamics, servantTR,
					servant->GetRigidBody(), masterTR);

				_fixedJoints.push_back(result);
			}
		}
	}

	void PhysicsSystem::AddActorsToScene()
	{
		for (auto& dynamics : _rigidDynamics)
		{
			static_cast<RocketEngine::DynamicCollider*>(dynamics->userData)->UpdateToPhysics();
			_pxScene->addActor(*dynamics);
		}
		for (auto& statics : _rigidStatics)
		{
			static_cast<RocketEngine::StaticCollider*>(statics->userData)->UpdateToPhysics();
			_pxScene->addActor(*statics);
		}
	}

	RocketEngine::Collider* PhysicsSystem::RayCast(RocketEngine::RMFLOAT3 original, RocketEngine::RMFLOAT3 direction, float length, int* type /*= nullptr*/)
	{
		physx::PxVec3 rayOrigin;
		rayOrigin.x = original.x;
		rayOrigin.y = original.y;
		rayOrigin.z = original.z;

		physx::PxVec3 rayDirection;
		rayDirection.x = direction.x;
		rayDirection.y = direction.y;
		rayDirection.z = direction.z;

		RocketEngine::Collider* hitCol = nullptr;

		// 맞았는지를 판정.
		physx::PxRaycastBuffer _hit;
		bool hitSomething = _pxScene->raycast(rayOrigin, rayDirection, length, _hit);

		// 맞았을 때의 동작. 히트 대상의 포인터, 히트 위치, 세부동작.
		if (hitSomething)
		{
			physx::PxRigidActor* hitActor = _hit.block.actor;	// 충돌한 물체의 actor의 포인터

			// 바닥이랑 충돌한 경우에 대한 예외 처리.
			if (hitActor->getType() == physx::PxActorType::eRIGID_STATIC)
			{
				hitActor = static_cast<physx::PxRigidStatic*>(hitActor);
				if (type != nullptr)
				{
					*type = 1;
				}
			}
			else if (hitActor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
			{
				hitActor = static_cast<physx::PxRigidDynamic*>(hitActor);
				if (type != nullptr)
				{
					*type = 2;
				}
			}

			//physx::PxRigidDynamic* hitRigid = static_cast<physx::PxRigidDynamic*>(hitActor);

			// 피격 정보를 userdata에 저장
			hitCol = static_cast<RocketEngine::Collider*>(hitActor->userData);

			//hitCol->Collide();

			// 맞는 위치도 보내서 파티클 터뜨려야 함.
			physx::PxVec3 hitPoint = _hit.block.position;
		}

		return hitCol;
	}

	void PhysicsSystem::PhysicsUpdate(float deltaTime)
	{
		// delta time 기반 시뮬레이션.
		_pxScene->simulate(deltaTime);
		_pxScene->fetchResults(true);

		// 각각의 GameObject들에게 물리 이벤트 발생.
		for (auto& rigid : _rigidDynamics)
		{
			RocketEngine::DynamicCollider* col = static_cast<RocketEngine::DynamicCollider*>(rigid->userData);
			RocketEngine::GameObject* gameObject = col->gameObject;

			if (!col->GetWasCollided() && col->GetIsCollided())
			{
				gameObject->OnCollisionEnter();
			}
			else if (col->GetWasCollided() && col->GetIsCollided())
			{
				gameObject->OnCollisionStay();
			}
			else if (col->GetWasCollided() && !col->GetIsCollided())
			{
				gameObject->OnCollisionExit();
			}
		}

		// 물리 연산 결과를 오브젝트에 적용.
		RocketEngine::RMFLOAT3 pos;
		RocketEngine::RMQuaternion quat;

		physx::PxTransform temp;
		for (auto& rigid : _rigidDynamics)
		{
			temp = rigid->getGlobalPose();

			pos.x = temp.p.x;
			pos.y = temp.p.y;
			pos.z = temp.p.z;

			quat.x = temp.q.x;
			quat.y = temp.q.y;
			quat.z = temp.q.z;
			quat.w = temp.q.w;

			static_cast<RocketEngine::DynamicCollider*>(rigid->userData)->UpdateFromPhysics(pos, quat);
		}
	}

	void PhysicsSystem::UpdateToPhysics()
	{
		for (auto& rigid : _rigidDynamics)
		{
			RocketEngine::DynamicCollider* col = static_cast<RocketEngine::DynamicCollider*>(rigid->userData);
			col->UpdateToPhysics();
		}

		for (auto& rigid : _rigidStatics)
		{
			RocketEngine::StaticCollider* col = static_cast<RocketEngine::StaticCollider*>(rigid->userData);
			col->UpdateToPhysics();
		}
	}

	void PhysicsSystem::Flush()
	{
		for (auto& rigid : _rigidDynamics)
		{
			static_cast<RocketEngine::DynamicCollider*>(rigid->userData)->Flush();
		}
	}

	physx::PxScene* PhysicsSystem::GetPxScene()
	{
		return _pxScene;
	}

	std::vector<physx::PxRigidDynamic*>& PhysicsSystem::GetRigidDynamicsList()
	{
		return _rigidDynamics;
	}

	std::vector<physx::PxRigidStatic*>& PhysicsSystem::GetRigidStaticsList()
	{
		return _rigidStatics;
	}

}
