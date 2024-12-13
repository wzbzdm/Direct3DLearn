#include "Light.hlsli"

cbuffer CameraData : register(b0)
{
    CameraBuffer camera; // 摄像头相关数据
};

cbuffer LightData : register(b1)
{
    Light lights[10]; // 光源数组（最多支持10个光源）
};

cbuffer MaterialData : register(b2)
{
    Material material; // 材质数据
};

cbuffer TransformData : register(b3)
{
   TransformBuffer transform; // 变换数据
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;

    // 物体的位置从局部空间转到世界空间
    float4 worldPos = float4(vin.position, 1.0f); // 物体局部位置
    vout.worldPosition = mul(worldPos, transform.worldMatrix);

    // 世界空间转视图空间：乘以摄像机的视图矩阵
    vout.position = mul(worldPos, camera.viewMatrix);
    
    // 视图空间转屏幕空间：乘以摄像机的投影矩阵
    vout.position = mul(vout.position, camera.projectionMatrix);

    // 传递法线给像素着色器
    vout.normal = mul(vin.normal, (float3x3) camera.viewMatrix); // 法线转到视图空间

    // 传递顶点颜色（可以根据材质计算色值）
    vout.color = vin.color * material.ambientColor;

    return vout;
}
