#include "CameraMatrices.h"
#include <cassert>


namespace RocketCore::Graphics
{
	CameraMatrices::CameraMatrices() :
		viewMat(DirectX::XMFLOAT4X4()), projectionMat(DirectX::XMFLOAT4X4()),
		mPosition(0.0f, 0.0f, 0.0f), mRight(1.0f, 0.0f, 0.0f),
		mUp(0.0f, 1.0f, 0.0f), mLook(0.0f, 0.0f, 1.0f)
	{

	}

	//CameraData에서 지속적으로 업데이트될 정보 (Position, Rotation을 가지고 다시 Axis 계산해서 넣기)
	void CameraMatrices::UpdateLocalAxisValues(const CameraData* camData)
	{
		this->mPosition = MathHelper::RMToXMFloat3(camData->position);

		DirectX::XMFLOAT4 tFF = MathHelper::RMToXMFloat4(RMQuaternionToFloat4(camData->rotation));
		DirectX::XMVECTOR tFFVec = DirectX::XMVectorZero();
		tFFVec = DirectX::XMLoadFloat4(&tFF);
		
		DirectX::XMVECTOR tAxisAngle;
		float tRadAngle = 0.0f;
		DirectX::XMQuaternionToAxisAngle(&tAxisAngle, &tRadAngle, tFFVec);
		DirectX::XMFLOAT4 tAAFFRadian = DirectX::XMFLOAT4();
		DirectX::XMStoreFloat4(&tAAFFRadian, tAxisAngle);
		//DirectX::XMMATRIX tRotMat = DirectX::XMMatrixRotationRollPitchYaw(tAAFFRadian.x, tAAFFRadian.y, tAAFFRadian.z);
		DirectX::XMMATRIX tRotMat = DirectX::XMMatrixRotationQuaternion(tFFVec);
		DirectX::XMFLOAT4X4 tRotMatFloat = DirectX::XMFLOAT4X4();
		DirectX::XMStoreFloat4x4(&tRotMatFloat, tRotMat);
		DirectX::XMFLOAT3 tForward = DirectX::XMFLOAT3(tRotMatFloat._31, tRotMatFloat._32, tRotMatFloat._33);

		// Extract the up vector (positive y-axis)
		DirectX::XMFLOAT3 tUp = DirectX::XMFLOAT3(tRotMatFloat._21, tRotMatFloat._22, tRotMatFloat._23);

		// Extract the right vector (positive x-axis)
		DirectX::XMFLOAT3 tRight = DirectX::XMFLOAT3(tRotMatFloat._11, tRotMatFloat._12, tRotMatFloat._13);

		DirectX::XMVECTOR tForwardVec = DirectX::XMLoadFloat3(&tForward);
		DirectX::XMVECTOR tUpVec = DirectX::XMLoadFloat3(&tUp);
		DirectX::XMVECTOR tRightVec = DirectX::XMLoadFloat3(&tRight);

		tForwardVec = DirectX::XMVector3Normalize(tForwardVec);
		tUpVec = DirectX::XMVector3Normalize(tUpVec);
		tRightVec = DirectX::XMVector3Normalize(tRightVec);

		////업데이트한 값을 저장.
		DirectX::XMStoreFloat3(&mLook, tForwardVec);
		DirectX::XMStoreFloat3(&mUp, tUpVec);
		DirectX::XMStoreFloat3(&mRight, tRightVec);
	}

	void CameraMatrices::UpdateViewMatrix(const CameraData* camData)
	{
		//Projection 매번 받아서 계산.
		DirectX::XMMATRIX tProj = DirectX::XMMatrixPerspectiveFovLH(camData->fovY, camData->aspect, camData->nearZ, camData->farZ);
		XMStoreFloat4x4(&projectionMat, tProj);

		//View 계산.
		DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&mRight);
		DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&mUp);
		DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&mLook);
		DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&mPosition);

		//여기서 정규직교화해야 한다.
		// Keep camera's axes orthogonal to each other and of unit length.
		L = DirectX::XMVector3Normalize(L);
		U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = DirectX::XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
		float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
		float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

		DirectX::XMStoreFloat3(&mRight, R);
		DirectX::XMStoreFloat3(&mUp, U);
		DirectX::XMStoreFloat3(&mLook, L);

		viewMat(0, 0) = mRight.x;
		viewMat(1, 0) = mRight.y;
		viewMat(2, 0) = mRight.z;
		viewMat(3, 0) = x;

		viewMat(0, 1) = mUp.x;
		viewMat(1, 1) = mUp.y;
		viewMat(2, 1) = mUp.z;
		viewMat(3, 1) = y;

		viewMat(0, 2) = mLook.x;
		viewMat(1, 2) = mLook.y;
		viewMat(2, 2) = mLook.z;
		viewMat(3, 2) = z;

		viewMat(0, 3) = 0.0f;
		viewMat(1, 3) = 0.0f;
		viewMat(2, 3) = 0.0f;
		viewMat(3, 3) = 1.0f;

		assert(&viewMat);
	}

	DirectX::XMMATRIX CameraMatrices::View() const
	{
		return DirectX::XMLoadFloat4x4(&viewMat);
	}

	DirectX::XMMATRIX CameraMatrices::Proj() const
	{
		return DirectX::XMLoadFloat4x4(&projectionMat);
	}

	void CameraMatrices::UpdateFrustumPlanes()
	{
		//이미 이 상태에서 View/Proj는 들어가 있는 상태이다. 
		//Culling을 위해서, Frustum Plane들을 미리 구해 놓자!
		DirectX::XMMATRIX tViewProjMat = DirectX::XMMatrixMultiply(View(), Proj());
		DirectX::XMFLOAT4X4 viewProj;
		DirectX::XMStoreFloat4x4(&viewProj, tViewProjMat);

		// x, y, z, and w represent A, B, C and D in the plane equation
	// where ABC are the xyz of the planes normal, and D is the plane constant

		// Left Frustum Plane
		// Add first column of the matrix to the fourth column
		_frustumPlanes[0].x = viewProj._14 + viewProj._11;
		_frustumPlanes[0].y = viewProj._24 + viewProj._21;
		_frustumPlanes[0].z = viewProj._34 + viewProj._31;
		_frustumPlanes[0].w = viewProj._44 + viewProj._41;

		// Right Frustum Plane
		// Subtract first column of matrix from the fourth column
		_frustumPlanes[1].x = viewProj._14 - viewProj._11;
		_frustumPlanes[1].y = viewProj._24 - viewProj._21;
		_frustumPlanes[1].z = viewProj._34 - viewProj._31;
		_frustumPlanes[1].w = viewProj._44 - viewProj._41;

		// Top Frustum Plane
		// Subtract second column of matrix from the fourth column
		_frustumPlanes[2].x = viewProj._14 - viewProj._12;
		_frustumPlanes[2].y = viewProj._24 - viewProj._22;
		_frustumPlanes[2].z = viewProj._34 - viewProj._32;
		_frustumPlanes[2].w = viewProj._44 - viewProj._42;

		// Bottom Frustum Plane
		// Add second column of the matrix to the fourth column
		_frustumPlanes[3].x = viewProj._14 + viewProj._12;
		_frustumPlanes[3].y = viewProj._24 + viewProj._22;
		_frustumPlanes[3].z = viewProj._34 + viewProj._32;
		_frustumPlanes[3].w = viewProj._44 + viewProj._42;

		// Near Frustum Plane
		// We could add the third column to the fourth column to get the near plane,
		// but we don't have to do this because the third column IS the near plane
		_frustumPlanes[4].x = viewProj._13;
		_frustumPlanes[4].y = viewProj._23;
		_frustumPlanes[4].z = viewProj._33;
		_frustumPlanes[4].w = viewProj._43;

		// Far Frustum Plane
		// Subtract third column of matrix from the fourth column
		_frustumPlanes[5].x = viewProj._14 - viewProj._13;
		_frustumPlanes[5].y = viewProj._24 - viewProj._23;
		_frustumPlanes[5].z = viewProj._34 - viewProj._33;
		_frustumPlanes[5].w = viewProj._44 - viewProj._43;

		// Normalize plane normals (A, B and C (xyz))
		// Also take note that planes face inward
		for (int i = 0; i < 6; ++i)
		{
			float length = sqrt((_frustumPlanes[i].x * _frustumPlanes[i].x) + (_frustumPlanes[i].y * _frustumPlanes[i].y) + (_frustumPlanes[i].z * _frustumPlanes[i].z));
			_frustumPlanes[i].x /= length;
			_frustumPlanes[i].y /= length;
			_frustumPlanes[i].z /= length;
			_frustumPlanes[i].w /= length;
		}
	}

}
