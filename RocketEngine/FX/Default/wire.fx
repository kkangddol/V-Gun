//***************************************************************************************
// wire.fx by LeHide (C) 2021 All Rights Reserved.
//
// Axes of world, grid���� �׸��� ���� ���̾������ӿ� ���̴�
//***************************************************************************************

cbuffer cbPerObject
{
	float4x4 gWorldViewProj; 
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
    // �ٱ��ʿ��� ����-��-�������� ����� �޾Ƽ� ����������ǥ�� (���� NDC�ƴ�)���� ��ȯ���ش�.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    // ���� ������Ʈ�� ���ؽ��÷��� �����ؼ� �׸��� �Ѵ�.
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    pin.Color.r = 1.0f;
    pin.Color.g = 0.0f;
    pin.Color.b = 0.0f;

    return pin.Color;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
