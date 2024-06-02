#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include <vector>

#if defined(_DX11) || defined(_DX12)
#include "..\\RocketMath\\RocketMath.h"
#endif // _DX11 || _DX12

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif //_DEBUG

namespace physx
{
	class PxScene;
}

namespace RocketEngine
{
	class GameObject;
}

namespace RocketEngine
{
	class ROCKET_API Collider : public Component
	{
	public:
		Collider(GameObject* owner);

	// Collider 자체적으로 들고 있는 TRS Offset과 그 getter, setter
	public:
		void SetPositionOffset(RMFLOAT3 position);
		void SetRotationOffset(RMQuaternion rotation);
		void SetRotationOffset(float angleX, float angleY, float angleZ);
		void SetScaleOffset(RMFLOAT3 scale);

		RMFLOAT3 GetPositionOffset() const;
		RMQuaternion GetRotationOffset() const;
		RMFLOAT3 GetScaleOffset() const;

		RMFLOAT4X4 GetOffsetTranslateMatrix() const;
		RMFLOAT4X4 GetOffsetRotationMatrix() const;
		RMFLOAT4X4 GetOffsetScaleMatrix() const;

		RMFLOAT4X4 GetOffsetTM() const;

	protected:
		RMFLOAT3 _positionOffset;
		RMQuaternion _rotationOffset;
		RMFLOAT3 _scaleOffset;

	public:
		virtual void UpdateToPhysics() abstract;

	protected:
		bool _wasTranslated = false;

	public:
		void SetPxScene(physx::PxScene* pxScene);
		physx::PxScene* GetPxScene();

	protected:
		physx::PxScene* _pxScene;
	};
}
