#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include <vector>

#if defined(_DX11) || defined(_DX12)
#include "..\\RocketMath\\RocketMath.h"
#endif // _DX11 || _DX12

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif //_DEBUG

namespace RocketEngine
{
	class GameObject;

	/// <summary>
	/// GameObject�� ���� Transform Ŭ����.
	/// 
	/// 23.06.29 ������ �����.
	/// </summary>
	class ROCKET_API Transform final : public Component
	{
		/// ������.
	public:
		Transform(GameObject* owner);		// ������ �� �� Entity�� �������� �����Ѵ�.

		/// Get World Position, Rotation, Scale
	public:
		RMFLOAT3 GetPosition() const;	// world ����
		RMQuaternion GetRotation() const;	// world ���� ���ʹϾ�
		RMFLOAT3 GetEuler() const;		// world ���� 60�й�
		RMFLOAT3 GetScale() const;		// world ����

		/// Set World Position, Rotation, Scale
	public:
		void SetPosition(const RMFLOAT3& position);
		void SetPosition(float x, float y, float z);
		void SetRotation(const RMQuaternion& quaternion);
		void SetRotation(float w, float x, float y, float z);
		void SetRotationEuler(const RMFLOAT3& euler);
		void SetRotationEuler(float angleX, float angleY, float angleZ);
		void SetScale(const RMFLOAT3& scale);
		void SetScale(float x, float y, float z);
		

		/// Tween���� ������ �����ϱ� ����.(�ӽ�)
		/// ������ �����ϱ� �� ���� ������ ��� �����غ���.
		/// 23.07.26 ������ && ������ �����
	public:
		RMFLOAT3& GetLocalPositionRef();
		RMQuaternion& GetLocalRotationRef();
		RMFLOAT3& GetLocalScaleRef();

		/// Get Local Position, Rotation, Scale ����
	public:
		RMFLOAT3 GetLocalPosition() const;
		RMQuaternion GetLocalRotation() const;
		RMFLOAT3 GetLocalEuler() const;
		RMFLOAT3 GetLocalScale() const;

	public:
		void SetLocalPosition(const RMFLOAT3& position);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalRotation(const RMQuaternion& quaternion);
		void SetLocalRotation(float w, float x, float y, float z);
		void SetLocalRotationEuler(const RMFLOAT3& euler);
		void SetLocalRotationEuler(float angleX, float angleY, float angleZ);
		void SetLocalRotationEulerXZConvert(float angleX, float angleY, float angleZ);
		void SetLocalScale(const RMFLOAT3& scale);
		void SetLocalScale(float x, float y, float z);

	public:
		/// Forwar,Up,Right vector (World ����)
		RMFLOAT3 GetForward() const;
		RMFLOAT3 GetUp() const;
		RMFLOAT3 GetRight() const;

		RMFLOAT4X4 GetLocalScaleMatrix() const;
		RMFLOAT4X4 GetLocalRotationMatrix() const;
		RMFLOAT4X4 GetLocalTranslateMatrix() const;

		RMFLOAT4X4 GetWorldScaleMatrix() const;
		RMFLOAT4X4 GetWorldRotationMatrix() const;
		RMFLOAT4X4 GetWorldTranslateMatrix() const;

		RMFLOAT4X4 GetWorldTM() const;

		// UI���� ����� 2D Transform ���� Get.
	public:
		RMFLOAT3X3 Get2DLocalScaleMatrix() const;
		RMFLOAT3X3 Get2DLocalRotationMatrix() const;
		RMFLOAT3X3 Get2DLocalTranslateMatrix() const;

		RMFLOAT3X3 Get2DWorldTM() const;

	public:
		void Translate(const RMFLOAT3& position);
		void Translate(float x, float y, float z);
		void Rotate(RMQuaternion quaternion);					// ���ʹϾ� ���� ȸ��
		void Rotate(float angleX, float angleY, float angleZ);	// ���Ϸ� �� ���� ȸ��, radian

		void LookAt(const RMFLOAT3& target, const RMFLOAT3& up);
		void LookAt(const RMFLOAT3& pos, const RMFLOAT3& target, const RMFLOAT3& up);

	private:
		RMFLOAT3 _position;
		RMQuaternion _rotation;		// ���ʹϾ�
		RMFLOAT3 _scale;

		/// ��������.
	public:
		void SetParent(Transform* parent);
		void SetParent(GameObject* parentObj);
		Transform* GetParent();
		Transform* GetChild(int index);

		void ReleaseParent();

	private:
		void ReleaseChild(Transform* child);
		
	public:
		std::vector<Transform*>& GetChildrenVec();

	private:
		void AddChild(Transform* child);

	private:
		Transform* _parent;
		std::vector<Transform*> _children;
	};
}
