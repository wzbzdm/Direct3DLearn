#pragma once

#include "Bindable.h"
#include "ConstantBuffers.h"

class TransformCbuf : public Bindable {
public:
    struct TransformData {
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    TransformCbuf(Graphics& gfx, const TransformData& transformData)
        : transformBuffer(std::make_unique<VertexConstantBuffer<TransformData>>(gfx, transformData)) {}

    void Update(Graphics& gfx, const TransformData& transformData) {
        TransformData updatedData = transformData;
        updatedData.view = DirectX::XMMatrixTranspose(updatedData.view);  // 转置以适配 HLSL
        updatedData.projection = DirectX::XMMatrixTranspose(updatedData.projection);
        transformBuffer->Update(gfx, updatedData);
    }

    virtual void Bind(Graphics& gfx) noexcept override {
        transformBuffer->Bind(gfx);
    }

private:
    std::unique_ptr<VertexConstantBuffer<TransformData>> transformBuffer;
};

