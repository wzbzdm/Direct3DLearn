struct VSInput {
    float2 pos : Position;
    float4 color : Color;
};

struct PSInput {
    float4 pos : SV_Position;
    float4 color : Color;
};

PSInput main(VSInput input) {
    PSInput output;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.color = input.color;
    return output;
}