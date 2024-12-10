#pragma once

#include "Shape3D.h"

class Hexahedron3D : public Shape3D
{
public:
	Hexahedron3D() = default;
	void SetPosition(const DirectX::XMFLOAT3& position) noexcept override;
	void Translate(const DirectX::XMFLOAT3& offset) noexcept override;
	void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;
	void Rotate(const DirectX::XMFLOAT3& delta) noexcept override;
	void SetSize(const DirectX::XMFLOAT3& size);
	void Zoom(const DirectX::XMFLOAT3& scale);
	DirectX::XMMATRIX GetTransformMatrix() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 size = { 0.5f, 0.5f, 0.5f };
};