#include "Cylinder.h"
#include "Cylinder3D.h"
#include "BindableBase.h"

DirectX::XMFLOAT3 Cylinder3D::conf = { 1.0f, 1.0f, 1.0 };

Cylinder3D::Cylinder3D(Graphics& gfx)
{
    InitColor();
    if (!IsStaticInitialized()) {
        auto model = Cylinder().CreateD();

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
    conf = size;  // 设置顶部半径、底部半径、高度
}

void Cylinder3D::ScaleSize(const DirectX::XMFLOAT3& radio)
{
    conf.x *= radio.x;
    conf.y *= radio.y;
    conf.z *= radio.z;  // 缩放操作
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
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(conf.x, conf.y, conf.z);

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}

void Cylinder3D::InitColor() noexcept
{
    SetColors({
		{ 0.0f, 1.0f, 0.0f, 1.0f },  // 顶部
		{ 1.0f, 0.0f, 0.0f, 1.0f },  // 底部
		{ 0.0f, 0.0f, 1.0f, 1.0f }   // 侧面
        });
}