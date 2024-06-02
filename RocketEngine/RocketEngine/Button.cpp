#include "Button.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputSystem.h"
#include "DebugSystem.h"
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG


namespace RocketEngine
{
	Button::Button(GameObject* owner)
		: UIRenderer(owner),
		_onClickEvent()
	{

	}

	void Button::LateUpdate()
	{
		// 디버그 정보 출력.
		RMFLOAT3 worldPos = gameObject->transform.GetPosition();
		RMFLOAT2 LT = { worldPos.x - _width / 2, worldPos.y - _height / 2 };
		RMFLOAT2 RB = { worldPos.x + _width / 2, worldPos.y + _height / 2 };
		RMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f };
		RocketCore::DebugSystem::Instance().DrawDebug2DBox(LT, RB, color);
	}

	void Button::OnFocusEvent()
	{
		if (!_onClickEvent)
		{
			return;
		}

		_onClickEvent();
	}

	void Button::SetOnClickEvent(std::function<void()> func)
	{
		_onClickEvent = func;
	}

	std::function<void()> Button::GetOnClickEvent() const
	{
		return _onClickEvent;
	}
}
