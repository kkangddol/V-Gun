#pragma once
#include "DLLExporter.h"
#include "StaticCollider.h"
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
/// BoxCollider ��� �̸�������
/// ��� Rigidbody�� ���ԵǾ��ֽ��ϴ�.
/// 
/// 23.08.01 ������&���ؿ� �����.
/// </summary>
namespace RocketEngine
{
	class GameObject;

	class ROCKET_API StaticBoxCollider final : public StaticCollider
	{
	public:
		StaticBoxCollider(GameObject* owner);

		/// ����� ���� ������.
	protected:
		virtual void Update() override;

	public:
		void SetVolume(float w, float h, float d);
		float GetWidth() const;
		float GetHeight() const;
		float GetDepth() const;

	private:
		float _width;
		float _height;
		float _depth;
	};
}
