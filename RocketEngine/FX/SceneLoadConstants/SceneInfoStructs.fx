//RocketEngine에서 렌더링에 호환되는 자료형들의 집합체.

#ifndef __DEFINED_SCENE_INFO_STRUCTS_HLSL__
#define __DEFINED_SCENE_INFO_STRUCTS_HLSL__

//위치에 상관없는 Directional Light.
struct DirectionalLightData
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;
    float3 _direction;
    float  _intensity; 
    //자동으로 GPU 바로 투입을 위한 바이트 패딩 수행됨.
};

//위치 상관 O, 방향 상관 X, Point Light.
struct PointLightData
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;

	//GPU용 4D Vector 패킹.
    float3 _position;
    float  _range;

	//GPU용 4D Vector 패킹.
    float3 _attenuation;
    float tPad;
};

//위치 상관 O, 방향 상관 O, Spot Light.
struct SpotLightData
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;

		//GPU용 4D Vector 패킹.
    float3 _position;
    float  _range;

		//GPU용 4D Vector 패킹.
    float3 _direction;
    float  _spot;

		//GPU용 4D Vector 패킹.
    float3 _attenuation;
    float  tPad;
};

struct LegacyMaterialData
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular; // w : Specular Power.
    float4 _reflect;    //1차 모델에서는 작동 안할 것.
};

struct PBRMaterialData
{
	//모두 다 0~1 범위에 있어야 한다. 
	//금속/유전체 비교는 Shader의 다른 변수를 업데이트해서 가를 것.
    float4 _albedo; // RGBA, 각각 0-1 사이. Base Color/Albedo.
    float _metallic; // 금속성, 0-1. (유전체일 경우 영향 X)
    float _roughness; // 거침의 정도, 0-1. 
    float _specular; // 스페큘러, 0-1. (금속일 경우 영향 X)
    float _emission; // Material 자체가 얼마나 빛을 내는가, 0-1. (일단 다른 옵젝의 라이팅에는 영향 X)

    ///gPBRMaterial 중, Specular, Emission은 1차에서 영향을 미치지 않을 것!
};

#endif //__DEFINED_SCENE_INFO_STRUCTS_HLSL__