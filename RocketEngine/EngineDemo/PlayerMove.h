#pragma once
#include "RocketAPILoader.h"
#include "Enum.pb.h"
#include "Types.h"

class PlayerInfo;

class PlayerMove : public RocketEngine::Component
{
public:
	PlayerMove(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	bool CheckIsOnGround();
	void TogglePlayerDebugMode();

public:
	void UpdateMoveDirection();
	void Move(int direction);
	void Jump();
	void PlayPlayerSound();
	void Control();
	void AdjustSetting();

	//void DoRayCast();
	void RotateOnMouseMove();
	void RotateY(float angle);
	void PitchCamera();

	RocketEngine::Camera* GetPlayerCamera();
	RocketEngine::AnimationController* GetPlayerAnim();

	void OnStateChanged();

	void AssignGunAnim(int index, RocketEngine::AnimationController* gunAnim);

public:
	void ShootGun(PlayerInfo* playerInfo, RocketEngine::RMFLOAT3 pos = { 0.0f,0.0f,0.0f }, RocketEngine::RMQuaternion rot = {1.0f,0.0f,0.0f,0.0f});
	// position을 보간하는 함수
	RocketEngine::RMFLOAT3 InterpolationPosition(RocketEngine::RMFLOAT3 currentPos, RocketEngine::RMFLOAT3 servPos, float intermediateValue);
	RocketEngine::RMQuaternion InterpolationRotation(RocketEngine::RMQuaternion currentRot, RocketEngine::RMQuaternion servRot, float intermediateValue);
	RocketEngine::RMFLOAT3 InterpolationPositionXZ(RocketEngine::RMFLOAT3 currentPos, RocketEngine::RMFLOAT3 servPos, float intermediateValue);

	void ClickAttack(Protocol::AttackPoint attackPoint, uint32 attackedPlayerIndex = 0);

	void ThrowCarrot();

private:
	PlayerInfo* _playerInfo = nullptr;

private:
	float _moveSpeed;
	int _moveDirection;	// 1~9 방향을 철권처럼 지정
	float _timer;
	float _posY;
	float _deltaTime;
	float _jumpCoolTime;
	float _shootCoolTime;
	bool _isOnGround;
	bool _isDebugMode;
	bool _isSetting;
	bool _isReloaded = false;
	float _reloadTime;
	int _jumpCount;
	int _jumpMax;
	float _carrotTime;

public:
	// 보간 완료
	RocketEngine::RMFLOAT3 remotePosition;
	RocketEngine::RMQuaternion remoteQuaternion;

private:
	RocketEngine::Camera* _playerCamera = nullptr;
	RocketEngine::BoxCollider* _playerCollider = nullptr;
	//RocketEngine::CapsuleCollider* _playerCollider = nullptr;
	RocketEngine::AudioClip* _playerAudio = nullptr;
	RocketEngine::AnimationController* _playerAnim = nullptr;
	RocketEngine::AnimationController* _gunAnim[4];
};

