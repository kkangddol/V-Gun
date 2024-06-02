#pragma once
#include "RocketAPILoader.h"

class SceneLoadTest : public RocketEngine::Component
{
public:
	SceneLoadTest(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void UpdateMoveDirection();
	void Move(int direction);

private:
	float _moveSpeed;
	int _moveDirection;	// 1~9 방향을 철권처럼 지정
};
