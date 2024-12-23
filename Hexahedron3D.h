#pragma once

#include "Shape3D.h"
#include "Hexahedron.h"

class Hexahedron3D : public Shape3D<Hexahedron3D>
{
public:
	Hexahedron3D() = default;
	Hexahedron3D(Graphics&);
	void SetSize(const DirectX::XMFLOAT3& size);
	void Zoom(const DirectX::XMFLOAT3& scale);
	void Update(float dt) noexcept;
<<<<<<< HEAD
=======
	DirectX::XMMATRIX GetTransformMatrix() const noexcept override;
	bool RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept override;
	void InitColor() noexcept;
>>>>>>> main
private:
	DirectX::XMFLOAT3 size = { 1.0, 1.0, 1.0 };
};