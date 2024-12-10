struct VSInput {
    float2 pos : Position;
    float4 color : Color;
};

struct PSInput {
    float4 pos : SV_Position;
    float4 color : Color;
};

cbuffer CBuf{
    row_major matrix transform;
};

PSInput main(VSInput input) {
    PSInput output;
    output.pos = mul(float4(input.pos, 0.0f, 1.0f), transform);
    output.color = input.color;
    return output;
}