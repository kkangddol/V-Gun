/// Pos / Normal / Tex : Light ����Ʈ. (Material�� Legacy�� ���� ����)
/// �ٸ� Light�� ���� �����ϳ�, ����μ��� �׳� Legacy Version Directional Light�� �ǰ� �����س���.
//���� NormalMapping���� ������.

//�̸� ������Ʈ�Ǵ� �� ������ ������ ����, Ŀ���� �ڷ����� �˰� ��.
#include "LightHelper.fx"
#include "../SceneLoadConstants/SceneInfoStructs.fx"

#define POSNORMALTEX_MAXIMUM_LIGHT_COUNT 10

cbuffer cbPerFrame
{
    float3 gEyePosW;
    
    DirectionalLightData gLightDataList[POSNORMALTEX_MAXIMUM_LIGHT_COUNT];
    int gDirLightListCount;
};

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    float4x4 gTexTransform;
    
    LegacyMaterialData gLegacyMaterial;
};

Texture2D gDiffuseMap; // Diffuse Map.
Texture2D gNormalMap; // Normal Map.

SamplerState samAnisotropic
{
	//Filter = ANISOTROPIC;
    Filter = MIN_MAG_MIP_LINEAR;

    MaxAnisotropy = 4;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

	// Transform to world space space.
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose);
    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld);
    
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

    // The toEye vector is used in lighting.
    float3 toEye = gEyePosW - pin.PosW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);

    // Normalize.
    toEye /= distToEye;

    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);
  
	// Sample texture.
    texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    
    //Normal Mapping!
    float3 normalMapSample = gNormalMap.Sample(samAnisotropic, pin.Tex).rgb;
    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
//
// Lighting.
//

    float4 litColor = texColor;
    int tLightCount = gDirLightListCount;
    
    if (tLightCount > 0)
    {
	    // Start with a sum of zero. 
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	    // Sum the light contribution from each light source.  
	    [unroll]
        //Maximum�� ���س��� ���̴�. ����μ��� POSNORMALTEX_MAXIMUM_LIGHT_COUNT ���� Maximum!
        tLightCount = min(tLightCount, POSNORMALTEX_MAXIMUM_LIGHT_COUNT);
            
        for (int i = 0; i < tLightCount; ++i)
        {
            float4 A, D, S;
            //�̹� CPU=>GPU�� �ε�Ǿ��� ��, �ݿ�������� Sort�Ǿ��� ���̴�.
            DirectionalLightData tDirLightData = gLightDataList[i];
            ComputeDirectionalLight_Legacy(gLegacyMaterial, tDirLightData, bumpedNormalW, toEye, A, D, S);

            ambient += A;
            diffuse += D;
            spec += S;
        }

	    // Modulate with late add.
        litColor = texColor * (ambient + diffuse) + spec;
    }

    // Common to take alpha from diffuse material and texture.
    litColor.a = gLegacyMaterial._diffuse.a * texColor.a;
    
    return litColor;
}

technique11 PosNormalTexTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}