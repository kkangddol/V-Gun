#include "UISystem.h"
#include "UIRenderer.h"
#include "InputSystem.h"
#include "GameObject.h"
#include "SceneSystem.h"
#include "Scene.h"

namespace RocketCore
{
	UISystem::UISystem()
	{

	}

	void UISystem::Initialize()
	{

	}

	void UISystem::Finalize()
	{

	}

	void UISystem::CheckFocusCurrentScene()
	{
		SceneSystem::Instance().GetCurrentScene()->CheckFocus();
	}

	void UISystem::AddUICurrentScene(UIRenderer* ui)
	{
		SceneSystem::Instance().GetCurrentScene()->AddUI(ui);
	}

}
