#include "SceneLoadTest.h"
#include "RocketAPILoader.h"

SceneLoadTest::SceneLoadTest(RocketEngine::GameObject* owner)
	: Component(owner),
	_moveSpeed(30.0f),
	_moveDirection(5)
{

}

void SceneLoadTest::Start()
{
	gameObject->transform.SetPosition({ 0.0f,0.0f,0.0f });
	gameObject->transform.SetRotation({ 1.0f,0.0f,0.0f,0.0f });
	gameObject->transform.SetScale({ 1.0f,1.0f,1.0f });
}

void SceneLoadTest::Update()
{
	if (RocketEngine::GetKeyDown('1'))
	{
		RocketEngine::LoadScene("AScene");
	}

	if (RocketEngine::GetKeyDown('2'))
	{
		RocketEngine::LoadScene("BScene");
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

void SceneLoadTest::UpdateMoveDirection()
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

void SceneLoadTest::Move(int direction)
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

	// Translate으로 움직이기!
	gameObject->transform.Translate(delta);
}
