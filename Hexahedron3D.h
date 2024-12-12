#pragma once

#include "Shape3D.h"

class Hexahedron3D : public Shape3D<Hexahedron3D>
{
	static DirectX::XMFLOAT3 defaultSize;
public:
	Hexahedron3D() = default;
	Hexahedron3D(Graphics&);
	void SetSize(const DirectX::XMFLOAT3& size);
	void Zoom(const DirectX::XMFLOAT3& scale);
	void Update(float dt) noexcept;
	DirectX::XMMATRIX GetTransformMatrix() const noexcept override;
private:
	DirectX::XMFLOAT3 size = { 1.0, 1.0, 1.0 };
};