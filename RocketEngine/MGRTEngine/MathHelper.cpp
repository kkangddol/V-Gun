// MathHelper.cpp by Frank Luna

#include "MathHelper.h"
#include <float.h>
#include <cmath>
#include "CameraMatrices.h"

const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi = 3.1415926535f;


DirectX::XMFLOAT2 MathHelper::RMToXMFloat2(RocketEngine::RMFLOAT2 val)
{
	return DirectX::XMFLOAT2(val.x, val.y);
}


DirectX::XMFLOAT3 MathHelper::RMToXMFloat3(RocketEngine::RMFLOAT3 val)
{
	return DirectX::XMFLOAT3(val.x, val.y, val.z);
}


DirectX::XMFLOAT4 MathHelper::RMToXMFloat4(RocketEngine::RMFLOAT4 val)
{
	return DirectX::XMFLOAT4(val.x, val.y, val.z, val.w);
}


DirectX::XMFLOAT3X3 MathHelper::RMToXMFloat3x3(RocketEngine::RMFLOAT3X3 val)
{
	DirectX::XMFLOAT3X3 tReturn;
	tReturn._11 = val._11;
	tReturn._12 = val._12;
	tReturn._13 = val._13;
	tReturn._21 = val._21;
	tReturn._22 = val._22;
	tReturn._23 = val._23;
	tReturn._31 = val._31;
	tReturn._32 = val._32;
	tReturn._33 = val._33;
	return tReturn;
}


DirectX::XMFLOAT4X4 MathHelper::RMToXMFloat4x4(RocketEngine::RMFLOAT4X4 val)
{
	DirectX::XMFLOAT4X4 tReturn;
	tReturn._11 = val._11;
	tReturn._12 = val._12;
	tReturn._13 = val._13;
	tReturn._14 = val._14;
	tReturn._21 = val._21;
	tReturn._22 = val._22;
	tReturn._23 = val._23;
	tReturn._24 = val._24;
	tReturn._31 = val._31;
	tReturn._32 = val._32;
	tReturn._33 = val._33;
	tReturn._34 = val._34;
	tReturn._41 = val._41;
	tReturn._42 = val._42;
	tReturn._43 = val._43;
	tReturn._44 = val._44;
	return tReturn;
}


void MathHelper::RMMatrix3x3Decompose(RocketEngine::RMFLOAT3X3 val, RocketEngine::RMFLOAT2& trnsl, float& rot, RocketEngine::RMFLOAT2& scl)
{
	// M11	M12  M13
	// M21	M22  M23
	// M31	M32  M33
	
	//Skew는 제외!
	scl.x = sqrtf(val._11 * val._11 + val._12 * val._12);
	scl.y = sqrtf(val._21 * val._21 + val._22 * val._22);
	rot = atan2(val._12, val._11);
	trnsl.x = val._31;
	trnsl.y = val._32;
}


float MathHelper::FloatLerp(float A, float B, float Alpha)
{
	return A * (1 - Alpha) + B * Alpha;
}

DirectX::XMFLOAT2 MathHelper::Float2Lerp(DirectX::XMFLOAT2 A, DirectX::XMFLOAT2 B, float Alpha)
{
	float tX = A.x * (1 - Alpha) + B.x * Alpha;
	float tY = A.y * (1 - Alpha) + B.y * Alpha;
	return DirectX::XMFLOAT2(tX, tY);
}


bool MathHelper::CheckIfFrustumCulled(
	std::array<DirectX::XMFLOAT4, 6>& frustumPlanes, DirectX::XMFLOAT3 worldPos,
	DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds)
{
	bool toCull = false;

	// Loop through each frustum plane
	for (int planeID = 0; planeID < 6; ++planeID)
	{
		XMVECTOR planeNormal = XMVectorSet(frustumPlanes[planeID].x, frustumPlanes[planeID].y, frustumPlanes[planeID].z, 0.0f);
		float planeConstant = frustumPlanes[planeID].w;

		// Check each axis (x,y,z) to get the AABB vertex furthest away from the direction the plane is facing (plane normal)
		XMFLOAT3 axisVert;

		// x-axis
		if (frustumPlanes[planeID].x < 0.0f)	// Which AABB vertex is furthest down (plane normals direction) the x axis
		{
			axisVert.x = minBounds.x + worldPos.x; // min x plus 오브젝트 World Pos X
		}	
		else
		{
			axisVert.x = maxBounds.x + worldPos.x; // max x plus 오브젝트 World Pos X
		}
			
		// y-axis
		if (frustumPlanes[planeID].y < 0.0f)	// Which AABB vertex is furthest down (plane normals direction) the y axis
		{
			axisVert.y = minBounds.y + worldPos.y; // min y plus 오브젝트 World Pos Y
		}
		else
		{
			axisVert.y = maxBounds.y + worldPos.y; // max y plus 오브젝트 World Pos Y
		}
			
		// z-axis
		if (frustumPlanes[planeID].z < 0.0f)	// Which AABB vertex is furthest down (plane normals direction) the z axis
		{
			axisVert.z = minBounds.z + worldPos.z; // min z plus 오브젝트 World Pos Z
		}
		else
		{
			axisVert.z = maxBounds.z + worldPos.z; // max z plus 오브젝트 World Pos Z
		}
			

		// Now we get the signed distance from the AABB vertex that's furthest down the frustum planes normal,
		// and if the signed distance is negative, then the entire bounding box is behind the frustum plane, which means
		// that it should be culled
		if (XMVectorGetX(XMVector3Dot(planeNormal, XMLoadFloat3(&axisVert))) + planeConstant < 0.0f)
		{
			toCull = true;
			// Skip remaining planes to check and move on to next tree
			break;
		}
	}

	//FrustumCulling 할지 말지 리턴.
	return toCull;
}

float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + Pi; // in [0, 2*pi).

	return theta;
}

XMVECTOR MathHelper::RandUnitVec3()
{
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while (true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		return XMVector3Normalize(v);
	}
}

XMVECTOR MathHelper::RandHemisphereUnitVec3(XMVECTOR n)
{
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while (true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		// Ignore points in the bottom hemisphere.
		if (XMVector3Less(XMVector3Dot(n, v), Zero))
			continue;

		return XMVector3Normalize(v);
	}
}

DirectX::XMFLOAT4X4 MathHelper::CreateTransformMatrix(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& rotationQuaternion, const DirectX::XMFLOAT3& scale)
{
	// Create individual transformation matrices
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotationQuaternion));
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	// Combine the matrices to get the final transformation matrix (order: scale, rotate, translate)
	DirectX::XMMATRIX transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	// Convert the DirectXMath matrix to XMFLOAT4X4 format
	DirectX::XMFLOAT4X4 resultMatrix;
	DirectX::XMStoreFloat4x4(&resultMatrix, transformMatrix);

	return resultMatrix;
}