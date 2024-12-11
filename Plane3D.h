#pragma once

#include "Shape3D.h"

class Plane3D : public Shape3D<Plane3D>
{
public:
    Plane3D() = default;
    Plane3D(Graphics& gfx);

    void SetPosition(const DirectX::XMFLOAT3& position) noexcept override;
    void Translate(const DirectX::XMFLOAT3& offset) noexcept override;
    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;
    void Rotate(const DirectX::XMFLOAT3& delta) noexcept override;
    void SetDimensions(const DirectX::XMFLOAT2& dimensions);
    void ScaleDimensions(const DirectX::XMFLOAT2& factors);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
    DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT2 dimensions = { 1.0f, 1.0f }; // 长和宽
};