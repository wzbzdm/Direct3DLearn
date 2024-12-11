#pragma once

#include "LightCbuf.h"
#include "CameraCbuf.h"

class Env {
public:
    Env(Graphics& gfx)
        : gfx(gfx),
        lightCbuf(gfx, { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, {0.0f, 0.0f, 0.0f} }),
        transformCbuf(gfx, { DirectX::XMMatrixIdentity(), DirectX::XMMatrixIdentity() }) {}

    // 光照相关
    void SetLight(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color, float intensity) {
        LightCbuf::LightData lightData = { position, 0.0f, color, intensity, {0.0f, 0.0f, 0.0f} };
        lightCbuf.Update(gfx, lightData);
    }

    // 相机相关
    void SetCamera(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) {
        TransformCbuf::TransformData transformData = { view, projection };
        transformCbuf.Update(gfx, transformData);
    }

    // 绑定光照
    void BindLighting() const {
        lightCbuf.Bind(gfx);
    }

    // 绑定相机
    void BindCamera() const {
        transformCbuf.Bind(gfx);
    }

private:
    Graphics& gfx;  // 存储 Graphics 的引用
    LightCbuf lightCbuf;
    TransformCbuf transformCbuf;
};
