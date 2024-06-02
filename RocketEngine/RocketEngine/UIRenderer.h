#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include <string>
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG


namespace RocketEngine
{
	class GameObject;
}

namespace RocketCore
{
	class ROCKET_API UIRenderer : public RocketEngine::Component
	{
	public:
		UIRenderer(RocketEngine::GameObject* owner);

	protected:
		virtual void Start() override;

	public:
		RocketEngine::RMFLOAT3X3 Get2DWorldTM();
		virtual std::string GetSketchData() { return "default"; }

	public:
		virtual void OnFocusEvent() {};

	public:
		bool CheckFocus();

		/// UI Size.
	public:
		float GetWidth() const;
		float GetHeight() const;

	public:
		/// ��ũ�� ���� ��ġ (Scale�� ������)
		float GetLeft() const;		// ��ũ�� ���� left (Scale�� ������)
		float GetTop() const;		// ��ũ�� ���� up (Scale�� ������)
		float GetRight() const;		// ��ũ�� ���� right (Scale�� ������)
		float GetBottom() const;		// ��ũ�� ���� down (Scale�� ������)
		int GetSortOrder() const;
		bool GetIsFocused() const;
		bool GetIgnoreFocus() const;

		void SetSize(float width, float height);
		void SetWidth(float width);
		void SetHeight(float height);
		void SetSortOrder(int sortOrder);
		void SetIsFocused(bool isFocused);
		void SetIgnoreFocus(bool isIgnore);

	protected:
		float _width;
		float _height;
		int _sortOrder;		// ���ڰ� �������� ������ ����.
		bool _isFocused;
		bool _ignoreFocus;

	public:
		float GetFillRatio() const;
		void SetFillRatio(float ratio);

	private:
		float _fillRatio = 100.0f;
	};
}


