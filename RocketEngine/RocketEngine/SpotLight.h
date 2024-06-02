#pragma once
#include "DLLExporter.h"
#include "Light.h"

#include "..\\RocketGraphicsInterface\\LightData.h"

namespace RocketEngine
{
	class GameObject;
}

namespace RocketEngine
{
	class ROCKET_API SpotLight final : public Light
	{
	public:
		SpotLight(GameObject* owner);

	public:
		RocketCore::Graphics::SpotLightData GetLightData() const;

	public:
		float GetRange() const;
		void SetRange(float range);
		float GetSpot() const;
		void SetSpot(float spot);
		RMFLOAT3 GetAttenuation() const;
		void SetAttenuation(RMFLOAT3 attenuation);

	private:
		float _range;
		float _spot;
		RMFLOAT3 _attenuation;
	};
}
