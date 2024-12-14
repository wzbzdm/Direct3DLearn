#include "Light.hlsli"

cbuffer CameraData : register(b0)
{
    CameraBuffer camera; // 摄像头相关数据
};

cbuffer TransformData : register(b1)
{
   TransformBuffer transform; // 变换数据
};

float3x3 SafeInverse3x3(float3x3 m)
{
    float3x3 adj =
    {
        cross(m[1], m[2]),
        cross(m[2], m[0]),
        cross(m[0], m[1])
    };
    float det = dot(m[0], adj[0]);
    return det != 0 ? adj / det : float3x3(1, 0, 0, 0, 1, 0, 0, 0, 1); // 返回单位矩阵作为默认值
}

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
    
    float3x3 normalMatrix = transpose(SafeInverse3x3((float3x3) transform.worldMatrix));
    vout.normal = normalize(mul(vin.normal, normalMatrix));

    // 传递顶点颜色（可以根据材质计算色值）
    vout.color = vin.color;
    
    vout.texCoord = vin.texCoord;

    return vout;
}
