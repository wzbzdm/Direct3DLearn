struct VSInput
{
    float3 pos : POSITION; // 顶点位置
    float2 tex : TEXCOORD; // 顶点纹理坐标
    uint index : INDEX;
};

struct PSInput
{
    float4 pos : SV_POSITION; // 屏幕空间的顶点位置
    float2 tex : TEXCOORD; // 纹理坐标传递给像素着色器
    uint index : INDEX;
};

cbuffer TransformBuffer : register(b0)
{
    matrix modelViewProjection; // 模型-视图-投影矩阵
};

PSInput main(VSInput input)
{
    PSInput output;

    // 将顶点位置与视图投影矩阵相乘
    output.pos = mul(float4(input.pos, 1.0f), modelViewProjection);
    output.tex = input.tex; // 保持纹理坐标
    output.index = input.index;

    return output;
}
