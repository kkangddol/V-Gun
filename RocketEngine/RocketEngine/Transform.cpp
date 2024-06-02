#include "Transform.h"
#include "GameObject.h"

namespace RocketEngine
{
	Transform::Transform(GameObject* owner)
		: Component(owner),
		_position(0.0f, 0.0f, 0.0f),
		_rotation(1.0f, 0.0f, 0.0f, 0.0f),
		_scale(1.0f, 1.0f, 1.0f),
		_parent(),
		_children()
	{

	}

	RocketEngine::RMFLOAT3 Transform::GetPosition() const
	{
		RMFLOAT4 result = { _position.x, _position.y, _position.z ,1.0f };
		if (_parent)
		{
			result = RMFloat4MultiplyMatrix(result, _parent->GetWorldTM());
		}

		return { result.x, result.y, result.z };
	}

	RocketEngine::RMQuaternion Transform::GetRotation() const
	{
		RMQuaternion result = _rotation;
		if (_parent)
		{
			result = RMQuaternionMultiply(result, _parent->GetRotation());
		}

		return result;
	}

	RocketEngine::RMFLOAT3 Transform::GetEuler() const
	{
		RMQuaternion rot = GetRotation();

		// 쿼터니언의 요소 추출
		float w = rot.w;
		float x = rot.x;
		float y = rot.y;
		float z = rot.z;

		// ZYX 회전 순서로 오일러 각 계산
		float yaw = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
		float pitch = std::asin(2.0f * (w * y - z * x));
		float roll = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));

		// 라디안을 도(degree)로 변환하여 반환
		const float toDegree = 180.0f / 3.14159265358979323846f;
		return { roll * toDegree, pitch * toDegree,  yaw * toDegree };
	}

	RocketEngine::RMFLOAT3 Transform::GetScale() const
	{
		RMFLOAT3 result = _scale;
		if (_parent)
		{
			result.x *= _parent->GetScale().x;
			result.y *= _parent->GetScale().y;
			result.z *= _parent->GetScale().z;
		}

		return result;
	}

	void Transform::SetPosition(const RMFLOAT3& position)
	{
		SetPosition(position.x, position.y, position.z);
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		RMFLOAT4 result = { x,y,z ,1.0f };
		if (_parent)
		{
			result = RMFloat4MultiplyMatrix(result, _parent->GetWorldTM().Inverse());
		}

		_position.x = result.x;
		_position.y = result.y;
		_position.z = result.z;
	}

	void Transform::SetRotation(const RMQuaternion& quaternion)
	{
		SetRotation(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
	}

	void Transform::SetRotation(float w, float x, float y, float z)
	{
		RMQuaternion result = { w,x,y,z };
		if (_parent)
		{
			result = RMQuaternionMultiply(result, _parent->GetRotation().conjugate());
		}

		_rotation = result;
	}

	void Transform::SetRotationEuler(const RMFLOAT3& euler)
	{
		SetRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetRotationEuler(float angleX, float angleY, float angleZ)
	{
		float radianX = RMConvertToRadians(angleX);
		float radianY = RMConvertToRadians(angleY);
		float radianZ = RMConvertToRadians(angleZ);

		// 회전 각도를 반으로 나누어 준비합니다.
		float half_radianX = radianX * 0.5f;
		float half_radianY = radianY * 0.5f;
		float half_radianZ = radianZ * 0.5f;

		// 각 축의 쿼터니언 요소를 계산합니다.
		float sin_half_radianX = sin(half_radianX);
		float sin_half_radianY = sin(half_radianY);
		float sin_half_radianZ = sin(half_radianZ);
		float cos_half_radianX = cos(half_radianX);
		float cos_half_radianY = cos(half_radianY);
		float cos_half_radianZ = cos(half_radianZ);

		RMQuaternion rot;

		rot.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;
		rot.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		rot.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		rot.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;

		SetRotation(rot);
	}

	void Transform::SetScale(const RMFLOAT3& scale)
	{
		SetScale(scale.x, scale.y, scale.z);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		RMFLOAT4 result = { x,y,z ,1.0f };
		if (_parent)
		{
			RMFLOAT4X4 scaleInverseMatrix = _parent->GetWorldScaleMatrix();
			scaleInverseMatrix.m[0][0] = 1 / scaleInverseMatrix.m[0][0];
			scaleInverseMatrix.m[1][1] = 1 / scaleInverseMatrix.m[1][1];
			scaleInverseMatrix.m[2][2] = 1 / scaleInverseMatrix.m[2][2];
			scaleInverseMatrix.m[3][3] = 1 / scaleInverseMatrix.m[3][3];
			result = RMFloat4MultiplyMatrix(result, scaleInverseMatrix);
		}

		_scale.x = result.x;
		_scale.y = result.y;
		_scale.z = result.z;
	}

	RocketEngine::RMFLOAT3 Transform::GetLocalPosition() const
	{
		return _position;
	}

	RocketEngine::RMQuaternion Transform::GetLocalRotation() const
	{
		return _rotation;
	}

	RocketEngine::RMFLOAT3 Transform::GetLocalEuler() const
	{
		// 쿼터니언의 요소 추출
		float w = _rotation.w;
		float x = _rotation.x;
		float y = _rotation.y;
		float z = _rotation.z;

		// ZYX 회전 순서로 오일러 각 계산
		float yaw = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
		float pitch = std::asin(2.0f * (w * y - z * x));
		float roll = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));

		// 라디안을 도(degree)로 변환하여 반환
		const float toDegree = 180.0f / 3.14159265358979323846f;
		return { roll * toDegree, pitch * toDegree,  yaw * toDegree };
	}

	RocketEngine::RMFLOAT3 Transform::GetLocalScale() const
	{
		return _scale;
	}

	RocketEngine::RMFLOAT3& Transform::GetLocalPositionRef()
	{
		return _position;
	}

	RocketEngine::RMQuaternion& Transform::GetLocalRotationRef()
	{
		return _rotation;
	}

	RocketEngine::RMFLOAT3& Transform::GetLocalScaleRef()
	{
		return _scale;
	}	

	void Transform::SetLocalPosition(const RMFLOAT3& position)
	{
		_position = position;
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	void Transform::SetLocalRotation(const RMQuaternion& quaternion)
	{
		_rotation = quaternion;
	}

	void Transform::SetLocalRotation(float w, float x, float y, float z)
	{
		RMQuaternion temp = RMQuaternionNormalize({ w,x,y,z });

		_rotation.w = temp.w;
		_rotation.x = temp.x;
		_rotation.y = temp.y;
		_rotation.z = temp.z;
	}

	void Transform::SetLocalRotationEuler(const RMFLOAT3& euler)
	{
		SetLocalRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetLocalRotationEuler(float angleX, float angleY, float angleZ)
	{
		float radianX = RMConvertToRadians(angleX);
		float radianY = RMConvertToRadians(angleY);
		float radianZ = RMConvertToRadians(angleZ);

		// 회전 각도를 반으로 나누어 준비합니다.
		float half_radianX = radianX * 0.5f;
		float half_radianY = radianY * 0.5f;
		float half_radianZ = radianZ * 0.5f;

		// 각 축의 쿼터니언 요소를 계산합니다.
		float sin_half_radianX = sin(half_radianX);
		float sin_half_radianY = sin(half_radianY);
		float sin_half_radianZ = sin(half_radianZ);
		float cos_half_radianX = cos(half_radianX);
		float cos_half_radianY = cos(half_radianY);
		float cos_half_radianZ = cos(half_radianZ);

		_rotation.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;
		_rotation.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		_rotation.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		_rotation.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;
	}

	void Transform::SetLocalRotationEulerXZConvert(float angleX, float angleY, float angleZ)
	{
		SetLocalRotationEuler(angleZ, angleY + 90.0f, angleX);
	}

	void Transform::SetLocalScale(const RMFLOAT3& scale)
	{
		_scale = scale;
	}

	void Transform::SetLocalScale(float x, float y, float z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}

	RocketEngine::RMFLOAT3 Transform::GetForward() const
	{
		RMFLOAT4 temp = RMFloat4MultiplyMatrix(RMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f), GetWorldRotationMatrix());
		return RMFLOAT3(temp.x, temp.y, temp.z);
	}

	RocketEngine::RMFLOAT3 Transform::GetUp() const
	{
		RMFLOAT4 temp = RMFloat4MultiplyMatrix(RMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f), GetWorldRotationMatrix());
		return RMFLOAT3(temp.x, temp.y, temp.z);
	}

	RocketEngine::RMFLOAT3 Transform::GetRight() const
	{
		RMFLOAT4 temp = RMFloat4MultiplyMatrix(RMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f), GetWorldRotationMatrix());
		return RMFLOAT3(temp.x, temp.y, temp.z);
	}

	RocketEngine::RMFLOAT4X4 Transform::GetLocalScaleMatrix() const
	{
		RMFLOAT4X4 scaleMatrix =
		{
			_scale.x,	0,		0,		0,
			0,		_scale.y,	0,		0,
			0,		0,		_scale.z,	0,
			0,		0,		0,			1
		};

		return scaleMatrix;
	}

	RocketEngine::RMFLOAT4X4 Transform::GetLocalRotationMatrix() const
	{
		RMFLOAT4X4 rotationMatrix =
		{
			1.0f - 2.0f * (_rotation.y * _rotation.y + _rotation.z * _rotation.z),
			2.0f * (_rotation.x * _rotation.y + _rotation.z * _rotation.w),
			2.0f * (_rotation.x * _rotation.z - _rotation.y * _rotation.w),
			0,

			2.0f * (_rotation.x * _rotation.y - _rotation.z * _rotation.w),
			1.0f - 2.0f * (_rotation.x * _rotation.x + _rotation.z * _rotation.z),
			2.0f * (_rotation.y * _rotation.z + _rotation.x * _rotation.w),
			0,

			2.0f * (_rotation.x * _rotation.z + _rotation.y * _rotation.w),
			2.0f * (_rotation.y * _rotation.z - _rotation.x * _rotation.w),
			1.0f - 2.0f * (_rotation.x * _rotation.x + _rotation.y * _rotation.y),
			0,

			0,
			0,
			0,
			1
		};

		return rotationMatrix;
	}

	RocketEngine::RMFLOAT4X4 Transform::GetLocalTranslateMatrix() const
	{
		RMFLOAT4X4 translateMatrix =
		{
			1,				0,				0,				0,
			0,				1,				0,				0,
			0,				0,				1,				0,
			_position.x,	_position.y,	_position.z,	1
		};

		return translateMatrix;
	}

	RocketEngine::RMFLOAT4X4 Transform::GetWorldScaleMatrix() const
	{
		RMFLOAT4X4 result = GetLocalScaleMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldScaleMatrix();
		}

		return result;
	}

	RocketEngine::RMFLOAT4X4 Transform::GetWorldRotationMatrix() const
	{
		RMFLOAT4X4 result = GetLocalRotationMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldRotationMatrix();
		}

		return result;
	}

	RocketEngine::RMFLOAT4X4 Transform::GetWorldTranslateMatrix() const
	{
		RMFLOAT4X4 result = GetLocalTranslateMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldTranslateMatrix();
		}

		return result;
	}

	RocketEngine::RMFLOAT4X4 Transform::GetWorldTM() const
	{
		RMFLOAT4X4 result = GetLocalScaleMatrix() * GetLocalRotationMatrix() * GetLocalTranslateMatrix();
		
		if (_parent)
		{
			result *= _parent->GetWorldTM();
		}

		return result;
	}

	RocketEngine::RMFLOAT3X3 Transform::Get2DLocalScaleMatrix() const
	{
		RMFLOAT3X3 scaleMatrix =
		{
			_scale.x,	0,		0,
			0,		_scale.y,	0,
			0,			0,		1
		};

		return scaleMatrix;
	}

	RocketEngine::RMFLOAT3X3 Transform::Get2DLocalRotationMatrix() const
	{
		RMFLOAT3X3 rotationMatrix =
		{
			1.0f - 2.0f * (_rotation.y * _rotation.y + _rotation.z * _rotation.z),
			2.0f * (_rotation.x * _rotation.y + _rotation.z * _rotation.w),
			0,

			2.0f * (_rotation.x * _rotation.y - _rotation.z * _rotation.w),
			1.0f - 2.0f * (_rotation.x * _rotation.x + _rotation.z * _rotation.z),
			0,

			0,
			0,
			1
		};

		return rotationMatrix;
	}

	RocketEngine::RMFLOAT3X3 Transform::Get2DLocalTranslateMatrix() const
	{
		RMFLOAT3X3 translateMatrix =
		{
			1,				0,				0,
			0,				1,				0,
			_position.x,	_position.y,	1
		};

		return translateMatrix;
	}

	RocketEngine::RMFLOAT3X3 Transform::Get2DWorldTM() const
	{
		RMFLOAT3X3 result = Get2DLocalScaleMatrix() * Get2DLocalRotationMatrix() * Get2DLocalTranslateMatrix();

		if (_parent)
		{
			result *= _parent->Get2DWorldTM();
		}

		return result;
	}

	void Transform::Translate(const RMFLOAT3& position)
	{
		_position.x += position.x;
		_position.y += position.y;
		_position.z += position.z;
	}

	void Transform::Translate(float x, float y, float z)
	{
		_position.x += x;
		_position.y += y;
		_position.z += z;
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ)
	{
		float radianX = RMConvertToRadians(angleX);
		float radianY = RMConvertToRadians(angleY);
		float radianZ = RMConvertToRadians(angleZ);

		// 회전 각도를 반으로 나누어 준비합니다.
		float half_radianX = radianX * 0.5f;
		float half_radianY = radianY * 0.5f;
		float half_radianZ = radianZ * 0.5f;

		// 각 축의 쿼터니언 요소를 계산합니다.
		float sin_half_radianX = sin(half_radianX);
		float sin_half_radianY = sin(half_radianY);
		float sin_half_radianZ = sin(half_radianZ);
		float cos_half_radianX = cos(half_radianX);
		float cos_half_radianY = cos(half_radianY);
		float cos_half_radianZ = cos(half_radianZ);

		// 쿼터니언 요소를 계산합니다.
		RMFLOAT4 rotQuat;
		rotQuat.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		rotQuat.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		rotQuat.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;
		rotQuat.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;

		// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언을 계산합니다.
		RMQuaternion result;
		result.x = _rotation.w * rotQuat.x + _rotation.x * rotQuat.w + _rotation.y * rotQuat.z - _rotation.z * rotQuat.y;
		result.y = _rotation.w * rotQuat.y - _rotation.x * rotQuat.z + _rotation.y * rotQuat.w + _rotation.z * rotQuat.x;
		result.z = _rotation.w * rotQuat.z + _rotation.x * rotQuat.y - _rotation.y * rotQuat.x + _rotation.z * rotQuat.w;
		result.w = _rotation.w * rotQuat.w - _rotation.x * rotQuat.x - _rotation.y * rotQuat.y - _rotation.z * rotQuat.z;

		// 결과를 저장합니다.
		_rotation = result;
	}

	void Transform::Rotate(RMQuaternion quaternion)
	{
		_rotation = RMQuaternionMultiply(_rotation, quaternion);
	}

	/// <summary>
	/// target과 up벡터를 주면 그것을 기준으로 카메라가 바라본다.
	/// 
	/// 23.04.20 강석원 인재원
	/// </summary>
	/// <param name="target">바라볼 타겟</param>
	/// <param name="up"> 카메라가 right vector를 구할때 사용할 up 벡터</param>
	void Transform::LookAt(const RMFLOAT3& target, const RMFLOAT3& up)
	{
// 		RMFLOAT3 look = RMFloat3Normalize(target - gameObject->transform.GetPosition());
// 		RMFLOAT3 right = RMFloat3Normalize(RMFloat3Cross(up, look));
// 		RMFLOAT3 up = RMFloat3Normalize(RMFloat3Cross(look, right));
// 
// 		_look
// 
// 			XMStoreFloat3(&look_, lookVector);
// 		XMStoreFloat3(&right_, rightVector);
// 		XMStoreFloat3(&up_, upVector);
	}

	void Transform::LookAt(const RMFLOAT3& pos, const RMFLOAT3& target, const RMFLOAT3& up)
	{

	}

	void Transform::SetParent(Transform* parent)
	{
		auto name = gameObject->objName;
		if (name != "playerLeg" && name != "playerBody" && name != "playerHead")
		{
			RMFLOAT4 tempPos = RMFloat4MultiplyMatrix({ GetPosition(), 1.0f }, parent->GetWorldTM().Inverse());
			_position.x = tempPos.x;
			_position.y = tempPos.y;
			_position.z = tempPos.z;

			_rotation = RMQuaternionMultiply(GetRotation(), parent->GetRotation().conjugate());
		}

		RMFLOAT4 tempScale = RMFloat4MultiplyMatrix({ GetScale(), 1.0f }, parent->GetWorldScaleMatrix().Inverse());
		_scale.x = tempScale.x;
		_scale.y = tempScale.y;
		_scale.z = tempScale.z;

		_parent = parent;
		_parent->AddChild(this);
	}

	void Transform::SetParent(GameObject* parentObj)
	{
		SetParent(&(parentObj->transform));
	}

	Transform* Transform::GetParent()
	{
		return _parent;
	}

	Transform* Transform::GetChild(int index)
	{
		if (_children.size() <= index)
		{
			return nullptr;
		}

		return _children[index];
	}

	void Transform::ReleaseParent()
	{
		if (!_parent)
		{
			return;
		}

		RMFLOAT4 tempPos = RMFloat4MultiplyMatrix({ _position.x,_position.y,_position.z,1.0f }, _parent->GetWorldTM());
		SetLocalPosition(tempPos.x, tempPos.y, tempPos.z);

		SetLocalRotation(RMQuaternionMultiply(_rotation, _parent->GetRotation()));
		RMFLOAT4 tempScale = RMFloat4MultiplyMatrix({ _scale,1.0f }, _parent->GetWorldScaleMatrix());
		SetLocalScale(tempScale.x, tempScale.y, tempScale.z);

		_parent->ReleaseChild(this);
		_parent = nullptr;
	}

	void Transform::ReleaseChild(Transform* child)
	{
		std::erase_if(_children, [child](Transform* tr) {return tr == child; });
	}

	std::vector<Transform*>& Transform::GetChildrenVec()
	{
		return _children;
	}

	void Transform::AddChild(Transform* child)
	{
		_children.push_back(child);
	}

}
