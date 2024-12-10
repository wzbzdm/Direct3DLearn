#include "Cylinder3D.h"

void Cylinder3D::SetPosition(const DirectX::XMFLOAT3& position) noexcept
{
    pos = position;
}

void Cylinder3D::Translate(const DirectX::XMFLOAT3& offset) noexcept
{
    pos.x += offset.x;
    pos.y += offset.y;
    pos.z += offset.z;
}

void Cylinder3D::SetRotation(const DirectX::XMFLOAT3& rotation) noexcept
{
    this->rotation = rotation;
}

void Cylinder3D::Rotate(const DirectX::XMFLOAT3& delta) noexcept
{
    rotation.x += delta.x;
    rotation.y += delta.y;
    rotation.z += delta.z;
}

void Cylinder3D::SetDimensions(float radius, float h)
{
    baseRadius = radius;
    height = h;
}

void Cylinder3D::ScaleDimensions(const DirectX::XMFLOAT2& factors)
{
    baseRadius *= factors.x; // 按 X 比例缩放半径
    height *= factors.y;     // 按 Y 比例缩放高度
}

DirectX::XMMATRIX Cylinder3D::GetTransformMatrix() const noexcept
{
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(baseRadius, height, baseRadius);
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationX(rotation.x) *
        DirectX::XMMatrixRotationY(rotation.y) *
        DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    return scaling * rotationMatrix * translation;
}
