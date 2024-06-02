#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketEngine
{
	class GameObject;
}

namespace RocketEngine
{
	class ROCKET_API Light : public Component
	{
	public:
		Light(GameObject* owner);

	public:
		RMFLOAT4 GetAmbient() const;
		void SetAmbient(RMFLOAT4 ambient);
		RMFLOAT4 GetDiffuse() const;
		void SetDiffuse(RMFLOAT4 diffuse);
		RMFLOAT4 GetSpecular() const;
		void SetSpecular(RMFLOAT4 specular);

	protected:
		RMFLOAT4 _ambient;
		RMFLOAT4 _diffuse;
		RMFLOAT4 _specular;
	};
}
