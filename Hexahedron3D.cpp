#include "Hexahedron3D.h"

using namespace DirectX;

Hexahedron3D::Hexahedron3D()
{
    // 默认构造器，pos 和 size 已在声明时初始化
}

void Hexahedron3D::SetPosition(const XMFLOAT3& position) noexcept
{
    pos = position;  // 设置位置
}

void Hexahedron3D::Translate(const XMFLOAT3& offset) noexcept
{
    pos.x += offset.x;
    pos.y += offset.y;
    pos.z += offset.z;
}

void Hexahedron3D::SetRotation(const XMFLOAT3& rotation) noexcept
{
    this->rotation = rotation;  // 设置旋转（欧拉角）
}

void Hexahedron3D::Rotate(const XMFLOAT3& delta) noexcept
{
    rotation.x += delta.x;
    rotation.y += delta.y;
    rotation.z += delta.z;
}

void Hexahedron3D::SetSize(const XMFLOAT3& size)
{
    this->size = size;  // 设置大小（长宽高）
}

void Hexahedron3D::Zoom(const XMFLOAT3& scale)
{
    size.x *= scale.x;
    size.y *= scale.y;
    size.z *= scale.z;  // 缩放操作，根据传入的比例因子来调整尺寸
}

XMMATRIX Hexahedron3D::GetTransformXM() const noexcept
{
    // 获取平移矩阵
    XMMATRIX translation = XMMatrixTranslation(pos.x, pos.y, pos.z);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    XMMATRIX rotationX = XMMatrixRotationX(rotation.x);
    XMMATRIX rotationY = XMMatrixRotationY(rotation.y);
    XMMATRIX rotationZ = XMMatrixRotationZ(rotation.z);
    XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    // 获取缩放矩阵（按 X, Y, Z 三个方向的缩放）
    XMMATRIX scaling = XMMatrixScaling(size.x, size.y, size.z);

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}
