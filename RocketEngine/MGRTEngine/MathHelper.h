///MathHelper.h By Frank Luna + RM->DX 변환 코드

#pragma once

#include <Windows.h>
// LEHIDE
//#include <xnamath.h>
#include <DirectXMath.h>
#include <array>
#include "../RocketMath/RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

using namespace DirectX;

class MathHelper
{
public:
	//[TW] 커스텀 추가됨.
	static DirectX::XMFLOAT2 RMToXMFloat2(RocketEngine::RMFLOAT2 val);
	static DirectX::XMFLOAT3 RMToXMFloat3(RocketEngine::RMFLOAT3 val);
	static DirectX::XMFLOAT4 RMToXMFloat4(RocketEngine::RMFLOAT4 val);
	static DirectX::XMFLOAT3X3 RMToXMFloat3x3(RocketEngine::RMFLOAT3X3 val);
	static DirectX::XMFLOAT4X4 RMToXMFloat4x4(RocketEngine::RMFLOAT4X4 val);

	static void RMMatrix3x3Decompose(RocketEngine::RMFLOAT3X3 val, RocketEngine::RMFLOAT2& trnsl, float& rot, RocketEngine::RMFLOAT2& scl);

	static float FloatLerp(float A, float B, float Alpha);
	static DirectX::XMFLOAT2 Float2Lerp(DirectX::XMFLOAT2 A, DirectX::XMFLOAT2 B, float Alpha);

	//[TW] 컬링용 함수 추가! YES면, Render. // No면, Render 하지도 않는다!
	static bool CheckIfFrustumCulled(
		std::array<DirectX::XMFLOAT4, 6>& frustumPlanes, DirectX::XMFLOAT3 worldPos,
		DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds);


	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF() * (b - a);
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	static XMVECTOR RandUnitVec3();
	static XMVECTOR RandHemisphereUnitVec3(XMVECTOR n);

	static DirectX::XMFLOAT4X4 CreateTransformMatrix(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& rotationQuaternion, const DirectX::XMFLOAT3& scale);
	
	static const float Infinity;
	static const float Pi;
};
 // MATHHELPER_H