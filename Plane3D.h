#pragma once

#include "Drawable.h"
#include "Transformable.h"

class Plane3D : public Drawable, public Transformable
{
public:
    Plane3D() = default;

    void SetPosition(const DirectX::XMFLOAT3& position) noexcept override;
    void Translate(const DirectX::XMFLOAT3& offset) noexcept override;
    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;
    void Rotate(const DirectX::XMFLOAT3& delta) noexcept override;
    void SetDimensions(const DirectX::XMFLOAT2& dimensions);
    void ScaleDimensions(const DirectX::XMFLOAT2& factors);
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
    DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT2 dimensions = { 1.0f, 1.0f }; // 长和宽
};
