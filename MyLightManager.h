#pragma once

#include <DirectXMath.h>
#include <vector>
#include <algorithm>
#include <iostream>

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
            DirectX::XMFLOAT4(0.0f, 10.0f, -10.0f, 1.0f),    // 点光源位置 (x, y, z)，例如放在场景中的(0, 10, 0)高度位置
            DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),   // 点光源方向，点光源不需要方向，通常用零向量 (0, 0, 0)
            DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    // 点光源的颜色，白色光，RGB=(1, 1, 1) 
            1.0f,                                         // 光强度，1.0表示正常强度
            50.0f,                                        // 光源范围，这里可以设定为 20，表示光可以照亮的范围
            0.0f,                                         // 聚光灯锥度为 0，因为这是点光源
            0.0f,                                         // 额外设置，未使用，通常是 0
            0,                                            // 光源类型为点光源 (0)，也可选择聚光灯 (1) 或方向光 (2)
            1                                             // 启用光源标志 (1: 启用，0: 禁用)
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

    std::vector<LightProperties> GetLightsData() {
        return lights;
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
        std::vector<LightBuffer> lightBufferData(10);
        for (int i = 0; i < std::min(10, (int)lights.size()); i++) {
            LightBuffer buffer;
            buffer.position = lights[i].position;
            buffer.color = lights[i].color;
            buffer.intensity = lights[i].intensity;
            buffer.range = lights[i].range;
            lightBufferData[i] = buffer;
        }
        return lightBufferData;
    }

    // 添加光源
    void AddLight(const LightProperties& light) {
        lights.push_back(light);  // 添加到灯光列表
    }

    void AddLight() {
        lights.push_back(defaultLight);
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
