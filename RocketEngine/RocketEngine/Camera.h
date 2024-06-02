#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "..\\RocketMath\\RocketMath.h"
#include "..\\RocketGraphicsInterface\\CameraData.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketEngine
{
	class GameObject;

	class ROCKET_API Camera final : public Component
	{
	public:
		Camera(GameObject* owner);

	protected:
		virtual void Start() override;

	public:
		RocketCore::Graphics::CameraData& GetCameraData();
		
	private:
		RocketCore::Graphics::CameraData _cameraData;

	public:
		float GetNearZ() const;
		float GetFarZ() const;
		float GetAspect() const;			// 카메라 비율 screen.width/screen.height
		float GetFovX() const;				// FovX값을 60분법으로 반환
		float GetRadianFovX() const;		// FovX값을 호도법으로 반환
		float GetFovY() const;				// FovY값을 60분법으로 반환
		float GetRadianFovY() const;		// FovY값을 호도법으로 반환

		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		RMFLOAT4X4 GetViewMatrix() const;	// 카메라의 로컬좌표'계'를 반환
		RMFLOAT4X4 GetProjMatrix() const;	// 카메라의 투영행렬을 반환

		void SetNearZ(float nearZ);
		void SetFarZ(float farZ);
		void SetAspect(float aspect);
		void SetFovY(float fovY);

	public:
		void Strafe(float delta);
		void Walk(float delta);
		void WorldUpDown(float delta);

		void Yaw(float angle);
		void Pitch(float radian);
		void Roll(float angle);
		void RotateY(float angle);

		void ShakeCameraUpdateOnHit(float deltaTime);
		void ShakeCameraUpdateOnShoot(float deltaTime);
		void SetIsCameraShakeOnHit(bool cameraShake);
		void SetIsCameraShakeOnShoot(bool cameraShake);

	private:
		float _nearZ;				// frustum의 가까운 평면까지의 거리
		float _farZ;				// frustum의 먼 평면까지의 거리
		float _aspect;				// 가로 / 세로 비율
		float _fovY;				// fov각도를 60분법으로 갖고있음
		float _nearWindowHeight;	// frustum의 가까운 평면의 높이
		float _farWindowHeight;		// frustum의 먼 평면의 높이

		RMFLOAT4X4 _viewMatrix;		// 카메라의 로컬좌표'계'
		RMFLOAT4X4 _projMatrix;		// 카메라의 투영 행렬

		bool _isShakeCameraOnHit;
		bool _isShakeCameraOnShoot;
		float _shakeDurationOnHit;
		float _shakeDurationOnShoot;
		float _distX;
		float _distY;
		float _distYOnShoot;
	};
}
