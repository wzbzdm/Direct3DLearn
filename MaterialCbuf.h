#pragma once

#include "Shape3DBase.h"
#include "ConstantBuffers.h"

// 材质
class MaterialCbuf : public Bindable {
public:
    MaterialCbuf(Graphics& gfx, const Shape3DBase& parent);
    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override;

private:
    static std::unique_ptr<PixelConstantBuffer<MaterialProperties>> pixelConstantBuffer;
    const Shape3DBase& parent;
};
