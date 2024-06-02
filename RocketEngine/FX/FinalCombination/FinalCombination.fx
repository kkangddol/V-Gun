
SamplerState gSamplerState;

//Depth�� �Ǻ��� �ϱ� ���� ���ο� ������ �����;� �Ѵ�. (NearClip, FarClip)
float gNearFlip;
float gFarFlip;

//�������� ������ ���¸� ��ģ��. �Ѵ� �̹� ���� ����.
Texture2D gForward_QuadBuffer;
Texture2D<float> gForward_DepthBuffer; //�� ���, m_pDepthStencilView�� ���� �ȴ�.

Texture2D gDeferredPhong_QuadBuffer;
Texture2D<float> gDeferredPhong_DepthBuffer; //�� �߿��� r���� ���� �ȴ�.

Texture2D gDeferredPBR_QuadBuffer;
Texture2D<float> gDeferredPBR_DepthBuffer; //��ü ���

Texture2D gDeferredPBR_Skinned_QuadBuffer;
Texture2D<float> gDeferredPBR_Skinned_DepthBuffer; //��ü ���

//Struct : �� ������ �Կ� �־ �ϰ������� ó���ϱ� ����. 
struct DepCol
{
    float4 _color;
    float _depth;
};

//4���� �迭�� ��ȯ�ϴ� ���� �Ȱ�����, Alpha���� 0�̸� �����ϰ� ����� �� �־�� �Ѵ�.
void ReturnColorDepthArrayAlphaNonZero(in float4 colors[4], in float depth[4], out int size, out DepCol depColArray[4])
{
    float epsilon = 1.401298E-45f;
    
    //���� �ݿ��� value�鸸 �����Ѵ�. (Alpha ���� 0 �ƴϸ� �迭�� �߰�)
    int tNowIndex = 0;
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        [flatten]
        if (colors[i].a > epsilon)
        {
            depColArray[tNowIndex]._color = colors[i];
            depColArray[tNowIndex]._depth = depth[i];
            tNowIndex++;
        }
    }
    size = tNowIndex;
}

//���� ������ ��ȯ�Ǵ��� ����.
float FindMinimumValueDepthArray(float depth[4])
{
    //Depth�� 0-1 ������ �Ѿ�� �ʴ´�!
    float minValue = 100.0f; // ū ������ �ʱ�ȭ.

    // �����ϰ� ����Ǵ� �ڵ带 �ٿ�, Branching�� �����شٰ� �Ѵ�.
    // 2�� ������� �۵��Ǵ� ��..?
    int ARRAY_SIZE = 4;
   
    [unroll]
    for (uint stride = ARRAY_SIZE / 2; stride > 0; stride /= 2)
    {
        [unroll]
        for (uint i = 0; i < stride; ++i)
        {
            float value1 = depth[i];
            float value2 = depth[i + stride];
            depth[i] = min(value1, value2);
        }
    }
    
    // ���� ���� ���� ���� �ε���0�� �ִ�.
    return depth[0];
}

float GetNormalizedDepth(in float depthVal, in float nearZ, in float farZ)
{
    float normalizedDepth = (2.0 * nearZ) / (farZ + nearZ - depthVal * (farZ - nearZ));
    return normalizedDepth;
}

struct VertexIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct VertexOut
{
    float4 hPos : HPOS;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelOutBasic
{
    float4 color : SV_Target;
};

VertexOut CombineVS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.hPos = float4(vIn.position, 1.0f);
    vOut.position = vOut.hPos;
    vOut.tex = vIn.tex;

    return vOut;
}

PixelOutBasic CombinePS(VertexOut pIn)
{
    //���ʴ�� Forward / Deferred Phong / Deferred PBR / Deferred PBR Skinned.
    
    PixelOutBasic pOut;
    float4 colorArray[4];
    colorArray[0] = gForward_QuadBuffer.Sample(gSamplerState, pIn.tex); //Color�� ������� ��.
    colorArray[1] = gDeferredPhong_QuadBuffer.Sample(gSamplerState, pIn.tex); //Color�� ������� ��.
    colorArray[2] = gDeferredPBR_QuadBuffer.Sample(gSamplerState, pIn.tex); //Color�� ������� ��.
    colorArray[3] = gDeferredPBR_Skinned_QuadBuffer.Sample(gSamplerState, pIn.tex); //Color�� ������� ��.
    
    //�� : Forward�� �����, Deferred ������ Depth Sort. ���� �ϴ� 0 ���϶��, Forward Render. 
    float epsilon = 1.401298E-45f;
    
    //���̴��� Branching�̶�.. ������ ��������... ������ ���;� �ϴ�!
    [branch]
    if (colorArray[1].a > epsilon || colorArray[2].a > epsilon || colorArray[3].a > epsilon)
    {
        //Depth Buffer �Ű� ���.
        float depthArray[4];
        depthArray[0] = gForward_DepthBuffer.Sample(gSamplerState, pIn.tex); //Color�� ������� ��.
        depthArray[1] = gDeferredPhong_DepthBuffer.Sample(gSamplerState, pIn.tex); //Color�� ������� ��. ��� ���⿡�� float4 �� r�� ���� ���.
        depthArray[2] = gDeferredPBR_DepthBuffer.Sample(gSamplerState, pIn.tex); //Color�� ������� ��.
        depthArray[3] = gDeferredPBR_Skinned_DepthBuffer.Sample(gSamplerState, pIn.tex); //Color�� ������� ��.
 
        ///���ֺ���.
        //depthArray[0] = GetNormalizedDepth(depthArray[0], gNearFlip, gFarFlip);
        //depthArray[1] = GetNormalizedDepth(depthArray[1], gNearFlip, gFarFlip);
        //depthArray[2] = GetNormalizedDepth(depthArray[2], gNearFlip, gFarFlip);
        //depthArray[3] = GetNormalizedDepth(depthArray[3], gNearFlip, gFarFlip);
        
        depthArray[0] = 0.0f; 
        //������ Forward�� ������ �ʰ� �����ϱ�. �� Branch�� Depth Sorting�� �׳� Forward�� ������ �ʰ� �� ���̴�!
        
        int sortedSize;
        DepCol sortedDepCol[4];
        ReturnColorDepthArrayAlphaNonZero(colorArray, depthArray, sortedSize, sortedDepCol);
       
        //DepCol minDepth;
        //minDepth._color = float4(1.0f, 1.0f, 1.0f, 1.0f);
        //minDepth._depth = 1000.0f;
        
        DepCol maxDepth;
        maxDepth._color = float4(1.0f, 1.0f, 1.0f, 1.0f);
        maxDepth._depth = -1000.0f;
        
        [unroll]
        for (int i = 0; i < sortedSize; i++)
        {
            [flatten]
            if (sortedDepCol[i]._depth > maxDepth._depth)
            {
                maxDepth._color = sortedDepCol[i]._color;
                maxDepth._depth = sortedDepCol[i]._depth;
            }
        }
        
        pOut.color = maxDepth._color;
        return pOut;
    }
    else
    {
        pOut.color = colorArray[0]; //���۵尡 �� �� �ش� �ȵɽ�, Forward�� ������������ ���.
        return pOut;
    }

}

technique11 CombineTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, CombineVS()));
        SetPixelShader(CompileShader(ps_5_0, CombinePS()));
    }
};