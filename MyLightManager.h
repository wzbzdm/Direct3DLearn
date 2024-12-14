#pragma once

#include <DirectXMath.h>
#include <vector>

// 灯光属性数据结构
struct LightProperties {
    DirectX::XMFLOAT4 position;  // 光源位置（如果是定向光，位置可以是(0, 0, 0)）
    DirectX::XMFLOAT4 direction; // 光源方向（例如定向光的方向）
    DirectX::XMFLOAT4 color;     // 光的颜色
    float intensity;             // 光强
    float range;                 // 灯光影响范围（如点光源）
	float spotFalloff;           // 聚光灯光锥的衰减
	float spotPower;             // 聚光灯光锥的锥度
	int type;                    // 灯光类型（0：点光源，1：定向光，2：聚光灯）
	int enabled;                 // 是否启用
};

typedef struct {
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT4 color;
    float intensity;
    float range;
    float pad[2];
} LightBuffer;

// 完善的 LightManager 管理类
class LightManager {
public:
    LightManager() {
        // 初始化一个默认的点光源
        defaultLight = {
            DirectX::XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f),    // 设置点光源位置（假设为 (0, 10, 0)）
            DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f),   // 点光源的方向，可以设为(0, -1, 0)，代表光向下照射
            DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.1f),    // 设置光的颜色为白色（RGB 都为 1）
            1.0f,                                         // 光强度
            50.0f,                                        // 灯光范围
            0.0f,                                         // 聚光灯锥衰减设置为 0.0f，因为这里是点光源
            0.0f,                                         // 聚光灯的锥度设置为 0.0f，因为这里没有聚光灯
            0,                                            // 设置为点光源（点光源类型是 0）
            1                                             // 启用光源
        };

        lights.push_back(defaultLight);
    }

    // 更新灯光数据
    void UpdateLights(const std::vector<LightProperties>& newLights) {
        lights = newLights;  // 更新灯光列表
    }

    // 获取所有灯光数据
    const std::vector<LightProperties>& GetLights() const noexcept {
        return lights;
    }

    // 获取某个灯光数据
    const LightProperties& GetLight(unsigned int index) const noexcept {
        return lights.at(index);  // 安全地访问灯光列表
    }

    // 更新某一个灯光的数据
    void UpdateLight(const LightProperties& light, unsigned int index) {
        lights.at(index) = light;  // 更新灯光列表中的数据
    }

    // 为所有灯光计算与视图相关的矩阵
    void TransformLightsWithViewMatrix(const DirectX::XMMATRIX& viewMatrix) {
        using namespace DirectX;

        for (auto& light : lights) {
            // 转换位置
            XMVECTOR lightPos = XMLoadFloat4(&light.position);
            lightPos = XMVector3Transform(lightPos, viewMatrix);
            XMStoreFloat4(&light.position, lightPos);

            // 转换方向（定向光，使用方向向量）
            XMVECTOR lightDir = XMLoadFloat4(&light.direction);
            lightDir = XMVector3TransformNormal(lightDir, viewMatrix); // 用 TransformNormal 而不是 Transform
            XMStoreFloat4(&light.direction, lightDir);
        }
    }

    std::vector<LightBuffer> GetLightBufferData() {
        std::vector<LightBuffer> lightBufferData;
        for (const auto& light : lights) {
            LightBuffer buffer;
            buffer.position = light.position;
            buffer.color = light.color;
            buffer.intensity = light.intensity;
            buffer.range = light.range;
            lightBufferData.push_back(buffer);
        }
        return lightBufferData;
    }

    // 添加光源
    void AddLight(const LightProperties& light) {
        lights.push_back(light);  // 添加到灯光列表
    }

    // 删除指定索引的光源
    void RemoveLight(unsigned int index) {
        if (index < lights.size()) {
            lights.erase(lights.begin() + index);
        }
    }

private:
    LightProperties defaultLight;
    std::vector<LightProperties> lights;  // 保存所有活动的灯光
};
