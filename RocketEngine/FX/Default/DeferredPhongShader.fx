#include "LightHelper.fx"
#include "../SceneLoadConstants/SceneInfoStructs.fx"

#define DEFERRED_PHONG_MAXIMUM_DIRECTIONAL_COUNT 10

//Deferred Phong : Basic Mesh���� �ش�.

// �� ���̴� ���� �ϴ��� ����� ���ѵǾ�� �Ѵ�.
// ���� ���� ���۵尡 ���̸�, �״�� ���� ���� Begin-End ���̾ ��������� �Ѵ�.
// ����� PHONG���� �������� ������ => PBR ���� �� ������ �����ϴ�, ������ ���̾ ����� ����.
// �ִ��� ���� �̸��� �����ϴ°� ���� ����! ���� ���� ����.

///DeferredPhongShader. 

RasterizerState DeferredCullingRS
{
    FillMode = Solid;
    CullMode = Back;
    FrontCounterClockwise = false; 
};

DepthStencilState DSS
{
    DepthEnable = true;
    //DepthWriteMask = Zero;
    DepthWriteMask = All;
    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;
    
    FrontFaceStencilFunc = Always;
    FrontFaceStencilPass = Incr;
    FrontFaceStencilFail = Keep;
    
    BackFaceStencilFunc = Always;
    BackFaceStencilPass = Incr;
    BackFaceStencilFail = Keep;
};

SamplerState gSamplerState;

SamplerState samAnisotropic
{
	//Filter = ANISOTROPIC;
    Filter = MIN_MAG_MIP_LINEAR;

    MaxAnisotropy = 4;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

///Deferred ��ü�� ���� ����.
Texture2D gTexDiffuseSRV_GBuffer;   // LightingPS ���.
Texture2D gTexNormalSRV_GBuffer;    // LightingPS ���.
Texture2D gTexPositionSRV_GBuffer;  // LightingPS ���.
Texture2D gTexDepthSRV_GBuffer;     // Depth�� LightingPS�� ���Ǵ� ���� �ƴ�, CombineRenderTarget�� �̿�ȴ�.

cbuffer cbPerFrame
{
    float3 gEyePosW;
    
    DirectionalLightData gLightDataList[DEFERRED_PHONG_MAXIMUM_DIRECTIONAL_COUNT];
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

///������Ʈ ������ ����Ǵ� Texture �ʵ�.
Texture2D gDiffuseMap; // Diffuse Map.
Texture2D gNormalMap;  // ���� Normal Map.

struct ActualVertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct ActualVertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
    
    //������ HPOS / Pos / Normal / Tex
};

struct QuadVertexIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct QuadVertexOut
{
    float4 hPos : HPOS;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelOutDeferred
{
    float4 diffuse : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
    float4 depth : SV_Target3; //Depth : ���߿� Depth Sorting�� ���� �ʿ��ϴ�.
};

//���������� ������ ��ģ��!
struct QuadPixelOut
{
    float4 color : SV_Target;
};


ActualVertexOut BasicVS(ActualVertexIn vin)
{
    ActualVertexOut vout;
    
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

PixelOutDeferred BasicPS(ActualVertexOut pin)
{
    PixelOutDeferred pOut;
    
    float4 texColor = float4(1, 1, 1, 1);
    texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    
    // Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);
    
    //Normal Mapping!
    float3 normalMapSample = gNormalMap.Sample(samAnisotropic, pin.Tex).rgb;
    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
    
    pOut.diffuse = texColor;                    // �ؽ��ĸ� �״�� ���ø�, Diffuse SVTarget���� ����.
    pOut.normal = float4(bumpedNormalW, 1.0f);  // NormalMap�� ������ bumpedNormalW�� ����.
    pOut.position = float4(pin.PosW, 1.0f);     // Position (World Position) ����, �� ���꿡 ���� �� ��.
    pOut.depth = pin.PosH.z / pin.PosH.w;       // 0-1 ���� ������ ����ȭ�� Depth.
    
    return pOut;
}

QuadVertexOut LightingVS(QuadVertexIn vIn)
{
    QuadVertexOut vOut;
    vOut.hPos = float4(vIn.position, 1.0f);
    vOut.position = vOut.hPos;
    vOut.tex = vIn.tex;

    return vOut;
}

QuadPixelOut LightingPS(QuadVertexOut pIn)
{
    //�ᱹ ���⿡�� ���� ó���ؾ� �Ѵٴ� ����̴�.
    //������ ������ ������ �����ϰ�, ������ ���İ��� ��� 0�� �Ǿ�� �Ѵ�.
    QuadPixelOut pOut;
    float4 diffuseAll = gTexDiffuseSRV_GBuffer.Sample(gSamplerState, pIn.tex);
    float4 normalAll = gTexNormalSRV_GBuffer.Sample(gSamplerState, pIn.tex);
    float4 posAll = gTexPositionSRV_GBuffer.Sample(gSamplerState, pIn.tex);
    
    float epsilon = 1.401298E-45f;
    
    ////������.
    //float4 depthAll = gTexDepthSRV_GBuffer.Sample(gSamplerState, pIn.tex);
    ////
    //
    ////������
    //epsilon += clamp(depthAll.r, 0, 0.0005f);
    ////
    
     //���� �ȼ��� ó���� Ư�� �� ���ϸ� ���� : => ���İ��� �������� �Ǵ�!
    if (diffuseAll.a <= epsilon)
    {
        clip(-1);
    }
    
     //float3 diffuse = diffuseAll.rgb;
    float3 bumpedNormalW = normalAll.rgb;
    float3 pos = posAll.rgb;
      // The toEye vector is used in lighting.
    float3 toEye = gEyePosW - pos;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);

    // Normalize.
    toEye /= distToEye;
    
    float4 litColor = diffuseAll;
    int tLightCount = gDirLightListCount;
    
    if (tLightCount > 0)
    {
	    // Start with a sum of zero. 
        float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	    // Sum the light contribution from each light source.  
        //Maximum�� ���س��� ���̴�. ����μ��� DEFERRED_PHONG_MAXIMUM_DIRECTIONAL_COUNT ���� Maximum!
	    [unroll]
        tLightCount = min(tLightCount, DEFERRED_PHONG_MAXIMUM_DIRECTIONAL_COUNT);
            
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
        litColor = diffuseAll * (ambient + diffuse) + spec;
    }
    
    // Common to take alpha from diffuse material and texture.
    litColor.a = gLegacyMaterial._diffuse.a * diffuseAll.a;
    
    pOut.color = litColor;
    
    return pOut;
}

technique11 BasicTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, BasicVS()));
        SetPixelShader(CompileShader(ps_5_0, BasicPS()));

        SetRasterizerState(DeferredCullingRS);
        SetDepthStencilState(DSS, 0);
    }
};

technique11 LightingTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, LightingVS()));
        SetPixelShader(CompileShader(ps_5_0, LightingPS()));
    }
};

//