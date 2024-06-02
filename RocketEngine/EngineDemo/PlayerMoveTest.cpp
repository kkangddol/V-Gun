#include "PlayerMoveTest.h"

PlayerMoveTest::PlayerMoveTest(RocketEngine::GameObject* owner)
	: Component(owner),
	moveSpeed(2.0f), isDebugMode(false)
{
}

void PlayerMoveTest::Start()
{
	_playerCollider = gameObject->GetComponent<RocketEngine::BoxCollider>();
	_playerCollider->SetIsJumping(true);

	physx::PxRigidDynamic* body = _playerCollider->GetRigidBody();
}

void PlayerMoveTest::Update()
{
	if (RocketEngine::GetKey(VK_RETURN))
	{
		RocketEngine::RMFLOAT3 pos = gameObject->transform.GetLocalPosition() + RocketEngine::RMFLOAT3(0.0f, 5.0f, 0.0f);
		RocketEngine::RMFLOAT3 target = pos + gameObject->transform.GetForward() * 10.0f;
		RocketEngine::DrawDebugLine(pos,target);
	}

	float deltaTime = RocketEngine::GetDeltaTime();

	if (isDebugMode)
	{
		MoveInDebugMode(deltaTime);
	}
	else
	{
		MoveInGameMode(deltaTime);
	}
	//_playerCollider->LockRotationXZ();
	TogglePlayerDebugMode();
}

void PlayerMoveTest::OnCollisionEnter()
{
	_playerCollider->AddForce({ 0.0f, 5.0f, 2.0f }, RocketEngine::eForceMode::eIMPULSE);
}

void PlayerMoveTest::OnHit(float x, float y, float z)
{
	_playerCollider->AddForce({ x, y, z }, RocketEngine::eForceMode::eIMPULSE);
}

void PlayerMoveTest::TogglePlayerDebugMode()
{
	if (GetAsyncKeyState('M') & 0x0001)
	{
		if (isDebugMode == false)
		{
			_playerCollider->DisableSimulation();
			isDebugMode = true;
		}
		else
		{
			// 씬에 등록하는 부분
			_playerCollider->EnableSimulation();
			isDebugMode = false;
		}
	}
}

void PlayerMoveTest::MoveInDebugMode(float deltaTime)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		// transform을 건드려 움직이기!
		RocketEngine::RMFLOAT3 temp = {0,0,0};
		temp.x = -moveSpeed * deltaTime * 0.1f;
		gameObject->transform.Translate(temp);
		//_playerCollider->UpdateToPhysics();
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		// transform을 건드려 움직이기!
		RocketEngine::RMFLOAT3 temp = { 0,0,0 };
		temp.z = moveSpeed * deltaTime * 0.1f;
		gameObject->transform.Translate(temp);
		//_playerCollider->UpdateToPhysics();
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		// transform을 건드려 움직이기!
		RocketEngine::RMFLOAT3 temp = { 0,0,0 };
		temp.x = moveSpeed * deltaTime * 0.1f;
		gameObject->transform.Translate(temp);
		//_playerCollider->UpdateToPhysics();
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		// transform을 건드려 움직이기!
		RocketEngine::RMFLOAT3 temp = { 0,0,0 };
		temp.z = -moveSpeed * deltaTime * 0.1f;
		gameObject->transform.Translate(temp);
		//_playerCollider->UpdateToPhysics();
	}

	if (GetAsyncKeyState('O') & 0x8000)
	{
		// transform을 건드려 움직이기!
		RocketEngine::RMFLOAT3 temp = { 0,0,0 };
		temp.y = moveSpeed * deltaTime * 0.1f;
		gameObject->transform.Translate(temp);
		//_playerCollider->UpdateToPhysics();
	}

	if (GetAsyncKeyState('L') & 0x8000)
	{
		// transform을 건드려 움직이기!
		RocketEngine::RMFLOAT3 temp = { 0,0,0 };
		temp.y = -moveSpeed * deltaTime * 0.1f;
		gameObject->transform.Translate(temp);
		//_playerCollider->UpdateToPhysics();
	}
}

void PlayerMoveTest::MoveInGameMode(float deltaTime)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		RocketEngine::RMFLOAT3 delta = gameObject->transform.GetRight() * -moveSpeed * deltaTime;
		// Physics로 움직이기!
		_playerCollider->AddVelocity(delta);
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		RocketEngine::RMFLOAT3 delta = gameObject->transform.GetForward() * moveSpeed * deltaTime;
		// Physics로 움직이기!
		_playerCollider->AddVelocity(delta);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		RocketEngine::RMFLOAT3 delta = gameObject->transform.GetRight() * moveSpeed * deltaTime;
		// Physics로 움직이기!
		_playerCollider->AddVelocity(delta);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		RocketEngine::RMFLOAT3 delta = gameObject->transform.GetForward() * -moveSpeed * deltaTime;
		// Physics로 움직이기!
		_playerCollider->AddVelocity(delta);
	}


	// Jump!
	if (_playerCollider->GetIsJumping() == false)
	{
		// 바닥에 있을 때
		if (_playerCollider->gameObject->transform.GetLocalPosition().y - _playerCollider->GetHeight() / 2 < 0.01f)
		{
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				_playerCollider->SetIsJumping(true);
				// Physics로 움직이기!
				_playerCollider->AddForce({ 0.0f, 20.0f, 0.0f }, RocketEngine::eForceMode::eIMPULSE);
			}
		}
	}
	else
	{
		if (_playerCollider->GetVelocity().y < 0.1f)
		{
			RocketEngine::RMFLOAT3 vel = _playerCollider->GetVelocity();
			vel.y = -10.0f;
			_playerCollider->SetVelocity(vel);
		}
		if (_playerCollider->gameObject->transform.GetLocalPosition().y - _playerCollider->GetHeight() / 2 < 0.01f) 
		{
			_playerCollider->SetIsJumping(false);
		}
	}
}
