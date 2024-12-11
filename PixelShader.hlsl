struct PSInput {
    float4 pos : SV_Position;
    float4 color : Color;
};

float4 main(PSInput input) : SV_Target{
    return input.color;
}