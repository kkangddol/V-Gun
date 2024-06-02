#pragma once
#include "DLLExporter.h"
#include "DynamicCollider.h"
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\RocketUtil.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\RocketUtil.lib")
#endif // _DEBUG

/// <summary>
/// BoxCollider 라는 이름이지만
/// 사실 Rigidbody도 포함되어있습니다.
/// 
/// 23.08.01 강석원&안준용 인재원.
/// </summary>
namespace RocketEngine
{
	class GameObject;

	class ROCKET_API BoxCollider final : public DynamicCollider
	{
	public:
		BoxCollider(GameObject* owner);


		/// 디버그 정보 보내기.
	protected:
		virtual void Update() override;

	public:
		void SetVolume(float w, float h, float d);
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	private:
		float _width;
		float _height;
		float _depth;
	};
}
