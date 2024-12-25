#include "Light.hlsli"

cbuffer CameraData : register(b0)
{
    CameraBuffer camera; // 摄像头相关数据
};

cbuffer AllConfig : register(b1)
{
    Config config; // 配置数据
};

cbuffer TransformData : register(b2)
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
    vout.position = mul(vout.worldPosition, camera.viewMatrix);
    
    // 视图空间转屏幕空间：乘以摄像机的投影矩阵
    vout.position = mul(vout.position, camera.projectionMatrix);
    
    vout.normal = normalize(mul(vin.normal, (float3x3) transform.worldMatrix));

    // 传递顶点颜色（可以根据材质计算色值）
    vout.index = vin.index;
    
    vout.texCoord = vin.texCoord;

    return vout;
}
