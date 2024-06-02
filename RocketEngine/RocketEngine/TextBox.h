#pragma once
#include "DLLExporter.h"
#include "UIRenderer.h"
#include <string>
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif //_DEBUG

namespace RocketEngine
{
	class GameObject;
}

namespace RocketEngine
{
	/// <summary>
	/// Text 정보를 담고 출력을 담당하는 컴포넌트.
	/// TextBox의 역할도 한다.
	/// 
	/// 23.08.11 강석원
	/// </summary>
	class ROCKET_API TextBox : public RocketCore::UIRenderer
	{
	public:
		TextBox(GameObject* owner);

	public:
		virtual std::string GetSketchData() override;

	protected:
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		std::string GetText() const;
		void SetText(std::string text);

		void AppendText(std::string text);
		void AppendText(int keyCode);
		void PopBackText();

	public:
		float GetFontSize() const;
		RMFLOAT4 GetColor() const;
		void SetFontSize(float size);
		void SetFontColor(RMFLOAT4 color);

	private:
		std::string _text;
		float _fontSize;
		RMFLOAT4 _color;
	};
}
