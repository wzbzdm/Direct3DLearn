cbuffer CBuf
{
    matrix transform;
};

struct VertexIn
{
    float4 color : Color;
    float3 pos : POSITION;
};

struct VertexOut
{
    float4 color : Color;
    float4 pos : SV_POSITION;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;

    // 变换顶点位置
    vout.pos = mul(float4(vin.pos, 1.0), transform);

    // 传递颜色
    vout.color = vin.color;

    return vout;
}
