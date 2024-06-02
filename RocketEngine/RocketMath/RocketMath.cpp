#include "RocketMath.h"
#include <cmath>

inline float RMFloat3Length(const RocketEngine::RMFLOAT3& f) noexcept
{
	return sqrtf(f.x * f.x + f.y * f.y + f.z * f.z);
}

inline RocketEngine::RMFLOAT3 RMFloat3Normalize(const RocketEngine::RMFLOAT3& f) noexcept
{
	float len = RMFloat3Length(f);
	return RocketEngine::RMFLOAT3(f.x / len, f.y / len, f.z / len);
}

inline RocketEngine::RMFLOAT4 RMFloat4Normalize(const RocketEngine::RMFLOAT4& f) noexcept
{
	float length = std::sqrt(f.x * f.x + f.y * f.y + f.z * f.z + f.w * f.w);
	return { f.x / length, f.y / length, f.z / length, f.w / length };
}

RocketEngine::RMQuaternion RMQuaternionNormalize(const RocketEngine::RMQuaternion& f) noexcept
{
	float length = std::sqrt(f.w * f.w + f.x * f.x + f.y * f.y + f.z * f.z);
	return { f.w / length, f.x / length, f.y / length, f.z / length };
}

constexpr RocketEngine::RMFLOAT3 RMFloat3Cross(const RocketEngine::RMFLOAT3& lhs, const RocketEngine::RMFLOAT3& rhs)
{
	return RocketEngine::RMFLOAT3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}


RocketEngine::RMFLOAT3 RMFloat3MultiplyMatrix(const RocketEngine::RMFLOAT3& lhs, const RocketEngine::RMFLOAT4X4& rhs)
{
	RocketEngine::RMFLOAT4 result = RMFloat4MultiplyMatrix({ lhs,1.0f }, rhs);
	return { result.x,result.y,result.z };
}

constexpr float RMFloat3Dot(const RocketEngine::RMFLOAT3& lhs, const RocketEngine::RMFLOAT3& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

RocketEngine::RMFLOAT4 RMFloat4MultiplyMatrix(const RocketEngine::RMFLOAT4& lhs, const RocketEngine::RMFLOAT4X4& rhs)
{
	RocketEngine::RMFLOAT4 result;

	result.x =
		lhs.x * rhs.m[0][0] +
		lhs.y * rhs.m[1][0] +
		lhs.z * rhs.m[2][0] +
		lhs.w * rhs.m[3][0];

	result.y =
		lhs.x * rhs.m[0][1] +
		lhs.y * rhs.m[1][1] +
		lhs.z * rhs.m[2][1] +
		lhs.w * rhs.m[3][1];

	result.z =
		lhs.x * rhs.m[0][2] +
		lhs.y * rhs.m[1][2] +
		lhs.z * rhs.m[2][2] +
		lhs.w * rhs.m[3][2];

	result.w =
		lhs.x * rhs.m[0][3] +
		lhs.y * rhs.m[1][3] +
		lhs.z * rhs.m[2][3] +
		lhs.w * rhs.m[3][3];

	return result;
}

RocketEngine::RMQuaternion RMRotateQuaternionY(const RocketEngine::RMQuaternion& quaternion, float radian)
{
	float halfAngleRad = radian * 0.5f;
	float sinHalfAngle = std::sin(halfAngleRad);
	float cosHalfAngle = std::cos(halfAngleRad);

	RocketEngine::RMQuaternion rotation;
	rotation.w = cosHalfAngle;
	rotation.x = 0.0f;
	rotation.y = sinHalfAngle;
	rotation.z = 0.0f;

	// 회전 쿼터니언을 정규화하여 사용
	rotation = RMQuaternionNormalize(rotation);

	// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언 계산
	RocketEngine::RMQuaternion result;
//	result = RMQuaternionMultiply(quaternion, rotation);
	result.w = quaternion.w * rotation.w - quaternion.x * rotation.x - quaternion.y * rotation.y - quaternion.z * rotation.z;
	result.x = quaternion.w * rotation.x + quaternion.x * rotation.w + quaternion.y * rotation.z - quaternion.z * rotation.y;
	result.y = quaternion.w * rotation.y - quaternion.x * rotation.z + quaternion.y * rotation.w + quaternion.z * rotation.x;
	result.z = quaternion.w * rotation.z + quaternion.x * rotation.y - quaternion.y * rotation.x + quaternion.z * rotation.w;

	return result;
}

RocketEngine::RMQuaternion RMRotateQuaternion(const RocketEngine::RMQuaternion& quaternion, const RocketEngine::RMFLOAT3& axis, float radian)
{
	// 축 벡터를 정규화합니다.
	RocketEngine::RMFLOAT3 normalizedAxis = RMFloat3Normalize(axis);

	// 회전 각도의 반을 구합니다.
	float halfAngleRad = radian * 0.5f;

	// 회전 각도의 부호를 쿼터니언에 반영합니다.
	float cosHalfAngle = std::cos(halfAngleRad);
	float sinHalfAngle = std::sin(halfAngleRad);

	float w = cosHalfAngle;
	float x = normalizedAxis.x * sinHalfAngle;
	float y = normalizedAxis.y * sinHalfAngle;
	float z = normalizedAxis.z * sinHalfAngle;

	// 회전 쿼터니언을 정규화하여 사용합니다.
	RocketEngine::RMQuaternion rotation = { w, x, y, z };
	rotation = RMQuaternionNormalize(rotation);

	// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언을 계산합니다.
	RocketEngine::RMQuaternion result;
//	result = RMQuaternionMultiply(quaternion, rotation);
	result.w = rotation.w * quaternion.w - rotation.x * quaternion.x - rotation.y * quaternion.y - rotation.z * quaternion.z;
	result.x = rotation.w * quaternion.x + rotation.x * quaternion.w + rotation.y * quaternion.z - rotation.z * quaternion.y;
	result.y = rotation.w * quaternion.y - rotation.x * quaternion.z + rotation.y * quaternion.w + rotation.z * quaternion.x;
	result.z = rotation.w * quaternion.z + rotation.x * quaternion.y - rotation.y * quaternion.x + rotation.z * quaternion.w;

	return result;
}

RocketEngine::RMQuaternion RMQuaternionMultiply(const RocketEngine::RMQuaternion& lhs, const RocketEngine::RMQuaternion& rhs)
{
	RocketEngine::RMQuaternion lhsNorm = RMQuaternionNormalize(lhs);
	RocketEngine::RMQuaternion rhsNorm = RMQuaternionNormalize(rhs);

	// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언을 계산합니다.
	RocketEngine::RMQuaternion result;

	result.w = rhsNorm.w * lhsNorm.w - rhsNorm.x * lhsNorm.x - rhsNorm.y * lhsNorm.y - rhsNorm.z * lhsNorm.z;
	result.x = rhsNorm.w * lhsNorm.x + rhsNorm.x * lhsNorm.w + rhsNorm.y * lhsNorm.z - rhsNorm.z * lhsNorm.y;
	result.y = rhsNorm.w * lhsNorm.y - rhsNorm.x * lhsNorm.z + rhsNorm.y * lhsNorm.w + rhsNorm.z * lhsNorm.x;
	result.z = rhsNorm.w * lhsNorm.z + rhsNorm.x * lhsNorm.y - rhsNorm.y * lhsNorm.x + rhsNorm.z * lhsNorm.w;

// 	result.w = lhsNorm.w * rhsNorm.w - lhsNorm.x * rhsNorm.x - lhsNorm.y * rhsNorm.y - lhsNorm.z * rhsNorm.z;
// 	result.x = lhsNorm.w * rhsNorm.x + lhsNorm.x * rhsNorm.w + lhsNorm.y * rhsNorm.z - lhsNorm.z * rhsNorm.y;
// 	result.y = lhsNorm.w * rhsNorm.y - lhsNorm.x * rhsNorm.z + lhsNorm.y * rhsNorm.w + lhsNorm.z * rhsNorm.x;
// 	result.z = lhsNorm.w * rhsNorm.z + lhsNorm.x * rhsNorm.y - lhsNorm.y * rhsNorm.x + lhsNorm.z * rhsNorm.w;

	result = RMQuaternionNormalize(result);

	return result;
}

RocketEngine::RMFLOAT4X4 RMMatrixRotationX(float angle)
{
	float    fSinAngle = std::sin(angle);
	float    fCosAngle = std::cos(angle);
	//XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

	RocketEngine::RMFLOAT4X4 M;
	M.m[0][0] = 1.0f;
	M.m[0][1] = 0.0f;
	M.m[0][2] = 0.0f;
	M.m[0][3] = 0.0f;

	M.m[1][0] = 0.0f;
	M.m[1][1] = fCosAngle;
	M.m[1][2] = -fSinAngle;
	M.m[1][3] = 0.0f;

	M.m[2][0] = 0.0f;
	M.m[2][1] = fSinAngle;
	M.m[2][2] = fCosAngle;
	M.m[2][3] = 0.0f;

	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;

	return M;
}

RocketEngine::RMFLOAT4X4 RMMatrixRotationY(float angle)
{
	float    fSinAngle = std::sin(angle);
	float    fCosAngle = std::cos(angle);
	//XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

	RocketEngine::RMFLOAT4X4 M;
	M.m[0][0] = fCosAngle;
	M.m[0][1] = 0.0f;
	M.m[0][2] = -fSinAngle;
	M.m[0][3] = 0.0f;

	M.m[1][0] = 0.0f;
	M.m[1][1] = 1.0f;
	M.m[1][2] = 0.0f;
	M.m[1][3] = 0.0f;

	M.m[2][0] = fSinAngle;
	M.m[2][1] = 0.0f;
	M.m[2][2] = fCosAngle;
	M.m[2][3] = 0.0f;

	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;

	return M;
}

RocketEngine::RMFLOAT3X3 RMInverseMatrix(const RocketEngine::RMFLOAT3X3& mat)
{
	// 3x3 행렬의 행렬식 계산
	float det = mat.m[0][0] * (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) -
		mat.m[0][1] * (mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]) +
		mat.m[0][2] * (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]);

	// 행렬식이 0인 경우 역행렬이 존재하지 않음
	if (det == 0)
	{
		return RocketEngine::RMFLOAT3X3::Identity();
	}

	RocketEngine::RMFLOAT3X3 inv;

	// 역행렬 계산
	float invDet = 1.0f / det;
	inv.m[0][0] = (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) * invDet;
	inv.m[0][1] = (mat.m[0][2] * mat.m[2][1] - mat.m[0][1] * mat.m[2][2]) * invDet;
	inv.m[0][2] = (mat.m[0][1] * mat.m[1][2] - mat.m[0][2] * mat.m[1][1]) * invDet;
	inv.m[1][0] = (mat.m[1][2] * mat.m[2][0] - mat.m[1][0] * mat.m[2][2]) * invDet;
	inv.m[1][1] = (mat.m[0][0] * mat.m[2][2] - mat.m[0][2] * mat.m[2][0]) * invDet;
	inv.m[1][2] = (mat.m[0][2] * mat.m[1][0] - mat.m[0][0] * mat.m[1][2]) * invDet;
	inv.m[2][0] = (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]) * invDet;
	inv.m[2][1] = (mat.m[0][1] * mat.m[2][0] - mat.m[0][0] * mat.m[2][1]) * invDet;
	inv.m[2][2] = (mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0]) * invDet;

	return inv;
}

RocketEngine::RMFLOAT4 RMQuaternionToFloat4(const RocketEngine::RMQuaternion& quaternion)
{
	return { quaternion.x,quaternion.y,quaternion.z,quaternion.w };
}

RocketEngine::RMFLOAT4 RMFloat4ToQuaternion(const RocketEngine::RMFLOAT4& f4)
{
	return { f4.w,f4.x,f4.y,f4.z };
}

RocketEngine::RMQuaternion RMMatrixToQuaternion(const RocketEngine::RMFLOAT4X4& matrix)
{
	RocketEngine::RMQuaternion quaternion;

	float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];
	if (trace > 0) {
		float s = 0.5f / sqrtf(trace + 1.0f);
		quaternion.w = 0.25f / s;
		quaternion.x = (matrix.m[1][2] - matrix.m[2][1]) * s;
		quaternion.y = (matrix.m[2][0] - matrix.m[0][2]) * s;
		quaternion.z = (matrix.m[0][1] - matrix.m[1][0]) * s;
	}
	else {
		if (matrix.m[0][0] > matrix.m[1][1] && matrix.m[0][0] > matrix.m[2][2]) {
			float s = 2.0f * sqrtf(1.0f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]);
			quaternion.w = (matrix.m[1][2] - matrix.m[2][1]) / s;
			quaternion.x = 0.25f * s;
			quaternion.y = (matrix.m[1][0] + matrix.m[0][1]) / s;
			quaternion.z = (matrix.m[2][0] + matrix.m[0][2]) / s;
		}
		else if (matrix.m[1][1] > matrix.m[2][2]) {
			float s = 2.0f * sqrtf(1.0f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2]);
			quaternion.w = (matrix.m[2][0] - matrix.m[0][2]) / s;
			quaternion.x = (matrix.m[1][0] + matrix.m[0][1]) / s;
			quaternion.y = 0.25f * s;
			quaternion.z = (matrix.m[2][1] + matrix.m[1][2]) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]);
			quaternion.w = (matrix.m[0][1] - matrix.m[1][0]) / s;
			quaternion.x = (matrix.m[2][0] + matrix.m[0][2]) / s;
			quaternion.y = (matrix.m[2][1] + matrix.m[1][2]) / s;
			quaternion.z = 0.25f * s;
		}
	}

	return quaternion;
}

RocketEngine::RMFLOAT4X4 RMScaleMatrix(const RocketEngine::RMFLOAT3 scale)
{
	RocketEngine::RMFLOAT4X4 scaleMatrix =
	{
		scale.x,	0,		0,		0,
		0,		scale.y,	0,		0,
		0,		0,		scale.z,	0,
		0,		0,		0,			1
	};

	return scaleMatrix;
}

RocketEngine::RMFLOAT4X4 RMRotationMatrix(const RocketEngine::RMQuaternion rotation)
{
	RocketEngine::RMFLOAT4X4 rotationMatrix =
	{
		1.0f - 2.0f * (rotation.y * rotation.y + rotation.z * rotation.z),
		2.0f * (rotation.x * rotation.y + rotation.z * rotation.w),
		2.0f * (rotation.x * rotation.z - rotation.y * rotation.w),
		0,

		2.0f * (rotation.x * rotation.y - rotation.z * rotation.w),
		1.0f - 2.0f * (rotation.x * rotation.x + rotation.z * rotation.z),
		2.0f * (rotation.y * rotation.z + rotation.x * rotation.w),
		0,

		2.0f * (rotation.x * rotation.z + rotation.y * rotation.w),
		2.0f * (rotation.y * rotation.z - rotation.x * rotation.w),
		1.0f - 2.0f * (rotation.x * rotation.x + rotation.y * rotation.y),
		0,

		0,
		0,
		0,
		1
	};

	return rotationMatrix;
}

RocketEngine::RMFLOAT4X4 RMTranslateMatrix(const RocketEngine::RMFLOAT3 position)
{
	RocketEngine::RMFLOAT4X4 translateMatrix =
	{
		1,				0,				0,				0,
		0,				1,				0,				0,
		0,				0,				1,				0,
		position.x,		position.y,		position.z,	1
	};

	return translateMatrix;
}

RocketEngine::RMFLOAT4& RocketEngine::RMFLOAT4::operator*=(const RMFLOAT4X4& rhs)
{
	RMFLOAT4 temp;

	temp.x = x * rhs.m[0][0] + y * rhs.m[1][0] + z * rhs.m[2][0] + w * rhs.m[3][0];
	temp.y = x * rhs.m[0][1] + y * rhs.m[1][1] + z * rhs.m[2][1] + w * rhs.m[3][1];
	temp.z = x * rhs.m[0][2] + y * rhs.m[1][2] + z * rhs.m[2][2] + w * rhs.m[3][2];
	temp.w = x * rhs.m[0][3] + y * rhs.m[1][3] + z * rhs.m[2][3] + w * rhs.m[3][3];

	*this = temp;
	return *this;
}

RocketEngine::RMFLOAT4 RocketEngine::RMFLOAT4::operator-(const RMFLOAT4& rhs)
{
	RMFLOAT4 result;
	result.x = x - rhs.x;
	result.y = y - rhs.y;
	result.z = z - rhs.z;
	result.w = w - rhs.w;

	return result;
}

RocketEngine::RMFLOAT4 RocketEngine::RMFLOAT4::operator*(const RMFLOAT4X4& rhs)
{
	RMFLOAT4 temp;

	temp.x = x * rhs.m[0][0] + y * rhs.m[1][0] + z * rhs.m[2][0] + w * rhs.m[3][0];
	temp.y = x * rhs.m[0][1] + y * rhs.m[1][1] + z * rhs.m[2][1] + w * rhs.m[3][1];
	temp.z = x * rhs.m[0][2] + y * rhs.m[1][2] + z * rhs.m[2][2] + w * rhs.m[3][2];
	temp.w = x * rhs.m[0][3] + y * rhs.m[1][3] + z * rhs.m[2][3] + w * rhs.m[3][3];

	return temp;
}

RocketEngine::RMFLOAT4X4 RocketEngine::RMFLOAT4X4::Inverse() const
{
	RocketEngine::RMFLOAT4X4 inv;

	float det =
		_11 * (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42)) -
		_12 * (_21 * (_33 * _44 - _34 * _43) - _23 * (_31 * _44 - _34 * _41) + _24 * (_31 * _43 - _33 * _41)) +
		_13 * (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41)) -
		_14 * (_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41));

	if (std::abs(det) < 1e-8)
	{
		// 행렬식이 0에 가까우면 역행렬이 존재하지 않습니다.
		return RocketEngine::RMFLOAT4X4::Identity();
	}

	float invDet = 1.0f / det;

	inv._11 = (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42)) * invDet;
	inv._12 = -(_12 * (_33 * _44 - _34 * _43) - _13 * (_32 * _44 - _34 * _42) + _14 * (_32 * _43 - _33 * _42)) * invDet;
	inv._13 = (_12 * (_23 * _44 - _24 * _43) - _13 * (_22 * _44 - _24 * _42) + _14 * (_22 * _43 - _23 * _42)) * invDet;
	inv._14 = -(_12 * (_23 * _34 - _24 * _33) - _13 * (_22 * _34 - _24 * _32) + _14 * (_22 * _33 - _23 * _32)) * invDet;
	inv._21 = -(_21 * (_33 * _44 - _34 * _43) - _23 * (_31 * _44 - _34 * _41) + _24 * (_31 * _43 - _33 * _41)) * invDet;
	inv._22 = (_11 * (_33 * _44 - _34 * _43) - _13 * (_31 * _44 - _34 * _41) + _14 * (_31 * _43 - _33 * _41)) * invDet;
	inv._23 = -(_11 * (_23 * _44 - _24 * _43) - _13 * (_21 * _44 - _24 * _41) + _14 * (_21 * _43 - _23 * _41)) * invDet;
	inv._24 = (_11 * (_23 * _34 - _24 * _33) - _13 * (_21 * _34 - _24 * _31) + _14 * (_21 * _33 - _23 * _31)) * invDet;
	inv._31 = (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41)) * invDet;
	inv._32 = -(_11 * (_32 * _44 - _34 * _42) - _12 * (_31 * _44 - _34 * _41) + _14 * (_31 * _42 - _32 * _41)) * invDet;
	inv._33 = (_11 * (_22 * _44 - _24 * _42) - _12 * (_21 * _44 - _24 * _41) + _14 * (_21 * _42 - _22 * _41)) * invDet;
	inv._34 = -(_11 * (_22 * _34 - _24 * _32) - _12 * (_21 * _34 - _24 * _31) + _14 * (_21 * _32 - _22 * _31)) * invDet;
	inv._41 = -(_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41)) * invDet;
	inv._42 = (_11 * (_32 * _43 - _33 * _42) - _12 * (_31 * _43 - _33 * _41) + _13 * (_31 * _42 - _32 * _41)) * invDet;
	inv._43 = -(_11 * (_22 * _43 - _23 * _42) - _12 * (_21 * _43 - _23 * _41) + _13 * (_21 * _42 - _22 * _41)) * invDet;
	inv._44 = (_11 * (_22 * _33 - _23 * _32) - _12 * (_21 * _33 - _23 * _31) + _13 * (_21 * _32 - _22 * _31)) * invDet;

	return inv;
}

RocketEngine::RMQuaternion RocketEngine::RMQuaternion::conjugate() const
{
	return RMQuaternion(w, -x, -y, -z);
}
