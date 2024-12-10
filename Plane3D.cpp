#include "Plane3D.h"
#include "BindableBase.h"

Plane3D::Plane3D(Graphics& gfx)
{
    
}

void Plane3D::SetPosition(const DirectX::XMFLOAT3& position) noexcept
{
    pos = position;
}

void Plane3D::Translate(const DirectX::XMFLOAT3& offset) noexcept
{
    pos.x += offset.x;
    pos.y += offset.y;
    pos.z += offset.z;
}

void Plane3D::SetRotation(const DirectX::XMFLOAT3& rotation) noexcept
{
    this->rotation = rotation;
}

void Plane3D::Rotate(const DirectX::XMFLOAT3& delta) noexcept
{
    rotation.x += delta.x;
    rotation.y += delta.y;
    rotation.z += delta.z;
}

void Plane3D::SetDimensions(const DirectX::XMFLOAT2& dims)
{
    dimensions = dims;
}

void Plane3D::ScaleDimensions(const DirectX::XMFLOAT2& factors)
{
    dimensions.x *= factors.x;
    dimensions.y *= factors.y;
}

DirectX::XMMATRIX Plane3D::GetTransformMatrix() const noexcept
{
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(dimensions.x, 1.0f, dimensions.y);
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationX(rotation.x) *
        DirectX::XMMatrixRotationY(rotation.y) *
        DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    return scaling * rotationMatrix * translation;
}
