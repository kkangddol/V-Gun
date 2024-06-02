#pragma once
#include <functional>

namespace RocketEngine
{
	struct RMFLOAT3;
	struct RMFLOAT4X4;
}

/// <summary>
/// DX와 호환가능한 Math 라이브러리.
/// 
/// 23.06.27 강석원 인재원.
/// </summary>
namespace RocketEngine
{
	struct RMFLOAT2
	{
		float x;
		float y;

		RMFLOAT2() = default;

		RMFLOAT2(const RMFLOAT2&) = default;
		RMFLOAT2& operator=(const RMFLOAT2&) = default;

		RMFLOAT2(RMFLOAT2&&) = default;
		RMFLOAT2& operator=(RMFLOAT2&&) = default;

		constexpr RMFLOAT2(float _x, float _y) noexcept
			: x(_x), y(_y)
		{

		}

		constexpr RMFLOAT2 operator-(const RMFLOAT2& rhs) const noexcept
		{
			return { x - rhs.x, y - rhs.y };
		}

		constexpr RMFLOAT2 operator*(float rhs) const noexcept
		{
			return { x * rhs,y * rhs };
		}

		constexpr RMFLOAT2 operator+(const RMFLOAT2& rhs) const noexcept
		{
			return { x + rhs.x, y + rhs.y };
		}
	};

	struct RMFLOAT3
	{
		float x;
		float y;
		float z;

		RMFLOAT3() = default;

		RMFLOAT3(const RMFLOAT3&) = default;
		RMFLOAT3& operator=(const RMFLOAT3&) = default;

		RMFLOAT3(RMFLOAT3&&) = default;
		RMFLOAT3& operator=(RMFLOAT3&&) = default;

		constexpr RMFLOAT3(float _x, float _y, float _z) noexcept
			: x(_x), y(_y), z(_z)
		{

		}

		RMFLOAT3 operator-(const RMFLOAT3& rhs) const
		{
			return RMFLOAT3(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		RMFLOAT3 operator*(float rhs) const
		{
			return RMFLOAT3(x * rhs, y * rhs, z * rhs);
		}

		float operator*(RMFLOAT3 rhs) const
		{
			return (x * rhs.x + y * rhs.y + z * rhs.z);
		}

		RMFLOAT3& operator+=(const RMFLOAT3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		RMFLOAT3 operator+(const RMFLOAT3& rhs)
		{
			return
			{
				x + rhs.x,
				y + rhs.y,
				z + rhs.z
			};
		}

		bool operator<(const RMFLOAT3& rhs) const
		{
			return
			{
				x < rhs.x
				&& y < rhs.y
				&& z < rhs.z
			};
		}

		bool operator>(const RMFLOAT3& rhs) const
		{
			return
			{
				x > rhs.x
				&& y > rhs.y
				&& z > rhs.z
			};
		}

		bool operator>=(const RMFLOAT3& rhs) const
		{
			return
			{
				x >= rhs.x
				&& y >= rhs.y
				&& z >= rhs.z
			};
		}

		bool operator<=(const RMFLOAT3& rhs) const
		{
			return
			{
				x <= rhs.x
				&& y <= rhs.y
				&& z <= rhs.z
			};
		}
	};

	struct RMFLOAT4
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};
		};

		RMFLOAT4() = default;
		RMFLOAT4(const RMFLOAT4&) = default;
		RMFLOAT4& operator=(const RMFLOAT4&) = default;

		RMFLOAT4(RMFLOAT4&&) = default;
		RMFLOAT4& operator=(RMFLOAT4&&) = default;

		constexpr RMFLOAT4(float _x, float _y, float _z, float _w) noexcept
			: x(_x), y(_y), z(_z), w(_w)
		{
			r = x;
			g = y;
			b = z;
			a = w;
		}

		constexpr RMFLOAT4(const RMFLOAT3& rhs, float _w) noexcept
			: x(rhs.x),y(rhs.y),z(rhs.z),w(_w)
		{
			r = x;
			g = y;
			b = z;
			a = w;
		}

		RMFLOAT4& operator*=(const RMFLOAT4X4& rhs);
		RMFLOAT4 operator*(const RMFLOAT4X4& rhs);
		RMFLOAT4 operator-(const RMFLOAT4& rhs);

		bool operator<(const RMFLOAT4& rhs) const
		{
			return
			{
				x < rhs.x
				&& y < rhs.y
				&& z < rhs.z
			};
		}

		bool operator>(const RMFLOAT4& rhs) const
		{
			return
			{
				x > rhs.x
				&& y > rhs.y
				&& z > rhs.z
			};
		}

		bool operator>=(const RMFLOAT4& rhs) const
		{
			return
			{
				x >= rhs.x
				&& y >= rhs.y
				&& z >= rhs.z
			};
		}

		bool operator<=(const RMFLOAT4& rhs) const
		{
			return
			{
				x <= rhs.x
				&& y <= rhs.y
				&& z <= rhs.z
			};
		}
	};

	struct RMQuaternion
	{
		float w;
		float x;
		float y;
		float z;

		RMQuaternion() = default;

		RMQuaternion(const RMQuaternion&) = default;
		RMQuaternion& operator=(const RMQuaternion&) = default;

		RMQuaternion(RMQuaternion&&) = default;
		RMQuaternion& operator=(RMQuaternion&&) = default;

		constexpr RMQuaternion(float _w, float _x, float _y, float _z) noexcept
			: w(_w), x(_x), y(_y), z(_z)
		{

		}
		
		RMQuaternion conjugate() const;
	};

	struct RMFLOAT4X4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};

		constexpr RMFLOAT4X4() noexcept
			: _11(0), _12(0), _13(0), _14(0),
			_21(0), _22(0), _23(0), _24(0),
			_31(0), _32(0), _33(0), _34(0),
			_41(0), _42(0), _43(0), _44(0) 
		{

		}

		RMFLOAT4X4(const RMFLOAT4X4&) = default;
		RMFLOAT4X4& operator=(const RMFLOAT4X4&) = default;

		RMFLOAT4X4(RMFLOAT4X4&&) = default;
		RMFLOAT4X4& operator=(RMFLOAT4X4&&) = default;

		// 역행렬
		RMFLOAT4X4 Inverse() const;

		constexpr RMFLOAT4X4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) noexcept
			: _11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23),
			_41(m30), _42(m31), _43(m32), _44(m33)
		{

		}

		RMFLOAT4X4 operator*(const RMFLOAT4X4& rhs)
		{
			RMFLOAT4X4 result;

			for (int row = 0; row < 4; row++)
			{
				for (int col = 0; col < 4; col++)
				{
					result.m[row][col] =
						m[row][0] * rhs.m[0][col] +
						m[row][1] * rhs.m[1][col] +
						m[row][2] * rhs.m[2][col] +
						m[row][3] * rhs.m[3][col];
				}
			}

			return result;
		}

		RMFLOAT4X4& operator*=(const RMFLOAT4X4& rhs)
		{
			RMFLOAT4X4 temp;

			for (int row = 0; row < 4; row++)
			{
				for (int col = 0; col < 4; col++)
				{
					temp.m[row][col] =
						m[row][0] * rhs.m[0][col] +
						m[row][1] * rhs.m[1][col] +
						m[row][2] * rhs.m[2][col] +
						m[row][3] * rhs.m[3][col];
				}
			}

			*this = temp;
			return *this;
		}

		static RMFLOAT4X4 Identity()
		{
			return
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}
	};

	struct RMFLOAT3X3
	{
		union
		{
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			float m[3][3];
		};

		RMFLOAT3X3() = default;

		RMFLOAT3X3(const RMFLOAT3X3&) = default;
		RMFLOAT3X3& operator=(const RMFLOAT3X3&) = default;

		RMFLOAT3X3(RMFLOAT3X3&&) = default;
		RMFLOAT3X3& operator=(RMFLOAT3X3&&) = default;

		constexpr RMFLOAT3X3(float m00, float m01, float m02, 
							 float m10, float m11, float m12,
						   	 float m20, float m21, float m22) noexcept
			  : _11(m00), _12(m01), _13(m02),
				_21(m10), _22(m11), _23(m12),
				_31(m20), _32(m21), _33(m22)
		{

		}

		static RMFLOAT3X3 Identity()
		{
			return RMFLOAT3X3
			(
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			);
		}

		RMFLOAT3X3 operator*(const RMFLOAT3X3& rhs)
		{
			RMFLOAT3X3 result;

			for (int row = 0; row < 3; row++)
			{
				for (int col = 0; col < 3; col++)
				{
					result.m[row][col] =
						m[row][0] * rhs.m[0][col] +
						m[row][1] * rhs.m[1][col] +
						m[row][2] * rhs.m[2][col];
				}
			}

			return result;
		}

		RMFLOAT3X3& operator*=(const RMFLOAT3X3& rhs)
		{
			RMFLOAT3X3 result;
			for (int row = 0; row < 3; row++)
			{
				for (int col = 0; col < 3; col++)
				{
					result.m[row][col] =
						m[row][0] * rhs.m[0][col] +
						m[row][1] * rhs.m[1][col] +
						m[row][2] * rhs.m[2][col];

				}
			}

			*this = result;
			return *this;
		}
	};
}

constexpr float RM_PI = 3.141592654f;
constexpr float RM_2PI = 6.283185307f;
constexpr float RM_1DIVPI = 0.318309886f;
constexpr float RM_1DIV2PI = 0.159154943f;
constexpr float RM_PIDIV2 = 1.570796327f;
constexpr float RM_PIDIV4 = 0.785398163f;

inline float RMConvertToRadians(float fDegrees) noexcept { return fDegrees * (RM_PI / 180.0f); }
inline float RMConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / RM_PI); }

inline float RMFloat3Length(const RocketEngine::RMFLOAT3& f) noexcept;
inline RocketEngine::RMFLOAT3 RMFloat3Normalize(const RocketEngine::RMFLOAT3& f) noexcept;
inline RocketEngine::RMFLOAT4 RMFloat4Normalize(const RocketEngine::RMFLOAT4& f) noexcept;
inline RocketEngine::RMQuaternion RMQuaternionNormalize(const RocketEngine::RMQuaternion& f) noexcept;
constexpr float RMFloat3Dot(const RocketEngine::RMFLOAT3& lhs, const RocketEngine::RMFLOAT3& rhs);
constexpr RocketEngine::RMFLOAT3 RMFloat3Cross(const RocketEngine::RMFLOAT3& lhs, const RocketEngine::RMFLOAT3& rhs);
RocketEngine::RMFLOAT3 RMFloat3MultiplyMatrix(const RocketEngine::RMFLOAT3& lhs, const RocketEngine::RMFLOAT4X4& rhs);
RocketEngine::RMFLOAT4 RMFloat4MultiplyMatrix(const RocketEngine::RMFLOAT4& lhs, const RocketEngine::RMFLOAT4X4& rhs);
RocketEngine::RMQuaternion RMRotateQuaternionY(const RocketEngine::RMQuaternion& quaternion, float radian);
RocketEngine::RMQuaternion RMRotateQuaternion(const RocketEngine::RMQuaternion& quaternion, const RocketEngine::RMFLOAT3& axis, float radian);
RocketEngine::RMQuaternion RMQuaternionMultiply(const RocketEngine::RMQuaternion& lhs, const RocketEngine::RMQuaternion& rhs);
RocketEngine::RMFLOAT4X4 RMMatrixRotationX(float angle);
RocketEngine::RMFLOAT4X4 RMMatrixRotationY(float angle);
RocketEngine::RMFLOAT3X3 RMInverseMatrix(const RocketEngine::RMFLOAT3X3& mat);
RocketEngine::RMFLOAT4 RMQuaternionToFloat4(const RocketEngine::RMQuaternion& quaternion);
RocketEngine::RMFLOAT4 RMFloat4ToQuaternion(const RocketEngine::RMFLOAT4& f4);
RocketEngine::RMQuaternion RMMatrixToQuaternion(const RocketEngine::RMFLOAT4X4& matrix);

RocketEngine::RMFLOAT4X4 RMScaleMatrix(const RocketEngine::RMFLOAT3 scale);
RocketEngine::RMFLOAT4X4 RMRotationMatrix(const RocketEngine::RMQuaternion rotation);
RocketEngine::RMFLOAT4X4 RMTranslateMatrix(const RocketEngine::RMFLOAT3 position);