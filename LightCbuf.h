#pragma once

#include "Bindable.h"
#include "ConstantBuffers.h"

class LightCbuf : public Bindable {
public:
    struct LightData {
        DirectX::XMFLOAT3 position;  // 光照位置
        float pad1;                 // 对齐填充
        DirectX::XMFLOAT4 color;    // 光照颜色（包括 RGBA）
        float intensity;            // 光照强度
        float pad2[3];              // 对齐填充，保证 16 字节对齐
    };

    LightCbuf(Graphics& gfx, const LightData& lightData)
        : lightBuffer(std::make_unique<PixelConstantBuffer<LightData>>(gfx, lightData)) {}

    void Update(Graphics& gfx, const LightData& lightData) {
        lightBuffer->Update(gfx, lightData);
    }

    virtual void Bind(Graphics& gfx) noexcept override {
        lightBuffer->Bind(gfx);
    }

private:
    std::unique_ptr<PixelConstantBuffer<LightData>> lightBuffer;
};

