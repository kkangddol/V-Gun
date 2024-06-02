#pragma once
#include "RocketAPILoader.h"

class PlayerMoveTest : public RocketEngine::Component
{
public:
	PlayerMoveTest(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual void OnCollisionEnter() override;

public:
	void OnHit(float x, float y, float z);

public:
	float moveSpeed;

public:
	// �÷��̾� ���� ���. �̰� ���� �ִ°� �´��� �𸣰ڴ�. 23.8.2.AJY.
	void TogglePlayerDebugMode();

private:
	void MoveInDebugMode(float deltaTime);
	void MoveInGameMode(float deltaTime);

private:
	RocketEngine::BoxCollider* _playerCollider;

	/// Noclip mode��.
private:
	bool isDebugMode;
};

