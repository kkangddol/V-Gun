///[TW] Legacy, PBR Material 모두를 담는 코드이다.
///기존 Map에서 읽어오는 값을 얼마나 반영할지를 담고 있다.

#pragma once
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore::Graphics
{
	struct LegacyMaterialData
	{
		RocketEngine::RMFLOAT4 _ambient;
		RocketEngine::RMFLOAT4 _diffuse;
		RocketEngine::RMFLOAT4 _specular; // w : Specular Power.
		RocketEngine::RMFLOAT4 _reflect;
	};

	struct PBRMaterialData
	{
		//모두 다 0~1 범위에 있어야 한다. 
		//금속/유전체 비교는 Shader의 다른 변수를 업데이트해서 가를 것.
		RocketEngine::RMFLOAT4 _albedo;		// RGBA, 각각 0-1 사이. Base Color/Albedo.
		float _metallic;						// 금속성, 0-1. (유전체일 경우 영향 X)
		float _roughness;					// 거침의 정도, 0-1. 
		float _specular;						// 스페큘러, 0-1. (금속일 경우 영향 X)
		float _emission;						// Material 자체가 얼마나 빛을 내는가, 0-1. (일단 다른 옵젝의 라이팅에는 영향 X)
	
		///PBRMaterialData 중, Specular, Emission은 일단 영향을 미치지 않을 것!
	};
}


