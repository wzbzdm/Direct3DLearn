// PixelShader.hlsl
#include "Light.hlsli"

cbuffer CameraData : register(b0)
{
    CameraBuffer camera; // 摄像头相关数据
};

cbuffer LightData : register(b1)
{
    Light lights[10]; // 光源数组
};

cbuffer MaterialData : register(b2)
{
    Material material; // 材质数据
};

cbuffer TransformData : register(b3)
{
    TransformBuffer transform; // 变换数据
};

Texture2D diffuseTexture : register(t0); // 漫反射纹理
SamplerState sampleState : register(s0); // 纹理采样器

float4 main(VertexOut pin) : SV_Target
{
    float3 viewDir = normalize(camera.cameraPosition - pin.worldPosition.xyz); // 观察方向
    
    // Phong 光照计算：环境光、漫反射光和高光反射
    float4 finalColor = material.ambientColor * pin.color; // 环境光
    
    // 漫反射与高光反射计算
    float3 lightColor = float3(0, 0, 0);
    for (int i = 0; i < 10; i++)
    {
        if (lights[i].intensity > 0)
        {
            // 计算光源方向与法线的点积，得到漫反射
            float3 lightDir = normalize(lights[i].position.xyz - pin.worldPosition.xyz);
            float diff = max(dot(pin.normal, lightDir), 0.0f);
            
            // 计算高光：光的反射方向和观察方向的点积
            float3 reflectDir = reflect(-lightDir, pin.normal);
            // float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

            // 计算点光源对当前像素的影响
            float dist = length(lights[i].position.xyz - pin.worldPosition.xyz);
            float attenuation = 1.0f / (1.0f + 0.1f * dist + 0.01f * dist * dist);
            float3 lightIntensity = lights[i].color.rgb * (lights[i].intensity * attenuation);

            // 累加光照效果
            lightColor += (lightIntensity * diff + spec * material.specularColor.rgb);
        }
    }

    float4 texColor = diffuseTexture.Sample(sampleState, pin.texCoord);
    finalColor *= texColor;
    finalColor.rgb += lightColor; // 加上光照效应
    finalColor.a = texColor.a * material.diffuseColor.a;
    
    return finalColor;
}
