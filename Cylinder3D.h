#pragma once

#include "Shape3D.h"

// 圆柱体
class Cylinder3D : public Shape3D<Cylinder3D>
{
    static DirectX::XMFLOAT3 conf;
public:
    Cylinder3D() = default;
    Cylinder3D(Graphics& gfx);

    void SetSize(const DirectX::XMFLOAT3& size);
    void ScaleSize(const DirectX::XMFLOAT3& radio);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
    DirectX::XMFLOAT3 size = { 1, 1, 1 }; // 顶部半径、底部半径、高度的缩放比例
    int numSegments = 36;
    int numRings = 36;
};