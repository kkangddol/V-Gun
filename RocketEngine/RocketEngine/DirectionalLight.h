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
	class ROCKET_API DirectionalLight final : public Light
	{
	public:
		DirectionalLight(GameObject* owner);

	public:
		RocketCore::Graphics::DirectionalLightData GetLightData() const;

	public:
		float GetIntensity() const;
		void SetIntensity(float intensity);

	private:
		float _intensity;
	};
}
