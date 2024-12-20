#include "Cylinder.h"
#include "Cylinder3D.h"
#include "BindableBase.h"

Cylinder3D::Cylinder3D(Graphics& gfx)
{
    InitColor();
    if (!IsStaticInitialized()) {
        auto model = Cylinder().CreateD(size.x, size.y, size.z);

        BindStaticAll(gfx, model);
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));
    }
    else {
        SetIndexFromStatic();
    }

    // 动态绑定数据
    this->BindAll(gfx);

    SetMaterialProperties(MATERIAL_GLASS);
}

void Cylinder3D::SetConf(int numC, int numH) {
	this->numC = numC;
	this->numH = numH;
}

void Cylinder3D::SetSize(const DirectX::XMFLOAT3& size)
{
    this->size = size;  // 设置顶部半径、底部半径、高度
}

void Cylinder3D::ScaleSize(const DirectX::XMFLOAT3& radio)
{
    this->size.x *= radio.x;
    this->size.y *= radio.y;
    this->size.z *= radio.z;  // 缩放操作
}

DirectX::XMMATRIX Cylinder3D::GetTransformMatrix() const noexcept
{
    // 获取平移矩阵
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(rotation.x);
    DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(rotation.y);
    DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    // 获取缩放矩阵（按 X, Y, Z 三个方向的缩放）
    // DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(this->size.x, this->size.y, this->size.z);

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    // 圆柱的缩放需要单独处理, 重新创建图形然后绑定缓冲区
    return rotationMatrix * translation;
}

// 计算当前图形与射线的交点
// 位置 pos
// 大小 size  分别为: 顶部半径，底部半径和高度
// 旋转 rotation
bool Cylinder3D::RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept {
    return false;
}

void Cylinder3D::InitColor() noexcept
{
    SetColors({
		{ 0.0f, 1.0f, 0.0f, 1.0f },  // 顶部
		{ 1.0f, 0.0f, 0.0f, 1.0f },  // 底部
		{ 0.0f, 0.0f, 1.0f, 1.0f }   // 侧面
        });
}