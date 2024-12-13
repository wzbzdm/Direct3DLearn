#pragma once

#include "Shape3D.h"
#include "ConstantBuffers.h"

// 材质
class MaterialCbuf : public Bindable {
public:
    MaterialCbuf(Graphics& gfx, const Shape3DBase& parent)
        : pixelConstantBuffer(gfx), parent(parent) {}

    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override {
        MaterialProperties matProps = parent.GetMaterialProperties(); // 获取材质数据
        pixelConstantBuffer.Update(gfx, matProps);
        pixelConstantBuffer.Bind(gfx, start, len);
    }

private:
    PixelConstantBuffer<MaterialProperties> pixelConstantBuffer;
    const Shape3DBase& parent;
};
