#include "DirectionalLight.h"
#include "GameObject.h"
#include "Transform.h"

namespace RocketEngine
{
	DirectionalLight::DirectionalLight(GameObject* owner)
		: Light(owner),
		_intensity(1.0f)
	{

	}

	RocketCore::Graphics::DirectionalLightData DirectionalLight::GetLightData() const
	{
		RocketCore::Graphics::DirectionalLightData data;
		data.ambient = _ambient;
		data.diffuse = _diffuse;
		data.specular = _specular;
		data.direction = gameObject->transform.GetForward();
		data.intensity = _intensity;

		return data;
	}

	float DirectionalLight::GetIntensity() const
	{
		return _intensity;
	}

	void DirectionalLight::SetIntensity(float intensity)
	{
		_intensity = intensity;
	}
}
