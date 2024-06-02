#include "Camera.h"
#include <cmath>
#include "GameObject.h"
#include "Transform.h"
#include "..\\RocketMath\\RocketMath.h"
#include <windows.h>	// 디버그용으로 열었음
#include "RenderSystem.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\RocketMath.lib")
#endif

namespace RocketEngine
{
	Camera::Camera(GameObject* owner)
		: Component(owner),
		/*_nearZ(0.01f),*/
		_nearZ(0.01f),
		_farZ(1000.0f),
		/*_farZ(300.0f),*/
		_aspect(16.0f / 9.0f),
		_fovY(70.0f),
		_nearWindowHeight(),
		_farWindowHeight(),
		_isShakeCameraOnHit(false), _isShakeCameraOnShoot(false),
		_shakeDurationOnHit(0.1f), _shakeDurationOnShoot(0.1f),
		_distX(0.0f),_distY(0.0f), _distYOnShoot(0.0f)
	{

	}

	void Camera::Start()
	{

	}

	RocketCore::Graphics::CameraData& Camera::GetCameraData()
	{
// 		float screenWidth = (float)RocketCore::RenderSystem::Instance().GetScreenWidth();
// 		float screenHeight = (float)RocketCore::RenderSystem::Instance().GetScreenHeight();
// 		_aspect = screenWidth / screenHeight;

		
		_cameraData.nearZ = _nearZ;
		_cameraData.farZ = _farZ;
		_cameraData.aspect = _aspect;
		_cameraData.fovY = _fovY;
		_cameraData.nearWindowHeight = _nearWindowHeight;
		_cameraData.farWindowHeight = _farWindowHeight;

		_cameraData.position = gameObject->transform.GetPosition();
		_cameraData.rotation = gameObject->transform.GetRotation();

		return _cameraData;
	}

	float Camera::GetNearZ() const
	{
		return _nearZ;
	}

	float Camera::GetFarZ() const
	{
		return _farZ;
	}

	float Camera::GetAspect() const
	{
		return _aspect;
	}

	float Camera::GetFovX() const
	{
		return RMConvertToDegrees(GetRadianFovX());
	}

	float Camera::GetRadianFovX() const
	{
		float halfWidth = GetNearWindowWidth() / 2;
		return 2.0f * atan(halfWidth / _nearZ);
	}

	float Camera::GetFovY() const
	{
		return _fovY;
	}

	float Camera::GetRadianFovY() const
	{
		return RMConvertToRadians(_fovY);
	}

	float Camera::GetNearWindowWidth() const
	{
		return _aspect * _nearWindowHeight;
	}

	float Camera::GetNearWindowHeight() const
	{
		return _nearWindowHeight;
	}

	float Camera::GetFarWindowWidth() const
	{
		return _aspect * _farWindowHeight;
	}

	float Camera::GetFarWindowHeight() const
	{
		return _farWindowHeight;
	}

	RMFLOAT4X4 Camera::GetViewMatrix() const
	{
		return _viewMatrix;
	}

	RMFLOAT4X4 Camera::GetProjMatrix() const
	{
		return _projMatrix;
	}

	void Camera::SetNearZ(float nearZ)
	{
		_nearZ = nearZ;
		_cameraData.nearZ = nearZ;
	}

	void Camera::SetFarZ(float farZ)
	{
		_cameraData.farZ = farZ;
	}

	void Camera::SetAspect(float aspect)
	{	
		_cameraData.aspect = aspect;
	}

	void Camera::SetFovY(float fovY)
	{
		_cameraData.fovY = fovY;
	}

	void Camera::Strafe(float delta)
	{
		RMFLOAT3 rightVec = gameObject->transform.GetRight();
		rightVec.x *= delta;
		rightVec.y *= delta;
		rightVec.z *= delta;

		gameObject->transform.Translate(rightVec);
	}

	void Camera::Walk(float delta)
	{
		RMFLOAT3 forwardVec = gameObject->transform.GetForward();
		forwardVec.x *= delta;
		forwardVec.y *= delta;
		forwardVec.z *= delta;

		gameObject->transform.Translate(forwardVec);
	}

	void Camera::WorldUpDown(float delta)
	{
		RMFLOAT3 worldUpDelta = { 0.0f,delta,0.0f };
		gameObject->transform.Translate(worldUpDelta);
	}

	void Camera::Pitch(float radian)
	{
		//RMQuaternion newRot = RMRotateQuaternion(gameObject->transform.GetLocalRotation(), gameObject->transform.GetRight(), angle);
		RMFLOAT4 r{ 1.0f,0.0f,0.0f,1.0f };
		r = RMFloat4MultiplyMatrix(r, gameObject->transform.GetLocalRotationMatrix());
		RMQuaternion newRot = RMRotateQuaternion(gameObject->transform.GetLocalRotation(), { r.x,r.y,r.z }, radian);
		gameObject->transform.SetLocalRotation(newRot);
	}

	void Camera::RotateY(float angle)
	{
		RMQuaternion newRot = RMRotateQuaternion(gameObject->transform.GetLocalRotation(), { 0.0f,1.0f,0.0f }, angle);
		gameObject->transform.SetLocalRotation(newRot);		
	}

	void Camera::ShakeCameraUpdateOnHit(float deltaTime)
	{
		if (!_isShakeCameraOnHit)
		{
			return;
		}

		float shakeIntensity = 0.01f;
		float shakeFrequency = 120.0f;

		if (_shakeDurationOnHit > 0.0f)
		{
			gameObject->transform.Translate(-_distX, -_distY, 0.0f);
			//_viewMatrix._41 += shakeIntensity * sin(shakeFrequency * _shakeDuration);
			//_viewMatrix._42 += shakeIntensity * cos(shakeFrequency * _shakeDuration);
			_distX = shakeIntensity * sin(shakeFrequency * _shakeDurationOnHit);
			_distY = shakeIntensity * cos(shakeFrequency * _shakeDurationOnHit);
			gameObject->transform.Translate(_distX, _distY, 0.0f);

			_shakeDurationOnHit -= deltaTime;
		}
		else
		{
			gameObject->transform.Translate(-_distX, -_distY, 0.0f);
			_shakeDurationOnHit = 0.1f;
			_isShakeCameraOnHit = false;
			_distX = 0.0f;
			_distY = 0.0f;
		}
	}

	void Camera::ShakeCameraUpdateOnShoot(float deltaTime)
	{
		if (!_isShakeCameraOnShoot)
		{
			return;
		}

		float shakeIntensity = 0.03f;
		float shakeFrequency = 90.0f;

		if (_shakeDurationOnShoot > 0.0f)
		{
			gameObject->transform.Translate(0.0f, -_distYOnShoot, 0.0f);
			//_viewMatrix._41 += shakeIntensity * sin(shakeFrequency * _shakeDuration);
			//_viewMatrix._42 += shakeIntensity * cos(shakeFrequency * _shakeDuration);
			_distYOnShoot = shakeIntensity * cos(shakeFrequency * _shakeDurationOnShoot);
			gameObject->transform.Translate(0.0f, _distYOnShoot, 0.0f);

			_shakeDurationOnShoot -= deltaTime;
		}
		else
		{
			gameObject->transform.Translate(0.0f, -_distYOnShoot, 0.0f);
			_shakeDurationOnShoot = 0.1f;
			_isShakeCameraOnShoot = false;
			_distYOnShoot = 0.0f;
		}
	}

	void Camera::SetIsCameraShakeOnHit(bool cameraShake)
	{
		_isShakeCameraOnHit = cameraShake;
	}

	void Camera::SetIsCameraShakeOnShoot(bool cameraShake)
	{
		_isShakeCameraOnShoot = cameraShake;
	}

}
