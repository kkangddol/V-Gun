#pragma once
#include "RocketAPILoader.h"

class HierarchyController : public RocketEngine::Component
{
public:
	HierarchyController(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void UpdateMoveDirection();
	void Move(int direction);
	void DoJump();

public:
	RocketEngine::Transform* childSave;

private:
	float _moveSpeed;
	int _moveDirection;	// 1~9 방향을 철권처럼 지정

private:
	RocketEngine::DynamicCollider* _playerCollider;
};

