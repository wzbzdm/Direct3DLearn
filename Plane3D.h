#pragma once

#include "Shape3D.h"

// 平面
class Plane3D : public Shape3D<Plane3D>
{
	static DirectX::XMFLOAT2 defaultSize;
public:
    Plane3D() = default;
    Plane3D(Graphics& gfx);

	//void Update(float dt) noexcept;
    void SetDimensions(const DirectX::XMFLOAT2& size);
    void ScaleDimensions(const DirectX::XMFLOAT2& factors);
    DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
    DirectX::XMFLOAT2 size = defaultSize; // 长和宽的放大比例
};