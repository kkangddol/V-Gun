/// Frank Luna의 LightHelper.fx를 RocketEngine에 맞게 개조함.
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
	// 출력 결과 초기화.
    outAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    outSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Light Ray가 오는 방향 반대로 Light Vector 위치시키기.
    float3 lightVec = -dirLight._direction;

	// Ambient 값 설정 (LegacyMaterial 연동)
    outAmbient = material._ambient * dirLight._ambient;

	//빛이 표면으로 들어온다면, Diffuse/Specular 추가!
	float diffuseFactor = dot(lightVec, normal);

	// 셰이더 동적 브랜칭 막기 위해 flatten.
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

	// 빛 벡터 정규화.
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
    float3 normalT = 2.0f * normalMapSample - 1.0f; // 0-1에서 -1-1로.
	
	//정규직교 기저.
    float3 N = unitNormalW; // Normal
    float3 T = normalize(tangentW - dot(tangentW, N) * N); // Tangent
    float3 B = cross(N, T); // Binormal
	
    float3x3 TBN = float3x3(T, B, N);
	
    float3 bumpedNormalW = mul(normalT, TBN);
    return bumpedNormalW;
}

#endif //__DEFINED_LIGHTHELPER_HLSL__