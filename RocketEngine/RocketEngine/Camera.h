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
		float GetAspect() const;			// ī�޶� ���� screen.width/screen.height
		float GetFovX() const;				// FovX���� 60�й����� ��ȯ
		float GetRadianFovX() const;		// FovX���� ȣ�������� ��ȯ
		float GetFovY() const;				// FovY���� 60�й����� ��ȯ
		float GetRadianFovY() const;		// FovY���� ȣ�������� ��ȯ

		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		RMFLOAT4X4 GetViewMatrix() const;	// ī�޶��� ������ǥ'��'�� ��ȯ
		RMFLOAT4X4 GetProjMatrix() const;	// ī�޶��� ��������� ��ȯ

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
		float _nearZ;				// frustum�� ����� �������� �Ÿ�
		float _farZ;				// frustum�� �� �������� �Ÿ�
		float _aspect;				// ���� / ���� ����
		float _fovY;				// fov������ 60�й����� ��������
		float _nearWindowHeight;	// frustum�� ����� ����� ����
		float _farWindowHeight;		// frustum�� �� ����� ����

		RMFLOAT4X4 _viewMatrix;		// ī�޶��� ������ǥ'��'
		RMFLOAT4X4 _projMatrix;		// ī�޶��� ���� ���

		bool _isShakeCameraOnHit;
		bool _isShakeCameraOnShoot;
		float _shakeDurationOnHit;
		float _shakeDurationOnShoot;
		float _distX;
		float _distY;
		float _distYOnShoot;
	};
}
