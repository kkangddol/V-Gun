#include "PlayerMove.h"
#include "PlayerInfo.h"
#include "Struct.pb.h"
#include <cmath>
#include "GameManager.h"
#include "NetworkManager.h"

PlayerMove::PlayerMove(RocketEngine::GameObject* owner)
	: RocketEngine::Component(owner),
	_moveSpeed(80.0f), _moveDirection(5), _timer(0.0f), _deltaTime(0.0f), _jumpCoolTime(0.0f),
	_isOnGround(true), _isDebugMode(false), _isSetting(false),
	_shootCoolTime(0.5f), _jumpCount(0), _jumpMax(1), _carrotTime(0.0f)
{

}

void PlayerMove::Start()
{
	_playerInfo = gameObject->GetComponent<PlayerInfo>();

	if (_playerInfo->playerId == GameManager::Instance().myId)
	{
		auto camObj = gameObject->transform.GetChild(2)->gameObject;
		_playerCamera = camObj->GetComponent<RocketEngine::Camera>();
	}

	_playerCollider = gameObject->GetComponent<RocketEngine::BoxCollider>();
	//_playerCollider = gameObject->GetComponent<RocketEngine::CapsuleCollider>();
	_playerAudio = gameObject->GetComponent<RocketEngine::AudioClip>();

	//[도토리도토리]
	_playerAnim = gameObject->GetComponent<RocketEngine::AnimationController>();
	_playerAnim->onStateChanged = [this]() { this->OnStateChanged(); };

	for (auto& child : gameObject->transform.GetChildrenVec())
	{
		if (child->gameObject->objName == "playerHead")
		{
			for (auto& grandChild : child->gameObject->transform.GetChildrenVec())
			{
				if (grandChild->gameObject->objName == "MainCamera")
				{
					_playerCamera = grandChild->gameObject->GetComponent<RocketEngine::Camera>();
				}
			}
		}
	}

}

void PlayerMove::Update()
{
	if (GameManager::Instance().IsGameEnd())
	{
		return;
	}

	_deltaTime = RocketEngine::GetDeltaTime();

	if (_playerInfo->playerId != GameManager::Instance().myId)
	{
		// 		gameObject->transform.SetPosition(InterpolationPosition(gameObject->transform.GetPosition(), remotePosition, 1));
		// 		gameObject->transform.SetRotation(InterpolationRotation(gameObject->transform.GetRotation(), remoteQuaternion, 0.3));

		gameObject->transform.SetPosition(remotePosition);
		gameObject->transform.SetRotation(remoteQuaternion);

		/*RocketEngine::Tween* pos = RocketEngine::CreateTween();
		pos->GetData(&gameObject->transform.GetLocalPositionRef()).DoMove(posStep, 1).SetEase(RocketEngine::INSINE);*/

		//auto rotationStep = InterpolationRotation(gameObject->transform.GetLocalRotation(), remoteQuaternion, 0.3);

		/*RocketEngine::Tween* rot = RocketEngine::CreateTween();
		rot->GetData(&gameObject->transform.GetLocalRotationRef()).DoMove(rotationStep, 0.3).SetEase(RocketEngine::INSINE);*/
		return;
	}

	// Todo 플레이 중이 아닐때와 죽었을때 입력 못받게 여기서 처리하는게 맞나?
	if (GameManager::Instance().state == Protocol::ROUND_STATE_PLAYING)
	{
		TogglePlayerDebugMode();

		AdjustSetting();

		if (_isSetting || _playerInfo->isDie)
		{
			return;
		}
		else
		{
			PitchCamera();
			RotateOnMouseMove();

			PlayPlayerSound();

			_playerCamera->ShakeCameraUpdateOnHit(_deltaTime);
			_playerCamera->ShakeCameraUpdateOnShoot(_deltaTime);

			Control();
		}
	}

	//AdjustSetting();

	//if (_isSetting || _playerInfo->isDie)
	//{
	//	return;
	//}

	//PitchCamera();
	//RotateOnMouseMove();
	//_playerCamera->ShakeCameraUpdateOnHit(_deltaTime);
	//_playerCamera->ShakeCameraUpdateOnShoot(_deltaTime);

	//TogglePlayerDebugMode();

	//PlayPlayerSound();

	//Control();
}

void PlayerMove::LateUpdate()
{
	/*
	if (_jumpCoolTime > 0.0f)
	{
		if (_isOnGround)
		{
			_jumpCoolTime -= _deltaTime * 5.0f;
		}
		else
		{
			_jumpCoolTime -= _deltaTime;
		}
	}
	else if (!_isOnGround)
	{
		_isOnGround = true;
		_playerAudio->Play3DOnce("landing");
		_jumpCount = 0;
	}
	*/

	if (_shootCoolTime > 0.0f)
	{
		_shootCoolTime -= _deltaTime;
	}

	if (_timer > 0.0f)
	{
		_timer -= _deltaTime;
		return;
	}

	if (_isReloaded == false)
	{
		_reloadTime += _deltaTime;

		if (_reloadTime > _playerInfo->stats.reloadTime)
		{
			_isReloaded = true;

			this->_playerInfo->currentAmmoCount = this->_playerInfo->stats.bulletCapacity;

			_playerAnim->SetCurrentState("Idle");
			_reloadTime = 0;
		}
	}

	CheckIsOnGround();
}

bool PlayerMove::CheckIsOnGround()
{
	//if (_jumpCoolTime > 0.4f)
	//{
	//	return false;
	//}
	// 0.25f;
	RocketEngine::RMFLOAT3 pos = gameObject->transform.GetPosition();
	const float delta = 0.2f;
	float x[9] = { -delta, -delta,0, delta,delta,delta,0,-delta,0 };
	float z[9] = { 0,delta,delta,delta,0,-delta,-delta,-delta,0 };

	for (int i = 0; i < 9; ++i)
	{
		//RocketEngine::RMFLOAT4 worldPos = RMFloat4MultiplyMatrix(RocketEngine::RMFLOAT4(pos.x + x[i], pos.y, pos.z + z[i], 1.0f), gameObject->transform.GetWorldTM());
		RocketEngine::RMFLOAT4 worldPos = RocketEngine::RMFLOAT4(pos.x + x[i], pos.y + 0.01f * i, pos.z + z[i], 1.0f);
		RocketEngine::RMFLOAT4 eachDir = worldPos;
		eachDir.y -= 0.05f;

		int type = 0;
		RocketEngine::Collider* temp = RocketEngine::ShootRay({ worldPos.x, worldPos.y, worldPos.z }, { 0.0f,-1.0f,0.0f }, 0.05f, &type);
		RocketEngine::DrawDebugLine({ worldPos.x,worldPos.y,worldPos.z }, { eachDir.x,eachDir.y,eachDir.z });

		if (temp)
		{
			// type 1이 rigidStatic.
			if (type == 1)
			{
				// 상태 변경 및 착지 Sound.
				if (_isOnGround == false)
				{
					_isOnGround = true;
					_playerAudio->Play3DOnce("landing");
					_jumpCount = 0;
				}
				return true;
			}
		}
	}
	return false;
}

void PlayerMove::TogglePlayerDebugMode()
{
	if (RocketEngine::GetKeyDown('M'))
	{
		if (!_isDebugMode)
		{
			_playerCollider->DisableSimulation();
			_isDebugMode = true;
		}
		else
		{
			// 씬에 등록하는 부분
			_playerCollider->EnableSimulation();
			_isDebugMode = false;
		}
	}
}

void PlayerMove::UpdateMoveDirection()
{
	// 키 입력 없으면 중립
	_moveDirection = 5;

	/// 오수안 애니메이션 주석
	gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isIdle", true);

	if (RocketEngine::GetKey('W'))
	{
		// idle -> walk
		std::string test = gameObject->GetComponent<RocketEngine::AnimationController>()->GetCurrentState()->GetAnimName();
		gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isMove", true);
		test = gameObject->GetComponent<RocketEngine::AnimationController>()->GetCurrentState()->GetAnimName();
		_moveDirection = 8;
	}

	if (RocketEngine::GetKey('S'))
	{
		// idle -> walk
		std::string test = gameObject->GetComponent<RocketEngine::AnimationController>()->GetCurrentState()->GetAnimName();
		gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isMove", true);
		test = gameObject->GetComponent<RocketEngine::AnimationController>()->GetCurrentState()->GetAnimName();
		_moveDirection = 2;
		if (RocketEngine::GetKey('W'))
		{
			_moveDirection = 5;
		}
	}

	if (RocketEngine::GetKey('A'))
	{
		// idle -> Left step
		gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isSideStepLeft", true);
		_moveDirection = 4;
		if (RocketEngine::GetKey('W'))
		{
			// leftstep -> walk
			gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isMove", true);
			_moveDirection = 7;
		}

		if (RocketEngine::GetKey('S'))
		{
			// leftstep -> walk
			gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isMove", true);
			_moveDirection = 1;
		}
	}

	if (RocketEngine::GetKey('D'))
	{
		// idle -> Left step
		gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isSideStepRight", true);
		_moveDirection = 6;
		if (RocketEngine::GetKey('W'))
		{
			// rightstep -> walk
			gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isMove", true);
			_moveDirection = 9;
		}

		if (RocketEngine::GetKey('A'))
		{
			// right step -> left
			gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isSideStepLeft", true);
			_moveDirection = 5;
		}

		if (RocketEngine::GetKey('S'))
		{
			// rightstep -> walk
			gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isMove", true);
			_moveDirection = 3;
		}
	}
}

void PlayerMove::Move(int direction)
{
	float speed;
	RocketEngine::RMFLOAT3 delta = { 0.0f,0.0f,0.0f };


	switch (direction)
	{
		case 1:
		{
			speed = _moveSpeed * 0.5f;
			speed *= _playerInfo->stats.moveSpeed;
			delta =
				gameObject->transform.GetRight() * -speed * _deltaTime +
				gameObject->transform.GetForward() * -speed * _deltaTime;
		}
		break;

		case 2:
		{
			speed = _moveSpeed * 0.7f;
			speed *= _playerInfo->stats.moveSpeed;
			delta =
				gameObject->transform.GetForward() * -speed * _deltaTime;
		}
		break;

		case 3:
		{
			speed = _moveSpeed * 0.5f;
			speed *= _playerInfo->stats.moveSpeed;
			delta =
				gameObject->transform.GetRight() * speed * _deltaTime +
				gameObject->transform.GetForward() * -speed * _deltaTime;
		}
		break;

		case 4:
		{
			speed = _moveSpeed * 0.7f;
			speed *= _playerInfo->stats.moveSpeed;
			delta =
				gameObject->transform.GetRight() * -speed * _deltaTime;
		}
		break;

		case 5:
			break;

		case 6:
		{
			speed = _moveSpeed * 0.7f;
			speed *= _playerInfo->stats.moveSpeed;
			delta =
				gameObject->transform.GetRight() * speed * _deltaTime;
		}
		break;

		case 7:
		{
			speed = _moveSpeed * 0.7f;
			speed *= _playerInfo->stats.moveSpeed;
			delta =
				gameObject->transform.GetRight() * -speed * _deltaTime +
				gameObject->transform.GetForward() * speed * _deltaTime;
		}
		break;

		case 8:
		{
			speed = _moveSpeed;
			speed *= _playerInfo->stats.moveSpeed;
			delta =
				gameObject->transform.GetForward() * speed * _deltaTime;
		}
		break;

		case 9:
		{
			speed = _moveSpeed * 0.7f;
			speed *= _playerInfo->stats.moveSpeed;
			delta =
				gameObject->transform.GetRight() * speed * _deltaTime +
				gameObject->transform.GetForward() * speed * _deltaTime;
		}
	}

	if (!_isDebugMode)
	{
		switch (direction)
		{
			// 왼쪽
			case 1:
			case 4:
				_playerAnim->SetCurrentState("MoveLeft");
				break;
				// 오른쪽
			case 3:
			case 6:
				_playerAnim->SetCurrentState("MoveRight");
				break;
				// 앞,뒤
			case 2:
			case 7:
			case 8:
			case 9:
				_playerAnim->SetCurrentState("MoveForward");
				break;
				// 정지
			case 5:
				_playerAnim->SetCurrentState("Idle");
				break;

			default:
				break;
		}

		// Physics로 움직이기!
		_playerCollider->AddVelocity(delta);
	}
	else
	{
		//Translate으로 움직이기!
		gameObject->transform.Translate(delta);

		if (RocketEngine::GetKey('Q'))
		{
			speed = _moveSpeed * 0.3f * _deltaTime;
			RocketEngine::RMFLOAT3 worldUpDelta = { 0.0f,-speed,0.0f };
			gameObject->transform.Translate(worldUpDelta);
		}

		if (RocketEngine::GetKey('E'))
		{
			speed = _moveSpeed * 0.3f * _deltaTime;
			RocketEngine::RMFLOAT3 worldUpDelta = { 0.0f,speed,0.0f };
			gameObject->transform.Translate(worldUpDelta);
		}
	}
}

void PlayerMove::Jump()
{
	//if (_jumpCoolTime > 0.8f * _jumpMax || _jumpCount >= _jumpMax)
	if (_jumpCount >= _playerInfo->stats.ableJumpCount + 1)
	{
		return;
	}

	_timer = 0.2f;
	_jumpCoolTime = 1.0 * (_playerInfo->stats.ableJumpCount + 1);
	++_jumpCount;
	_isOnGround = false;
	_playerCollider->AddForce({ 0.0f, 40.0f, 0.0f }, RocketEngine::eForceMode::eIMPULSE);
	_playerAudio->Play3DOnce("jump");
	// 오수안 애니메이션 추가 주석
	gameObject->GetComponent<RocketEngine::AnimationController>()->SetCondition("isJump", true);


	// 	_timer = 0.0f;
	// 		
	// 	_timer += _deltaTime;
	// 	DoJump();

	// 	if (_isOnGround)
	// 	{
	// 		RocketEngine::RMFLOAT3 curVel = _playerCollider->GetVelocity();
	// 		curVel.y = 0.0f;
	// 		_playerCollider->SetVelocity(curVel);
	// 	}
}

void PlayerMove::PlayPlayerSound()
{
	// 걷는 Sound.
	if (_moveDirection != 5 && _isOnGround == true)
	{
		if (_playerAudio->IsSoundPlaying("walk02") == false)
		{
			_playerAudio->Play3DOnce("walk02");
		}
	}
}

void PlayerMove::Control()
{
	/*if (GameManager::Instance().state != Protocol::ROUND_STATE_PLAYING)
		return;*/

	UpdateMoveDirection();

	Move(_moveDirection);

	if (RocketEngine::GetKeyDown(VK_LBUTTON))
	{
		ShootGun(_playerInfo);
	}

	if (RocketEngine::GetKeyDown(VK_SPACE))
	{
		// Jump!!
		Jump();
	}

	ThrowCarrot();
}

void PlayerMove::AdjustSetting()
{
	if (RocketEngine::GetKeyDown(VK_ESCAPE))
	{
		// TODO : 마우스 뜨게 해야함.
		_isSetting = !_isSetting;
		RocketEngine::PlayAudioOnceNested("..\\Sound\\ButtonClick\\button_push.wav", 1.0f);

		if (_isSetting)
		{
			GameManager::Instance().Setting();
		}
		else
		{
			GameManager::Instance().ExitSetting();
		}
	}
}

void PlayerMove::RotateOnMouseMove()
{
	//RocketEngine::RMFLOAT2 mouseDelta = RocketEngine::GetMouseDelta();
	RocketEngine::RMFLOAT2 mouseDelta = RocketEngine::MouseDeltaSmooth();
	RotateY(mouseDelta.x * 0.1f * RocketEngine::GetDeltaTime());
}

void PlayerMove::RotateY(float angle)
{
	RocketEngine::RMQuaternion newRot = RMRotateQuaternion(gameObject->transform.GetLocalRotation(), { 0.0f,1.0f,0.0f }, angle);
	gameObject->transform.SetLocalRotation(newRot);
}


void PlayerMove::PitchCamera()
{
	/// Try.0
	RocketEngine::RMFLOAT2 mouseDelta = RocketEngine::GetMouseDelta();
	_playerCamera->Pitch(mouseDelta.y * RocketEngine::GetDeltaTime() * 0.5f);

	/// Tryseok
	RocketEngine::RMFLOAT3 euler = _playerCamera->gameObject->transform.GetLocalEuler();
	if (89.0f < euler.x)
	{
		_playerCamera->gameObject->transform.SetLocalRotationEuler(89.0f, 0.0f, 0.0f);
	}
	else if (euler.x < -89.0f)
	{
		_playerCamera->gameObject->transform.SetLocalRotationEuler(-89.0f, 0.0f, 0.0f);
	}
}

RocketEngine::Camera* PlayerMove::GetPlayerCamera()
{
	return _playerCamera;
}

RocketEngine::AnimationController* PlayerMove::GetPlayerAnim()
{
	return _playerAnim;
}

void PlayerMove::OnStateChanged()
{
	// [도토리도토리]
	std::string stateName = _playerAnim->GetCurrentState()->GetStateName();

	if (stateName == "Idle")
	{
		_playerInfo->animState = Protocol::ANIM_IDLE;
	}

	if (stateName == "Die")
	{
		_playerInfo->animState = Protocol::ANIM_DIE;
	}

	if (stateName == "Jump")
	{
		_playerInfo->animState = Protocol::ANIM_DIE;
	}

	if (stateName == "MoveForward")
	{
		_playerInfo->animState = Protocol::ANIM_MOVE_FORWARD;
	}

	if (stateName == "MoveLeft")
	{
		_playerInfo->animState = Protocol::ANIM_MOVE_LEFT;
	}

	if (stateName == "MoveRight")
	{
		_playerInfo->animState = Protocol::ANIM_MOVE_RIGHT;
	}

	if (stateName == "Reload")
	{
		_playerInfo->animState = Protocol::ANIM_RELOAD;
	}

	if (stateName == "Shoot")
	{
		_playerInfo->animState = Protocol::ANIM_SHOOT;
	}

	if (GameManager::Instance().myId == _playerInfo->playerId)
	{
		NetworkManager::Instance().SendChangeAnimationState(_playerInfo->animState);
	}
}

void PlayerMove::AssignGunAnim(int index, RocketEngine::AnimationController* gunAnim)
{
	_gunAnim[index] = gunAnim;
}

RocketEngine::RMFLOAT3 PlayerMove::InterpolationPosition(RocketEngine::RMFLOAT3 currentPos, RocketEngine::RMFLOAT3 servPos, float intermediateValue)
{
	RocketEngine::RMFLOAT3 interpolated;

	// 점과 점 사이의 거리 구하기
	float distance = std::sqrt(
		(servPos.x - currentPos.x) * (servPos.x - currentPos.x) +
		(servPos.y - currentPos.y) * (servPos.y - currentPos.y) +
		(servPos.z - currentPos.z) * (servPos.z - currentPos.z)
	);

	// 가중치
	float weight = 1.0 - std::exp(-distance * 0.1f);

	interpolated.x = currentPos.x + (servPos.x - currentPos.x) * intermediateValue * weight;
	interpolated.y = currentPos.y + (servPos.y - currentPos.y) * intermediateValue * weight;
	interpolated.z = currentPos.z + (servPos.z - currentPos.z) * intermediateValue * weight;

	return interpolated;
}

RocketEngine::RMQuaternion PlayerMove::InterpolationRotation(RocketEngine::RMQuaternion currentRot, RocketEngine::RMQuaternion servRot, float intermediateValue)
{
	RocketEngine::RMQuaternion interpolated;

	// 점과 점 사이의 거리 구하기
	float distance = std::sqrt(
		(servRot.w - currentRot.w) * (servRot.w - currentRot.w) +
		(servRot.x - currentRot.x) * (servRot.x - currentRot.x) +
		(servRot.y - currentRot.y) * (servRot.y - currentRot.y) +
		(servRot.z - currentRot.z) * (servRot.z - currentRot.z)
	);

	// 가중치
	float weight = 1.0 - std::exp(-distance * 0.1f);

	interpolated.w = currentRot.w + (servRot.w - currentRot.w) * intermediateValue;
	interpolated.x = currentRot.x + (servRot.x - currentRot.x) * intermediateValue;
	interpolated.y = currentRot.y + (servRot.y - currentRot.y) * intermediateValue;
	interpolated.z = currentRot.z + (servRot.z - currentRot.z) * intermediateValue;

	return interpolated;
}

RocketEngine::RMFLOAT3 PlayerMove::InterpolationPositionXZ(RocketEngine::RMFLOAT3 currentPos, RocketEngine::RMFLOAT3 servPos, float intermediateValue)
{
	RocketEngine::RMFLOAT3 interpolated;

	// 점과 점 사이의 거리 구하기
	float distance = std::sqrt(
		(servPos.x - currentPos.x) * (servPos.x - currentPos.x) +
		(servPos.y - currentPos.y) * (servPos.y - currentPos.y) +
		(servPos.z - currentPos.z) * (servPos.z - currentPos.z)
	);

	// 가중치
	float weight = 1.0 - std::exp(-distance * 0.1f);

	interpolated.x = currentPos.x + (servPos.x - currentPos.x) * intermediateValue * weight;
	interpolated.y = currentPos.y;
	interpolated.z = currentPos.z + (servPos.z - currentPos.z) * intermediateValue * weight;

	return interpolated;
}

// Todo 장전 함수 만들기

void PlayerMove::ShootGun(PlayerInfo* shooterInfo, RocketEngine::RMFLOAT3 pos /*={0.0f,0.0f,0.0f}*/, RocketEngine::RMQuaternion rot /*={1.0f,0.0f,0.0f,0.0f}*/)
{
	if (_shootCoolTime > 0.01f)
	{
		return;
	}

	if (shooterInfo->IsMine() == true)
	{
		/// 총알이 0 이하일 때
		if (shooterInfo->currentAmmoCount <= 0)
		{
			/// [도토리도토리] : Reload랑 Idle 이거 State 이름이 원인이었다면 이름 어떻게든 할것.
			// 시간을 누적하는 동안 장전 애니메이션
			if (_isReloaded == true);
			{
				_isReloaded = false;

				_playerAudio->Play3DOnce("reload");

				_playerAnim->SetCurrentState("Reload");

				return;
			}
		}
		shooterInfo->currentAmmoCount--;

	}

	if (GameManager::Instance().myId == shooterInfo->playerId)
	{

		//_shootCoolTime = this->_playerInfo->stats.fireRate;
		_shootCoolTime = 0.5f;

		// 쏘는 소리 재생
		_playerAudio->Play3DOnce("shoot");

		// 쏘는 애니메이션 재생
		_gunAnim[_playerInfo->GetGunIndex()]->SetCurrentState("Shoot");

		// 카메라 흔들기
		_playerCamera->SetIsCameraShakeOnShoot(true);

		// 당근 슈웅
		_carrotTime = 0.15f;

		//반동
// 		_recoilTween = RocketEngine::CreateTween();
// 		RocketEngine::RMQuaternion originRot = _playerCamera->gameObject->transform.GetLocalRotation();
// 
// 		RocketEngine::RMFLOAT4 r{ 1.0f, 0.0f, 0.0f, 1.0f };
// 		r = RMFloat4MultiplyMatrix(r, _playerCamera->gameObject->transform.GetLocalRotationMatrix());
// 		RocketEngine::RMQuaternion targetRot = RMRotateQuaternion(_playerCamera->gameObject->transform.GetLocalRotation(), { r.x,r.y,r.z }, -0.1f);
// 
// 		_recoilTween->GetData(&(_playerCamera->gameObject->transform.GetLocalRotationRef()))
// 			.DoMove(targetRot, 0.5f)
// 			.SetEase(RocketEngine::OUTEXPO)
// 			.OnComplete([originRot, this]()
// 				{
// 					RocketEngine::CreateTween()
// 					->GetData(&(this->_playerCamera->gameObject->transform.GetLocalRotationRef()))
// 					.DoMove(originRot, 1.0f)
// 					.SetEase(RocketEngine::OUTQUAD);
// 				});

		RocketEngine::RMFLOAT3 origin = _playerCamera->gameObject->transform.GetPosition();
		//RocketEngine::RMFLOAT3 origin = gameObject->transform.GetPosition();
		RocketEngine::RMFLOAT3 dir = _playerCamera->gameObject->transform.GetForward();
		RocketEngine::RMFLOAT3 right = _playerCamera->gameObject->transform.GetRight();
		RocketEngine::RMFLOAT3 up = _playerCamera->gameObject->transform.GetUp();

		// 쏘는 것 개선 시도
		// origin에서 y방향 아래로 k만큼 내린 newOrigin을 만들고
		// direction에서 y방향으로 k만큼 올린 newDirection을 만들자
		//RocketEngine::RMFLOAT3 newOrigin = origin + RocketEngine::RMFLOAT3{0.05f, -0.05f, 0.0f};
		//RocketEngine::RMFLOAT3 newDir = dir + RocketEngine::RMFLOAT3{0.0f, 0.0f, 0.0f};

		// Ray 쏴서 처리.
		//RocketEngine::DynamicCollider* col = dynamic_cast<RocketEngine::DynamicCollider*>(RocketEngine::ShootRay(origin + (dir * 0.35f), dir, 100.0f, &type));
		RocketEngine::DynamicCollider* col = nullptr;
		RocketEngine::Collider* tempCol = nullptr;
		int type = 0;

		//RocketEngine::RMFLOAT3 newDir = dir + up * 0.01f - right * 0.01f;
		// RigidStatic에 맞으면 Ray를 몇 발 더 쏘도록 변경 23.8.23.AJY.
		for (int i = -2; i < 3; ++i)
		{
			for (int j = -2; j < 3; ++j)
			{
				RocketEngine::RMFLOAT3 newOrigin = origin + RocketEngine::RMFLOAT3{0.0f, +0.11f * j, 0.0f} + right * -0.11f * i;
				tempCol = RocketEngine::ShootRay(newOrigin + (dir * 0.35f), dir, 1000.0f, &type);
				//RocketEngine::DrawDebugLine(newOrigin, newOrigin + dir * 10.0f);

				if (tempCol == nullptr)
				{
					continue;
				}
			}
			if (tempCol == nullptr)
			{
				type = 0;
			}
			else
			{
				auto name = tempCol->gameObject->objName;

				if (name == "player"
					|| name == "playerBody"
					|| name == "playerHead"
					|| name == "player1"
					|| name == "player2"
					|| name == "player3"
					|| name == "player4"
					|| name == "playerBody1"
					|| name == "playerBody2"
					|| name == "playerBody3"
					|| name == "playerBody4"
					|| name == "playerHead1"
					|| name == "playerHead2"
					|| name == "playerHead3"
					|| name == "playerHead4")
				{
					break;
				}
			}
		}

		if (type == 2)
		{
			col = dynamic_cast<RocketEngine::DynamicCollider*>(tempCol);
		}

		if (col == nullptr)
		{
			return;
		}

		// 맞춘 녀석 접근해서 데미지 주고 서버에 알려야함.
		for (int i = 1; i <= 4; ++i)
		{
			RocketEngine::PlayAudioOnceNested("../Sound/Hit/hit_water.wav", 1.0f);

			if (col->gameObject->objName == "playerHead" + std::to_string(i) || col->gameObject->objName == "player" + std::to_string(i))
			{
				ClickAttack(Protocol::ATTACK_POINT_HEAD, i);
				return;
			}
			else if (col->gameObject->objName == "playerBody" + std::to_string(i))
			{
				ClickAttack(Protocol::ATTACK_POINT_BODY, i);
				return;
			}
		}

		ClickAttack(Protocol::ATTACK_POINT_NONE);

		// 수박 날리기
		col->AddForce(dir * 15.0f, RocketEngine::eForceMode::eIMPULSE);
	}
	else
	{
		/// 내가 아니라면
		// 위치랑 방향 들어온 매개변수로 SET해주고
		// 쏘는 모양 보여주면 된다.
		shooterInfo->gameObject->transform.SetPosition(pos);
		shooterInfo->gameObject->transform.SetRotation(rot);

		// 쏘는 애니메이션 재생
		auto shooterAnim = shooterInfo->gameObject->GetComponent<RocketEngine::AnimationController>();
		assert(shooterAnim);
		if (shooterAnim != nullptr)
		{
			shooterAnim->SetCurrentState("Shoot");
		}
	}
}

void PlayerMove::ClickAttack(Protocol::AttackPoint attackPoint, uint32 attackedPlayerIndex /*= 0*/)
{
	if (this->_playerInfo->IsMine() == false)
		return;

	// Todo 
	// attack point 다른 값으로 넘기고 여기서 enum 으로 바꿔서 보내도 됨

	auto transform = this->gameObject->transform;
	if (attackPoint == Protocol::ATTACK_POINT_NONE)
	{
		NetworkManager::Instance().SendAttackPlayer(transform.GetPosition(), transform.GetRotation());
	}
	else
	{
		auto player = GameManager::Instance().GetPlayerInfo(attackedPlayerIndex);

		NetworkManager::Instance().SendAttackPlayer(transform.GetPosition(), transform.GetRotation(), player->playerId, player->playerIndex, attackPoint);
	}
}

void PlayerMove::ThrowCarrot()
{
	if (_carrotTime > 0.001f)
	{
		if (_carrotTime < 0.02f)
		{
			_playerCamera->gameObject->transform.GetChild(0)->SetLocalPosition(0.1f, -100.0f, 1.1f);
		}
		else
		{
			RocketEngine::RMFLOAT3 dir = _playerCamera->gameObject->transform.GetChild(0)->gameObject->transform.GetLocalPosition();
			dir += RocketEngine::RMFLOAT3{0.1f, -0.1f, 1.1f};
			_playerCamera->gameObject->transform.GetChild(0)->SetLocalPosition(dir);
		}
		_carrotTime -= _deltaTime;
	}
	else
	{
		_playerCamera->gameObject->transform.GetChild(0)->SetLocalPosition(0.1f, -0.1f, 1.1f);
	}
}
