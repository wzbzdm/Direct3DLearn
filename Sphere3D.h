#pragma once

#include "Shape3D.h"

class Sphere3D : public Shape3D<Sphere3D>
{
public:
    Sphere3D() = default;
    Sphere3D(Graphics& gfx);

    void SetPosition(const DirectX::XMFLOAT3& position) noexcept override;
    void Translate(const DirectX::XMFLOAT3& offset) noexcept override;
    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;
    void Rotate(const DirectX::XMFLOAT3& delta) noexcept override;
    void SetRadius(float radius);
    void ScaleRadius(float factor);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
    DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
    float radius = 1.0f; // 半径
};
