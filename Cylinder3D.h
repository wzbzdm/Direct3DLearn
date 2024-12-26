#pragma once

#include "Shape3D.h"

// 圆柱体
class Cylinder3D : public Shape3D<Cylinder3D>
{
public:
    Cylinder3D() = default;
    Cylinder3D(Graphics& gfx);

	void SetConf(int numC,int numH);
    void SetSize(const DirectX::XMFLOAT3& size);
    void ScaleSize(const DirectX::XMFLOAT3& radio);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;
    bool RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept override;
    void InitColor() noexcept;
private:
    DirectX::XMFLOAT3 size = { 1.0f, 1.0f, 2.0f }; // 顶部半径、底部半径、高度的缩放比例
    int numC = 36;
    int numH = 36;
};