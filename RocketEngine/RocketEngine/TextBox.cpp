#include "TextBox.h" 
#include "InputSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "DebugSystem.h"
#include "TimeSystem.h"

namespace RocketEngine
{
	float downTime = 0.0f;

	TextBox::TextBox(GameObject* owner)
		: RocketCore::UIRenderer(owner),
		_text("default"),
		_fontSize(20.0f),
		_color(1.0f, 1.0f, 1.0f, 1.0f)
	{

	}

	std::string TextBox::GetSketchData()
	{
		return _text;
	}

	void TextBox::Update()
	{
		if (!_isFocused)
		{
			return;
		}

		for (int keyCode = 0x30; keyCode <= 0x5A; keyCode++)
		{
			if (RocketCore::InputSystem::Instance().GetKeyDown(keyCode))
			{
				AppendText(keyCode);
			}
		}

		if (RocketCore::InputSystem::Instance().GetKeyDown(VK_BACK))
		{
			PopBackText();
		}

		if (RocketCore::InputSystem::Instance().GetKey(VK_BACK))
		{
			downTime += RocketCore::TimeSystem::Instance().GetDeltaTime();

			if (downTime >= 0.5f)
			{
				PopBackText();
			}
		}

		if (RocketCore::InputSystem::Instance().GetKeyUp(VK_BACK))
		{
			downTime = 0.0f;
		}

		if (RocketCore::InputSystem::Instance().GetKeyDown(VK_DELETE))
		{
			PopBackText();
		}

		if (RocketCore::InputSystem::Instance().GetKey(VK_DELETE))
		{
			downTime += RocketCore::TimeSystem::Instance().GetDeltaTime();

			if (downTime >= 0.5f)
			{
				PopBackText();
			}
		}

		if (RocketCore::InputSystem::Instance().GetKeyUp(VK_DELETE))
		{
			downTime = 0.0f;
		}
	}

	void TextBox::LateUpdate()
	{
		// 디버그 정보 출력.
		RMFLOAT3 worldPos = gameObject->transform.GetPosition();
		RMFLOAT2 LT = { worldPos.x - _width / 2, worldPos.y - _height / 2 };
		RMFLOAT2 RB = { worldPos.x + _width / 2, worldPos.y + _height / 2 };
		RMFLOAT4 color = { 0.0f,0.0f,1.0f,1.0f };
		RocketCore::DebugSystem::Instance().DrawDebug2DBox(LT, RB, color);
	}

	void TextBox::SetText(std::string text)
	{
		_text = text;
	}

	void TextBox::AppendText(std::string text)
	{
		_text += text;
	}

	void TextBox::AppendText(int keyCode)
	{
		_text += keyCode;
	}

	void TextBox::PopBackText()
	{
		if (_text.size() > 0)
		{
			_text.pop_back();
		}
	}

	std::string TextBox::GetText() const
	{
		return _text;
	}

	float TextBox::GetFontSize() const
	{
		return _fontSize;
	}

	RocketEngine::RMFLOAT4 TextBox::GetColor() const
	{
		return _color;
	}

	void TextBox::SetFontSize(float size)
{
		_fontSize = size;
	}

	void TextBox::SetFontColor(RMFLOAT4 color)
	{
		_color = color;
	}

}
