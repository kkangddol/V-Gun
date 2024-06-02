#pragma once
#pragma once
#include "RocketAPILoader.h"

namespace RocketEngine
{
	class Camera;
}


class DebugCameraMove : public RocketEngine::Component
{
public:
	DebugCameraMove(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void OnMouseMove();

public:
	float moveSpeed;

private:
	RocketEngine::Camera* _camera;
};
