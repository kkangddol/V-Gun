///SkinnedMeshTex : For Skinning! (Default, Legacy)

#include "LightHelper.fx"

cbuffer cbPerFrame
{
    float4x4 bones[100];
    float3 gEyePosW;
};

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    float4x4 gTexTransform;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    uint nodeIndex0 : BLENDINDICES0;
    uint nodeIndex1 : BLENDINDICES1;
    uint nodeIndex2 : BLENDINDICES2;
    uint nodeIndex3 : BLENDINDICES3;
    float node0Weight : BLENDWEIGHT0;
    float node1Weight : BLENDWEIGHT1;
    float node2Weight : BLENDWEIGHT2;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    //이거 되어야 한다.
    
    float _weights[4] = { 0.f, 0.f, 0.f, 0.f };
    _weights[0] = vin.node0Weight;
    _weights[1] = vin.node1Weight;
    _weights[2] = vin.node2Weight;
    _weights[3] = 1 - (vin.node0Weight + vin.node1Weight + vin.node2Weight);

    float3 posL = float3(0.f, 0.f, 0.f);
    float3 normalL = float3(0.f, 0.f, 0.f);
	
    posL += _weights[0] * mul(float4(vin.PosL, 1.f), bones[vin.nodeIndex0]).xyz;
    normalL += _weights[0] * mul(vin.NormalL, (float3x3) bones[vin.nodeIndex0]);
    
    posL += _weights[1] * mul(float4(vin.PosL, 1.f), bones[vin.nodeIndex1]).xyz;
    normalL += _weights[1] * mul(vin.NormalL, (float3x3) bones[vin.nodeIndex1]);
    
    posL += _weights[2] * mul(float4(vin.PosL, 1.f), bones[vin.nodeIndex2]).xyz;
    normalL += _weights[2] * mul(vin.NormalL, (float3x3) bones[vin.nodeIndex2]);
    
    posL += _weights[3] * mul(float4(vin.PosL, 1.f), bones[vin.nodeIndex3]).xyz;
    normalL += _weights[3] * mul(vin.NormalL, (float3x3) bones[vin.nodeIndex3]);
    
	
    vout.PosW = mul(float4(posL, 1.f), gWorld).xyz;
    vout.NormalW = mul(normalL, (float3x3) gWorldInvTranspose);
		//vout.NormalW = vin.NormalL;

    vout.PosH = mul(float4(posL, 1.f), gWorldViewProj);

		//vout.Tex = mul(float4(vin.Tex, 0.f, 1.f), gTexTransform).xy;
    vout.Tex = vin.Tex;
    
    return vout;
}

float4 PS(VertexOut pin, uniform bool gUseTexure) : SV_Target
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

    if (gUseTexure)
    {
    // Sample texture.
        texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    }

    float4 litColor = texColor;
    
    // Common to take alpha from diffuse material and texture.
    litColor.a = texColor.a;
    
    return litColor;
}

technique11 Light0Tex
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(true)));
    }
}

