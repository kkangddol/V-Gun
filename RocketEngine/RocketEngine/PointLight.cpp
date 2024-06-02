#include "DLLExporter.h"
#include "PointLight.h"

#include "GameObject.h"
#include "Transform.h"

namespace RocketEngine
{

	PointLight::PointLight(GameObject* owner)
		: Light(owner),
		_range(1.0f),
		_attenuation(1.0f,1.0f,1.0f)
	{

	}

	RocketCore::Graphics::PointLightData PointLight::GetLightData() const
	{
		RocketCore::Graphics::PointLightData data;
		data.ambient = _ambient;
		data.diffuse = _diffuse;
		data.specular = _specular;
		data.position = gameObject->transform.GetLocalPosition();
		data.range = _range;
		data.attenuation = _attenuation;
		data.tPad = 666.0f;

		return data;
	}

	float PointLight::GetRange() const
	{
		return _range;
	}

	void PointLight::SetRange(float range)
	{
		_range = range;
	}

	RocketEngine::RMFLOAT3 PointLight::GetAttenuation() const
	{
		return _attenuation;
	}

	void PointLight::SetAttenuation(RMFLOAT3 attenuation)
	{
		_attenuation = attenuation;
	}

}
