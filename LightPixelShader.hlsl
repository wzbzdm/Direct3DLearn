#include "Light.hlsli"

cbuffer CameraData : register(b0)
{
    CameraBuffer camera; // 摄像头相关数据
};

cbuffer AllConfig : register(b1)
{
    Config config; // 配置数据
};

cbuffer LightData : register(b2)
{
    Light lights[10]; // 光源数组
};

cbuffer MaterialData : register(b3)
{
    Material material; // 材质数据
};

cbuffer ColorData : register(b4)
{
    ColorBuffer colors[20];  // 颜色数组
};

Texture2D diffuseTexture : register(t0); // 漫反射纹理
SamplerState defaultSampler : register(s0);

float4 main(VertexOut pin) : SV_Target
{
    float3 viewDir = normalize(camera.cameraPosition - pin.worldPosition.xyz); // 观察方向
    
    // 环境光
    float4 envLight = float4(0.1, 0.1, 0.1, 1.0);
    
    // Phong 光照计算：环境光、漫反射光和高光反射
    float4 finalColor = material.ambientColor * envLight; // 环境光
    
    // 漫反射与高光反射计算
    float3 lightColor = float3(0, 0, 0);
    for (int i = 0; i < 10; i++)
    {
        if (lights[i].intensity > 0)
        {
            // 计算光源方向与法线的点积，得到漫反射
            float3 lightDir = normalize(lights[i].position.xyz - pin.worldPosition.xyz);
            float diff = max(dot(pin.normal, lightDir), 0.0f);
            // float diff = saturate(dot(pin.normal, lightDir)); // 或者使用更柔和的计算方式
            
            // 计算高光：光的反射方向和观察方向的点积
            float3 reflectDir = reflect(-lightDir, pin.normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

            // 计算点光源对当前像素的影响
            float dist = length(lights[i].position.xyz - pin.worldPosition.xyz);
            float attenuation = 1.0f / (1.0f + 0.05f * dist + 0.005f * dist * dist);
            
            float3 lightIntensity = lights[i].color.rgb * (lights[i].intensity * attenuation);

            // 累加光照效果
            lightColor += (lightIntensity * (diff + 0.5f * spec) * material.specularColor.rgb);
        }
    }
    
    pin.texCoord.x *= config.textureWeight.x; // 缩放 x 分量
    pin.texCoord.y *= config.textureWeight.y; // 缩放 y 分量
    pin.texCoord += config.textureOff; // 偏移纹理坐标

    float4 texColor = diffuseTexture.Sample(defaultSampler, pin.texCoord);
    
    if (!config.useTexture)
    {
        texColor = colors[pin.index].color;
    }
    
    finalColor.rgb = texColor.rgb * (finalColor.rgb + lightColor); // 独立光照影响
    finalColor.a = texColor.a * material.diffuseColor.a;
    
    // Gamma 校正
    finalColor.rgb = pow(finalColor.rgb, 1.0f / 2.2f);
    return finalColor;
}
