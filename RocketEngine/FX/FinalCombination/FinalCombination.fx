
SamplerState gSamplerState;

//Depth값 판별을 하기 위해 새로운 값들을 가져와야 한다. (NearClip, FarClip)
float gNearFlip;
float gFarFlip;

//개별적인 렌더된 상태를 합친다. 둘다 이미 계산된 상태.
Texture2D gForward_QuadBuffer;
Texture2D<float> gForward_DepthBuffer; //이 경우, m_pDepthStencilView가 들어가면 된다.

Texture2D gDeferredPhong_QuadBuffer;
Texture2D<float> gDeferredPhong_DepthBuffer; //이 중에서 r값만 쓰면 된다.

Texture2D gDeferredPBR_QuadBuffer;
Texture2D<float> gDeferredPBR_DepthBuffer; //자체 사용

Texture2D gDeferredPBR_Skinned_QuadBuffer;
Texture2D<float> gDeferredPBR_Skinned_DepthBuffer; //자체 사용

//Struct : 값 소팅을 함에 있어서 일괄적으로 처리하기 위해. 
struct DepCol
{
    float4 _color;
    float _depth;
};

//4개의 배열을 반환하는 것은 똑같지만, Alpha값이 0이면 무시하고 사용할 수 있어야 한다.
void ReturnColorDepthArrayAlphaNonZero(in float4 colors[4], in float depth[4], out int size, out DepCol depColArray[4])
{
    float epsilon = 1.401298E-45f;
    
    //실제 반영할 value들만 선별한다. (Alpha 값이 0 아니면 배열에 추가)
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

//실제 어디까지 반환되는지 점검.
float FindMinimumValueDepthArray(float depth[4])
{
    //Depth는 0-1 범위를 넘어가지 않는다!
    float minValue = 100.0f; // 큰 값으로 초기화.

    // 평행하게 실행되는 코드를 줄여, Branching을 막아준다고 한다.
    // 2의 배수여야 작동되는 것..?
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
    
    // 가장 작은 값은 이제 인덱스0에 있다.
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
    //차례대로 Forward / Deferred Phong / Deferred PBR / Deferred PBR Skinned.
    
    PixelOutBasic pOut;
    float4 colorArray[4];
    colorArray[0] = gForward_QuadBuffer.Sample(gSamplerState, pIn.tex); //Color가 들어있을 것.
    colorArray[1] = gDeferredPhong_QuadBuffer.Sample(gSamplerState, pIn.tex); //Color가 들어있을 것.
    colorArray[2] = gDeferredPBR_QuadBuffer.Sample(gSamplerState, pIn.tex); //Color가 들어있을 것.
    colorArray[3] = gDeferredPBR_Skinned_QuadBuffer.Sample(gSamplerState, pIn.tex); //Color가 들어있을 것.
    
    //답 : Forward를 남기고, Deferred 끼리의 Depth Sort. 만약 싹다 0 이하라면, Forward Render. 
    float epsilon = 1.401298E-45f;
    
    //셰이더에 Branching이라.. 마음이 아프지만... 게임은 나와야 하니!
    [branch]
    if (colorArray[1].a > epsilon || colorArray[2].a > epsilon || colorArray[3].a > epsilon)
    {
        //Depth Buffer 옮겨 담기.
        float depthArray[4];
        depthArray[0] = gForward_DepthBuffer.Sample(gSamplerState, pIn.tex); //Color가 들어있을 것.
        depthArray[1] = gDeferredPhong_DepthBuffer.Sample(gSamplerState, pIn.tex); //Color가 들어있을 것. 얘는 여기에서 float4 중 r만 꺼내 써야.
        depthArray[2] = gDeferredPBR_DepthBuffer.Sample(gSamplerState, pIn.tex); //Color가 들어있을 것.
        depthArray[3] = gDeferredPBR_Skinned_DepthBuffer.Sample(gSamplerState, pIn.tex); //Color가 들어있을 것.
 
        ///없애보기.
        //depthArray[0] = GetNormalizedDepth(depthArray[0], gNearFlip, gFarFlip);
        //depthArray[1] = GetNormalizedDepth(depthArray[1], gNearFlip, gFarFlip);
        //depthArray[2] = GetNormalizedDepth(depthArray[2], gNearFlip, gFarFlip);
        //depthArray[3] = GetNormalizedDepth(depthArray[3], gNearFlip, gFarFlip);
        
        depthArray[0] = 0.0f; 
        //강제로 Forward가 보이지 않게 세팅하기. 이 Branch의 Depth Sorting은 그냥 Forward가 보이지 않게 할 것이다!
        
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
        pOut.color = colorArray[0]; //디퍼드가 싹 다 해당 안될시, Forward를 무조건적으로 출력.
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