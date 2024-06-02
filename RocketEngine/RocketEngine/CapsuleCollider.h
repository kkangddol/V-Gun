#pragma once
#include "DLLExporter.h"
#include "DynamicCollider.h"
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketEngine
{
	class GameObject;

	class ROCKET_API CapsuleCollider final : public DynamicCollider
	{
	public:
		CapsuleCollider(GameObject* owner);

		/// 디버그 정보 보내기.
	protected:
		virtual void Update() override;

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		float GetHalfHeight() const;
		void SetCapsuleInfo(float radius, float halfHeight);

		void SetRadius(float rad);
		void SetHalfHeight(float val);

	private:
		float _radius;
		float _halfHeight;
	};
}
