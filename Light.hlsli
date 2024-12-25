// Light.hlsl

// 常量缓冲区结构体
struct CameraBuffer
{
    float4x4 viewMatrix; // 视图矩阵
    float4x4 projectionMatrix; // 投影矩阵
    float3 cameraPosition; // 摄像头位置
};

// 配置结构体
struct Config
{
    float2 textureWeight;   // 纹理权重
    float2 textureOff;      // 纹理偏移
    bool useTexture; // 是否使用纹理
};

// 光源结构体
struct Light
{
    float4 position; // 光源位置（对于定向光则使用0,0,0）
    float4 color; // 光源颜色
    float intensity; // 光强度
    float range; // 光源影响范围
};

// 材质属性结构体
struct Material
{
    float4 ambientColor; // 材质的环境光颜色
    float4 diffuseColor; // 材质的漫反射颜色
    float4 specularColor; // 材质的高光反射颜色
    float shininess; // 材质的光泽度
};

// 变换缓冲区
struct TransformBuffer
{
    float4x4 worldMatrix; // 世界矩阵
};

// 颜色缓冲区
struct ColorBuffer
{
    float4 color;  // 颜色
};

// 顶点数据结构体
struct VertexIn
{
    float3 position : POSITION; // 顶点位置
    float3 normal : NORMAL; // 顶点法线
    float2 texCoord : TEXCOORD0; // 顶点的纹理坐标
    uint index : INDEX; // 顶点颜色索引
};

// 输出结构体（传递到像素着色器）
struct VertexOut
{
    float4 position : SV_POSITION; // 转换后的屏幕空间位置
    float3 normal : NORMAL; // 传递法线给像素着色器
    uint index : INDEX; // 顶点颜色索引
    float4 worldPosition : TEXCOORD0; // 传递物体世界空间位置
    float2 texCoord : TEXCOORD2; // 传递纹理坐标
};
