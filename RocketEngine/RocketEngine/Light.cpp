#include "DLLExporter.h"
#include "Light.h"
#include "RenderSystem.h"

namespace RocketEngine
{
	Light::Light(GameObject* owner)
		: Component(owner),
		_ambient(1.0f,1.0f,1.0f,1.0f),
		_diffuse(1.0f,1.0f,1.0f,1.0f),
		_specular(1.0f,1.0f,1.0f,1.0f)
	{
		//RocketCore::RenderSystem::Instance().AddLight(this);
	}

	RocketEngine::RMFLOAT4 Light::GetAmbient() const
	{
		return _ambient;
	}

	void Light::SetAmbient(RMFLOAT4 ambient)
	{
		_ambient = ambient;
	}

	RocketEngine::RMFLOAT4 Light::GetDiffuse() const
	{
		return _diffuse;
	}

	void Light::SetDiffuse(RMFLOAT4 diffuse)
	{
		_diffuse = diffuse;
	}

	RocketEngine::RMFLOAT4 Light::GetSpecular() const
	{
		return _specular;
	}

	void Light::SetSpecular(RMFLOAT4 specular)
	{
		_specular = specular;
	}

}
