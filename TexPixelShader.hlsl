SamplerState samTex
{
    Filter = MIN_MAG_MIP_LINEAR;
};

Texture2D texFront : register(t0); // 前面的纹理
Texture2D texBack : register(t1); // 后面的纹理
Texture2D texTop : register(t2); // 上面的纹理
Texture2D texBottom : register(t3); // 下面的纹理
Texture2D texLeft : register(t4); // 左面的纹理
Texture2D texRight : register(t5); // 右面的纹理

struct PSInput
{
    float4 pos : SV_POSITION; // 屏幕空间的顶点位置
    float2 tex : TEXCOORD; // 纹理坐标传递给像素着色器
    uint index : INDEX;
};

float4 main(PSInput pin) : SV_Target
{
    switch (pin.index)
    {
        case 0:
            return texFront.Sample(samTex, pin.tex); // 从前面的纹理采样
        case 1:
            return texBack.Sample(samTex, pin.tex); // 从后面的纹理采样
        case 2:
            return texTop.Sample(samTex, pin.tex); // 从左面的纹理采样
        case 3:
            return texBottom.Sample(samTex, pin.tex); // 从右面的纹理采样
        case 4:
            return texLeft.Sample(samTex, pin.tex); // 从上面的纹理采样
        case 5:
            return texRight.Sample(samTex, pin.tex); // 从下面的纹理采样
        default:
            return texFront.Sample(samTex, pin.tex); // 给一个默认返回，避免遗漏情况
    }
}