#pragma once

#include "Shape3D.h"

// 球体
class Sphere3D : public Shape3D<Sphere3D>
{
public:
    Sphere3D() = default;
    Sphere3D(Graphics& gfx);

    void SetConf(int numC, int numH);
    void SetRadius(float radius);
    void ScaleRadius(float factor);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
    float radius = 1.0f; // 半径
	int numC = 36;
	int numH = 36;
};
