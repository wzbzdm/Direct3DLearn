#include "Sphere.h"
#include "Sphere3D.h"
#include "BindableBase.h"

Sphere3D::Sphere3D(Graphics& gfx)
{
    InitColor();
    if (!IsStaticInitialized()) {
        auto model = Sphere().CreateD();

        BindStaticAll(gfx, model);
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));
    }
    else {
        SetIndexFromStatic();
    }

    // 动态绑定数据
    this->BindAll(gfx);

    SetMaterialProperties(MATERIAL_METAL);
}

void Sphere3D::SetConf(int numC, int numH)
{
	this->numC = numC;
	this->numH = numH;
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
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(radius, radius, radius);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(rotation.x);
    DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(rotation.y);
    DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    return scaling * rotationMatrix * translation;
}

void Sphere3D::InitColor() noexcept
{
	SetColors({
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }
		});
}