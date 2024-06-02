//[TW] FrankLuna�� Sky.fx ����, ť��� Ȱ���� ����.

cbuffer cbPerFrame
{
    float4x4 gWorldViewProj;
};

//ť��� �ؽ���
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
    
    //W Divide�� 1�� �Ǿ�, �ϴ� ���� �׻� �� ��鿡 ����. ( Z = W )
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;
    //Cubemap ��ȸ ����.
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
    // ����ȭ�� ���� �� �������� ����, LESS�� �ƴ� LESS_EQUAL ���.
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

