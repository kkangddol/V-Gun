#include "HierarchyController.h"

HierarchyController::HierarchyController(RocketEngine::GameObject* owner)
	: Component(owner),
	_moveSpeed(2.0f),
	_moveDirection(5)
{

}

void HierarchyController::Start()
{
	_playerCollider = gameObject->GetComponentDynamic<RocketEngine::DynamicCollider>();
	//_playerCollider = gameObject->GetComponent<RocketEngine::CapsuleCollider>();
	//_bodyCollider = gameObject->transform.GetChild(1)->gameObject->GetComponent<RocketEngine::CapsuleCollider>();
}

void HierarchyController::Update()
{
	if (RocketEngine::GetKeyDown('P'))
	{
		childSave->SetParent(gameObject);
	}

	if (RocketEngine::GetKeyDown('O'))
	{
		childSave->ReleaseParent();
	}

	if (RocketEngine::GetKey('Z'))
	{
		gameObject->transform.Rotate(0.0f, -0.1f, 0.0f);
	}

	if (RocketEngine::GetKey('C'))
	{
		gameObject->transform.Rotate(0.0f, 0.1f, 0.0f);
	}

	UpdateMoveDirection();
	Move(_moveDirection);
}

void HierarchyController::UpdateMoveDirection()
{
	_moveSpeed = 30.0f;
	if (RocketEngine::GetKey(VK_SHIFT))
	{
		_moveSpeed *= 2.0f;
	}

	// 키 입력 없으면 중립
	_moveDirection = 5;

	if (RocketEngine::GetKey('W'))
	{
		_moveDirection = 8;
	}

	if (RocketEngine::GetKey('S'))
	{
		_moveDirection = 2;

		if (RocketEngine::GetKey('W'))
		{
			_moveDirection = 5;
		}
	}

	if (RocketEngine::GetKey('A'))
	{
		_moveDirection = 4;

		if (RocketEngine::GetKey('W'))
		{
			_moveDirection = 7;
		}

		if (RocketEngine::GetKey('S'))
		{
			_moveDirection = 1;
		}
	}

	if (RocketEngine::GetKey('D'))
	{
		_moveDirection = 6;

		if (RocketEngine::GetKey('W'))
		{
			_moveDirection = 9;
		}

		if (RocketEngine::GetKey('A'))
		{
			_moveDirection = 5;
		}

		if (RocketEngine::GetKey('S'))
		{
			_moveDirection = 3;
		}
	}
}

void HierarchyController::Move(int direction)
{
	float deltaTime = RocketEngine::GetDeltaTime();
	float speed;
	RocketEngine::RMFLOAT3 delta = { 0.0f,0.0f,0.0f };

	switch (direction)
	{
		case 1:
		{
			speed = _moveSpeed * 0.5f;
			delta =
				gameObject->transform.GetRight() * -speed * deltaTime +
				gameObject->transform.GetForward() * -speed * deltaTime;
		}
		break;

		case 2:
		{
			speed = _moveSpeed * 0.7f;
			delta =
				gameObject->transform.GetForward() * -speed * deltaTime;
		}
		break;

		case 3:
		{
			speed = _moveSpeed * 0.5f;
			delta =
				gameObject->transform.GetRight() * speed * deltaTime +
				gameObject->transform.GetForward() * -speed * deltaTime;
		}
		break;

		case 4:
		{
			speed = _moveSpeed * 0.7f;
			delta =
				gameObject->transform.GetRight() * -speed * deltaTime;
		}
		break;

		case 5:
			break;

		case 6:
		{
			speed = _moveSpeed * 0.7f;
			delta =
				gameObject->transform.GetRight() * speed * deltaTime;
		}
		break;

		case 7:
		{
			speed = _moveSpeed * 0.7f;
			delta =
				gameObject->transform.GetRight() * -speed * deltaTime +
				gameObject->transform.GetForward() * speed * deltaTime;
		}
		break;

		case 8:
		{
			speed = _moveSpeed;
			delta =
				gameObject->transform.GetForward() * speed * deltaTime;
		}
		break;

		case 9:
		{
			speed = _moveSpeed * 0.7f;
			delta =
				gameObject->transform.GetRight() * speed * deltaTime +
				gameObject->transform.GetForward() * speed * deltaTime;
		}
	}

	// Physics로 움직이기!
	_playerCollider->AddVelocity(delta);

	// Translate으로 움직이기!
	//gameObject->transform.Translate(delta);

	// Jump!!
	DoJump();
}

void HierarchyController::DoJump()
{
	// Jump!
	if (_playerCollider->GetIsJumping() == false)
	{
		// 바닥에 있을 때
		if (_playerCollider->gameObject->transform.GetLocalPosition().y - _playerCollider->GetHeight() / 2 < 0.001f)
		//if (_playerCollider->GetVelocity().y == 0)
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
		if (_playerCollider->GetVelocity().y < 0.01f)
		{
			_playerCollider->SetVelocity(RocketEngine::RMFLOAT3{0.0, -10.0f, 0.0f});
		}
		//if (_playerCollider->gameObject->transform.GetLocalPosition().y - _playerCollider->GetHeight() / 2 < 0.001f)
		if (_playerCollider->GetVelocity().y <= 0.1f)
		{
			_playerCollider->SetIsJumping(false);
		}
	}
}