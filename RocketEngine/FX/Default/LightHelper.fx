/// Frank Luna�� LightHelper.fx�� RocketEngine�� �°� ������.
#include "../SceneLoadConstants/SceneInfoStructs.fx"

#ifndef __DEFINED_LIGHTHELPER_HLSL__
#define __DEFINED_LIGHTHELPER_HLSL__

void ComputeDirectionalLight_Legacy(
	LegacyMaterialData material, DirectionalLightData dirLight,
	float3 normal, float3 toEye,
	out float4 outAmbient,
	out float4 outDiffuse,
	out float4 outSpecular)
{
	// ��� ��� �ʱ�ȭ.
    outAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Light Ray�� ���� ���� �ݴ�� Light Vector ��ġ��Ű��.
    float3 lightVec = -dirLight._direction;

	// Ambient �� ���� (LegacyMaterial ����)
    outAmbient = material._ambient * dirLight._ambient;

	//���� ǥ������ ���´ٸ�, Diffuse/Specular �߰�!
	float diffuseFactor = dot(lightVec, normal);

	// ���̴� ���� �귣Ī ���� ���� flatten.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), material._specular.w);

        outDiffuse = diffuseFactor * material._diffuse * dirLight._diffuse;
        outSpecular = specFactor * material._specular * dirLight._specular;
    }
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------

void ComputePointLight_Legacy(
	LegacyMaterialData material, PointLightData pointLight, 
	float3 pos, float3 normal, float3 toEye,
	out float4 outAmbient, out float4 outDiffuse, out float4 outSpecular)
{
	// Initialize outputs.
    outAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
    float3 lightVec = pointLight._position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test.
	if (d > pointLight._range)
    {
        return;
    }

	// �� ���� ����ȭ.
	lightVec /= d;

	// Ambient term.
	outAmbient = material._ambient * pointLight._ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), material._specular.w);

		outDiffuse = diffuseFactor * material._diffuse * pointLight._diffuse;
        outSpecular = specFactor * material._specular * pointLight._specular;
    }

	// Attenuate
    float att = 1.0f / dot(pointLight._attenuation, float3(1.0f, d, d * d));

	outDiffuse *= att;
	outSpecular *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight_Legacy(LegacyMaterialData material, SpotLightData spotLight, float3 pos, float3 normal, float3 toEye,
	out float4 outAmbient, out float4 outDiffuse, out float4 outSpecular)
{
	// Initialize outputs.
    outAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
    float3 lightVec = spotLight._position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test.
	if (d > spotLight._range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	outAmbient = material._ambient * spotLight._ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), material._specular.w);

		outDiffuse = diffuseFactor * material._diffuse * spotLight._diffuse;
		outSpecular = specFactor * material._specular * spotLight._specular;
	}

	// Scale by spotlight factor and attenuate.
    float spot = pow(max(dot(-lightVec, spotLight._direction), 0.0f), spotLight._spot);

	// Scale by spotlight factor and attenuate.
    float att = spot / dot(spotLight._attenuation, float3(1.0f, d, d * d));

	outAmbient *= spot;
	outDiffuse *= att;
	outSpecular *= att;
}

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    float3 normalT = 2.0f * normalMapSample - 1.0f; // 0-1���� -1-1��.
	
	//�������� ����.
    float3 N = unitNormalW; // Normal
    float3 T = normalize(tangentW - dot(tangentW, N) * N); // Tangent
    float3 B = cross(N, T); // Binormal
	
    float3x3 TBN = float3x3(T, B, N);
	
    float3 bumpedNormalW = mul(normalT, TBN);
    return bumpedNormalW;
}

#endif //__DEFINED_LIGHTHELPER_HLSL__