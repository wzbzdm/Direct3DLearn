#pragma once

#include "Drawable.h"
#include "Transformable.h"

class Sphere3D : public Drawable, public Transformable
{
public:
    Sphere3D() = default;

    void SetPosition(const DirectX::XMFLOAT3& position) noexcept override;
    void Translate(const DirectX::XMFLOAT3& offset) noexcept override;
    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;
    void Rotate(const DirectX::XMFLOAT3& delta) noexcept override;
    void SetRadius(float radius);
    void ScaleRadius(float factor);
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
    DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
    float radius = 1.0f; // 半径
};
