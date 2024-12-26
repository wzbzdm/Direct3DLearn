#pragma once

#include "Shape3D.h"
#include "Diamond.h"

class Diamond3D : public Shape3D<Diamond3D>
{
public:
	Diamond3D() = default;
	Diamond3D(Graphics& gfx);
	void SetSize(const DirectX::XMFLOAT3& size);
	void Zoom(const DirectX::XMFLOAT3& scale);
	void Update(float dt) noexcept;
	DirectX::XMMATRIX GetTransformMatrix() const noexcept override;
	bool RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept override;
	void InitColor() noexcept;
private:
	DirectX::XMFLOAT3 size = { 1.0, 1.0, 1.0 };
};