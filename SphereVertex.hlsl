cbuffer CBuf
{
    matrix transform;
};

struct VSInput
{
    float3 pos : POSITION;
    int colorIndex : COLORINDEX;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    int colorIndex : COLORINDEX;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = mul(float4(input.pos, 1.0f), transform);
    output.colorIndex = input.colorIndex; // 将颜色索引传递给像素着色器
    return output;
}
