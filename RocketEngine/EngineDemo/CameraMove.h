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

	// Enter 누르면 조준선 보여줌.
private:
	void DrawAimLine();

public:
	float moveSpeed;

private:
	RocketEngine::Camera* _camera;
};
