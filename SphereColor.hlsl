cbuffer CBuf
{
    float4 face_colors[3];
};

struct PSInput
{
    float4 pos : SV_POSITION;
    int colorIndex : COLORINDEX;
};

float4 main(PSInput input) : SV_Target
{
    return face_colors[input.colorIndex]; // 使用颜色索引选择颜色
}
