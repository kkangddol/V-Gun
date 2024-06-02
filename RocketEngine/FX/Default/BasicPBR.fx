#include "BRDF.hlsli"
#include "LightHelper.fx"
#include "../SceneLoadConstants/SceneInfoStructs.fx"

/// [TW] 교수님 PBR 예제를 1차 수정!

#define DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT 3

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
   // BackFaceStencilPass = Decr;
    BackFaceStencilFail = Keep;
};


SamplerState gSamplerState;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;

    MaxAnisotropy = 4;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

///Deferred 전체에 대한 단위.
Texture2D<float4> gTexAlbedoSRV_GBuffer; // LightingPS_PBR 사용.
Texture2D<float4> gTexNormalSRV_GBuffer; // LightingPS_PBR 사용.
Texture2D<float4> gTexPositionSRV_GBuffer; // LightingPS_PBR 사용.
Texture2D<float4> gTexArmSRV_GBuffer; // LightingPS_PBR 사용.
Texture2D<float> gTexDepthSRV_GBuffer; // Depth는 LightingPS에 사용되는 것이 아닌, CombineRenderTarget에 이용된다.

cbuffer cbPerFrame
{
    float3 gEyePosW;
	
    DirectionalLightData gLightDataList[DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT];
    int gDirLightListCount;
};

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gTexTransform;
    float4x4 gWorldViewProj;
    
    PBRMaterialData gPBRMaterial;
}; 

Texture2D gAlbedoMap; //얘는 Base Color.
Texture2D gARMMap; // 수정, 이제 ARM을 순서대로 받는다. Ambient Occlusion / Roughness / Metallic.
Texture2D gNormalMap;

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

struct PixelOutPBRDeferred
{
	//일부 요소가 Float4가 아닌데.. 괜찮은가? 확인 필요.
	
    float4 albedo : SV_Target0; //Albedo.
    float4 normal : SV_Target1; //Normal.
    float4 position : SV_Target2; //Position.
    float3 arm : SV_Target3; //Ambient Occlusion / Roughness / Metallic.
    float depth : SV_Target4; //Depth (float)
	//Phong, PBR은 자료형이 Depth가 다르다. (simd)
};

//최종적으로 영향을 미친다!
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

PixelOutPBRDeferred BasicPS(ActualVertexOut pin)
{
    PixelOutPBRDeferred pOut;

    float4 texColor = float4(1, 1, 1, 1);
    texColor = gAlbedoMap.Sample(samAnisotropic, pin.Tex);

	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

	//Normal Mapping!
    float3 normalMapSample = gNormalMap.Sample(samAnisotropic, pin.Tex).rgb;
    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);

    pOut.albedo = texColor;
    pOut.normal = float4(bumpedNormalW, 1.0f); // NormalMap을 적용한 bumpedNormalW값 대입.
    pOut.position = float4(pin.PosW, 1.0f); // Position (World Position) 대입, 빛 연산에 쓰게 될 것.
    pOut.arm = gARMMap.Sample(samAnisotropic, pin.Tex).rgb;
    pOut.depth = pin.PosH.z / pin.PosH.w; // 0-1 사이 범위로 정규화된 Depth.
	
    return pOut;
}
 
QuadVertexOut LightingVS_PBR(QuadVertexIn vIn)
{
    QuadVertexOut vOut;
    vOut.hPos = float4(vIn.position, 1.0f);
    vOut.position = vOut.hPos;
    vOut.tex = vIn.tex;

    return vOut;
}

QuadPixelOut LightingPS_PBR(QuadVertexOut pin)
{
    QuadPixelOut pOut;
    
    // Default to multiplicative identity.
    float4 albedoAll = gTexAlbedoSRV_GBuffer.Sample(gSamplerState, pin.tex);
    float4 normalAll = gTexNormalSRV_GBuffer.Sample(gSamplerState, pin.tex);
    float4 posAll = gTexPositionSRV_GBuffer.Sample(gSamplerState, pin.tex);
    float3 armAll = gTexArmSRV_GBuffer.Sample(gSamplerState, pin.tex);
    
    float epsilon = 1.401298E-45f;
    //현재 픽셀의 처리를 특정 값 이하면 막기 : => 알파값을 기준으로 판단!
    if (albedoAll.a <= epsilon)
    {
        clip(-1);
    }
	
    float3 bumpedNormalW = normalAll.rgb;
    float3 pos = posAll.rgb;
      // The toEye vector is used in lighting.
    float3 toEye = gEyePosW - pos;
    float distToEye = length(toEye);

    // Normalize.
    toEye /= distToEye;
	
	//일단 Base Color 기준으로 다른 연산들이 이루어지게.
    float4 litColor = albedoAll;
    int tLightCount = gDirLightListCount;
	
	// 광원 연산 => BRDF를 계산.

    if (tLightCount > 0)
    {
		// N : 월드 좌표계의 노말
        float3 N = normalAll;
        
		// V : 시선 벡터
		// The toEye vector is used in lighting.
        float3 V = toEye;

		// 러프니스, 메탈릭, 앰비언트오클루전
        float tAmbientOcclusion = armAll.r;
        float tRoughness = armAll.g;
        float tMetallic = armAll.b;
        
        //실제 Material 값 반영.
        //gPBRMaterial 중, Specular, Emission은 1차에서 영향을 미치지 않을 것!
        albedoAll = gPBRMaterial._albedo * albedoAll;
        tRoughness = gPBRMaterial._roughness * tRoughness;
        tMetallic = gPBRMaterial._metallic * tMetallic;
        
        //계산 상의 한계로 개수 한계로 계산.
        tLightCount = min(tLightCount, DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT);
        
        //계산 하기 전에, 값을 정리해야 한다.
        float3 tLightColorArray[DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT];
        float3 tLightDirectionArray[DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT];
        [unroll]
        for (int i = 0; i < tLightCount; i++)
        {
            float4 tLightCol = gLightDataList[i]._diffuse;
            tLightColorArray[i] = float3(tLightCol.x, tLightCol.y, tLightCol.z);
            // L : 빛이 들어오는 벡터 (를 뒤집은 것)
            //이거 이상하면, 나중에 거꾸로 넣어도 된다.
            // 하지만, Reflect에서 이를 의미 뒤집기에 괜찮을지도?
            tLightDirectionArray[i] = normalize(gLightDataList[i]._direction);
        }    
        //여러 개의 라이트를 합치기.
        //float3 color = LightSurface1(V, N, f3White, L, albedoAll.rgb, tRoughness, tMetallic, tAmbientOcclusion);
        float3 color = LightSurface(V, N, tLightCount, 
        tLightColorArray, tLightDirectionArray, 
        albedoAll.rgb, tRoughness, tMetallic, tAmbientOcclusion);
            
        ////[TW] PBR이 너무 어둡다! 강제로 밝기를 올려볼까?
        //color.r = clamp(color.r + 0.2f, 0.0f, 1.0f);
        //color.g = clamp(color.g + 0.2f, 0.0f, 1.0f);
        //color.b = clamp(color.b + 0.2f, 0.0f, 1.0f);
        
        //litColor = float4(color, albedoAll.w);
        //[TW] 강제로 투명해보이지 않게 하기 위해 FIX.
        litColor = float4(color, 1.0f);
        
        //[TW] Gamma Correction 반영.
        float gamma = 2.2f;
        litColor.rgb = pow(litColor.rgb, float3(1.0f / gamma, 1.0f / gamma, 1.0f / gamma));
    }
    pOut.color = litColor;
    return pOut;
}

// 라이팅 연산 전, 값 변형.
technique11 BasicTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, BasicVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, BasicPS()));

        SetRasterizerState(DeferredCullingRS);
        SetDepthStencilState(DSS, 0);
       // SetBlendState(StopBackgroundBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
}

// Quad에 적용 + 실제 라이팅.
technique11 LightingTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, LightingVS_PBR()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, LightingPS_PBR()));
    }
}

