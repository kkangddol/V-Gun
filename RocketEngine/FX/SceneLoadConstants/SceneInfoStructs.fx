//RocketEngine���� �������� ȣȯ�Ǵ� �ڷ������� ����ü.

#ifndef __DEFINED_SCENE_INFO_STRUCTS_HLSL__
#define __DEFINED_SCENE_INFO_STRUCTS_HLSL__

//��ġ�� ������� Directional Light.
struct DirectionalLightData
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;
    float3 _direction;
    float  _intensity; 
    //�ڵ����� GPU �ٷ� ������ ���� ����Ʈ �е� �����.
};

//��ġ ��� O, ���� ��� X, Point Light.
struct PointLightData
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;

	//GPU�� 4D Vector ��ŷ.
    float3 _position;
    float  _range;

	//GPU�� 4D Vector ��ŷ.
    float3 _attenuation;
    float tPad;
};

//��ġ ��� O, ���� ��� O, Spot Light.
struct SpotLightData
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;

		//GPU�� 4D Vector ��ŷ.
    float3 _position;
    float  _range;

		//GPU�� 4D Vector ��ŷ.
    float3 _direction;
    float  _spot;

		//GPU�� 4D Vector ��ŷ.
    float3 _attenuation;
    float  tPad;
};

struct LegacyMaterialData
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular; // w : Specular Power.
    float4 _reflect;    //1�� �𵨿����� �۵� ���� ��.
};

struct PBRMaterialData
{
	//��� �� 0~1 ������ �־�� �Ѵ�. 
	//�ݼ�/����ü �񱳴� Shader�� �ٸ� ������ ������Ʈ�ؼ� ���� ��.
    float4 _albedo; // RGBA, ���� 0-1 ����. Base Color/Albedo.
    float _metallic; // �ݼӼ�, 0-1. (����ü�� ��� ���� X)
    float _roughness; // ��ħ�� ����, 0-1. 
    float _specular; // ����ŧ��, 0-1. (�ݼ��� ��� ���� X)
    float _emission; // Material ��ü�� �󸶳� ���� ���°�, 0-1. (�ϴ� �ٸ� ������ �����ÿ��� ���� X)

    ///gPBRMaterial ��, Specular, Emission�� 1������ ������ ��ġ�� ���� ��!
};

#endif //__DEFINED_SCENE_INFO_STRUCTS_HLSL__