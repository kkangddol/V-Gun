//[TW] FrankLuna의 Sky.fx 개조, 큐브맵 활용을 위해.

cbuffer cbPerFrame
{
    float4x4 gWorldViewProj;
};

//큐브맵 텍스쳐
TextureCube gCubeMap;

SamplerState samTriLinearSam
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VertexIn
{
    float3 PosL : POSITION;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    //W Divide가 1이 되어, 하늘 돔이 항상 먼 평면에 있음. ( Z = W )
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;
    //Cubemap 조회 벡터.
    vout.PosL = vin.PosL;
   
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return gCubeMap.Sample(samTriLinearSam, pin.PosL);
}

RasterizerState NoCull
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
    // 정규화된 깊이 값 판정까지 위해, LESS가 아닌 LESS_EQUAL 사용.
    DepthFunc = LESS_EQUAL;
};

technique11 SkyTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));

        SetRasterizerState(NoCull);
        SetDepthStencilState(LessEqualDSS, 0);
    }
}

