#pragma once
#include "RocketAPILoader.h"

namespace RocketEngine
{
	class Camera;
}


class CameraMove : public RocketEngine::Component
{
public:
	CameraMove(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void OnMouseMove();

	// Enter ������ ���ؼ� ������.
private:
	void DrawAimLine();

public:
	float moveSpeed;

private:
	RocketEngine::Camera* _camera;
};
