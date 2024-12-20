#pragma once

#include "Shape3D.h"

// 平面
class Plane3D : public Shape3D<Plane3D>
{
public:
    Plane3D() = default;
    Plane3D(Graphics& gfx);

	//void Update(float dt) noexcept;
    void SetDimensions(const DirectX::XMFLOAT2& size);
    void ScaleDimensions(const DirectX::XMFLOAT2& factors);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;
    bool RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept override;
    void InitColor() noexcept;

private:
    DirectX::XMFLOAT2 size = { 1.0f, 1.0f }; // 长和宽的放大比例
};