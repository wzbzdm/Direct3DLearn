#include "Sphere3D.h"

void Sphere3D::SetPosition(const DirectX::XMFLOAT3& position) noexcept
{
    pos = position;
}

void Sphere3D::Translate(const DirectX::XMFLOAT3& offset) noexcept
{
    pos.x += offset.x;
    pos.y += offset.y;
    pos.z += offset.z;
}

void Sphere3D::SetRotation(const DirectX::XMFLOAT3& rotation) noexcept
{
    return;
}

void Sphere3D::Rotate(const DirectX::XMFLOAT3& delta) noexcept
{
    return;
}

void Sphere3D::SetRadius(float r)
{
    radius = r;
}

void Sphere3D::ScaleRadius(float factor)
{
    radius *= factor;
}

DirectX::XMMATRIX Sphere3D::GetTransformMatrix() const noexcept
{
    // 平移 + 缩放 (球体只需要用同一个比例因子缩放所有方向)
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(radius, radius, radius);
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    return scaling * translation;
}
