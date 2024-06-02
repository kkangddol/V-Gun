#include "DebugCameraMove.h"

DebugCameraMove::DebugCameraMove(RocketEngine::GameObject* owner)
	: Component(owner),
	moveSpeed(2.0f)
{
}

void DebugCameraMove::Start()
{
	_camera = gameObject->GetComponent<RocketEngine::Camera>();
}

void DebugCameraMove::Update()
{
	//float deltaTime = rocket.GetDeltaTime();
	float deltaTime = RocketEngine::GetDeltaTime();
	moveSpeed = 5.0f;

	if (RocketEngine::GetKey(VK_SHIFT))
	{
		moveSpeed *= 2.0f;
	}

	if (RocketEngine::GetKey(VK_UP))
	{
		_camera->Walk(moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_DOWN))
	{
		_camera->Walk(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_LEFT))
	{
		_camera->Strafe(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_RIGHT))
	{
		_camera->Strafe(moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_NEXT))
	{
		_camera->WorldUpDown(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_PRIOR))
	{
		_camera->WorldUpDown(moveSpeed * deltaTime);
	}

	OnMouseMove();
}

void DebugCameraMove::OnMouseMove()
{
	if (!RocketEngine::GetKey(VK_RBUTTON))
	{
		return;
	}

	RocketEngine::RMFLOAT2 mouseDelta = RocketEngine::GetMouseDelta();
	mouseDelta = mouseDelta * RocketEngine::GetDeltaTime();
	_camera->Pitch(mouseDelta.y);
	_camera->RotateY(mouseDelta.x);
}