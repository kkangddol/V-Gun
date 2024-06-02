///[TW] Legacy, PBR Material ��θ� ��� �ڵ��̴�.
///���� Map���� �о���� ���� �󸶳� �ݿ������� ��� �ִ�.

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
		//��� �� 0~1 ������ �־�� �Ѵ�. 
		//�ݼ�/����ü �񱳴� Shader�� �ٸ� ������ ������Ʈ�ؼ� ���� ��.
		RocketEngine::RMFLOAT4 _albedo;		// RGBA, ���� 0-1 ����. Base Color/Albedo.
		float _metallic;						// �ݼӼ�, 0-1. (����ü�� ��� ���� X)
		float _roughness;					// ��ħ�� ����, 0-1. 
		float _specular;						// ����ŧ��, 0-1. (�ݼ��� ��� ���� X)
		float _emission;						// Material ��ü�� �󸶳� ���� ���°�, 0-1. (�ϴ� �ٸ� ������ �����ÿ��� ���� X)
	
		///PBRMaterialData ��, Specular, Emission�� �ϴ� ������ ��ġ�� ���� ��!
	};
}


