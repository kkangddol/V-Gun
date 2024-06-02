#include "SpriteRenderer.h"

//디버그용
#include <windows.h>
#include "GameObject.h"
#include "Transform.h"
#include "TimeSystem.h"

namespace RocketEngine
{
	SpriteRenderer::SpriteRenderer(GameObject* owner)
		: RocketCore::UIRenderer(owner),
		_spritePath()
	{

	}

	std::string SpriteRenderer::GetSketchData()
	{
		return _spritePath;
	}

	void SpriteRenderer::SetPath(std::string path)
	{
		_spritePath = path;
	}

	std::string SpriteRenderer::GetPath() const
	{
		return _spritePath;
	}
}
