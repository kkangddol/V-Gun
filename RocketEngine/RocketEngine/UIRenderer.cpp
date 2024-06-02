#include "UIRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "UISystem.h"
#include "InputSystem.h"

namespace RocketCore
{
	UIRenderer::UIRenderer(RocketEngine::GameObject* owner)
		: RocketEngine::Component(owner),
		_width(100.0f),
		_height(100.0f),
		_sortOrder(0),
		_isFocused(false),
		_ignoreFocus(false)
	{
		UISystem::Instance().AddUICurrentScene(this);
	}

	void UIRenderer::Start()
	{
		_isFocused = false;
	}

	RocketEngine::RMFLOAT3X3 UIRenderer::Get2DWorldTM()
	{
		return gameObject->transform.Get2DWorldTM();
	}

	bool UIRenderer::CheckFocus()
	{
		if (_ignoreFocus)
		{
			return false;
		}

		RocketEngine::RMFLOAT2 mousePos = RocketCore::InputSystem::Instance().GetMousePosition();

		if (GetLeft() <= mousePos.x && mousePos.x <= GetRight())
		{
			if (GetTop() <= mousePos.y && mousePos.y <= GetBottom())
			{
				return true;
			}
		}

		return false;
	}

	void UIRenderer::SetSize(float width, float height)
	{
		_width = width;
		_height = height;
	}

	void UIRenderer::SetWidth(float width)
	{
		_width = width;
	}

	void UIRenderer::SetHeight(float height)
	{
		_height = height;
	}

	void UIRenderer::SetSortOrder(int sortOrder)
	{
		_sortOrder = sortOrder;
	}

	void UIRenderer::SetIsFocused(bool isFocused)
	{
		_isFocused = isFocused;
	}

	void UIRenderer::SetIgnoreFocus(bool isIgnore)
	{
		_ignoreFocus = isIgnore;
	}

	float UIRenderer::GetFillRatio() const
	{
		return _fillRatio;
	}

	void UIRenderer::SetFillRatio(float ratio)
	{
		_fillRatio = ratio;
	}

	float UIRenderer::GetLeft() const
	{
		RocketEngine::RMFLOAT3 pos = gameObject->transform.GetPosition();
		RocketEngine::RMFLOAT3 scale = gameObject->transform.GetScale();

		return pos.x - _width * scale.x / 2;
	}

	float UIRenderer::GetTop() const
	{
		RocketEngine::RMFLOAT3 pos = gameObject->transform.GetPosition();
		RocketEngine::RMFLOAT3 scale = gameObject->transform.GetScale();

		return pos.y - _height * scale.y / 2;
	}

	float UIRenderer::GetRight() const
	{
		RocketEngine::RMFLOAT3 pos = gameObject->transform.GetPosition();
		RocketEngine::RMFLOAT3 scale = gameObject->transform.GetScale();

		return pos.x + _width * scale.x / 2;
	}

	float UIRenderer::GetBottom() const
	{
		RocketEngine::RMFLOAT3 pos = gameObject->transform.GetPosition();
		RocketEngine::RMFLOAT3 scale = gameObject->transform.GetScale();

		return pos.y + _height * scale.y / 2;
	}

	int UIRenderer::GetSortOrder() const
	{
		return _sortOrder;
	}

	bool UIRenderer::GetIsFocused() const
	{
		return _isFocused;
	}

	bool UIRenderer::GetIgnoreFocus() const
	{
		return _ignoreFocus;
	}

	float UIRenderer::GetWidth() const
	{
		return _width;
	}

	float UIRenderer::GetHeight() const
	{
		return _height;
	}

}
