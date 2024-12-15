#include "Hexahedron.h"
#include "Hexahedron3D.h"
#include "BindableBase.h"

Hexahedron3D::Hexahedron3D(Graphics& gfx)
{
	InitColor();
	if (!IsStaticInitialized()) {
		auto model = Hexahedron().CreateD();

		BindStaticAll(gfx, model);
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));
	}
	else {
		SetIndexFromStatic();
	}

	// 动态绑定数据
	this->BindAll(gfx);

	SetMaterialProperties(MATERIAL_CERAMIC);
}

void Hexahedron3D::SetSize(const DirectX::XMFLOAT3& size)
{
    this->size = size;  // 设置大小（长宽高）
}

void Hexahedron3D::Zoom(const DirectX::XMFLOAT3& scale)
{
    size.x *= scale.x;
    size.y *= scale.y;
    size.z *= scale.z;  // 缩放操作，根据传入的比例因子来调整尺寸
}

DirectX::XMMATRIX Hexahedron3D::GetTransformMatrix() const noexcept
{
    // 获取平移矩阵
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(rotation.x);
    DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(rotation.y);
    DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    // 获取缩放矩阵（按 X, Y, Z 三个方向的缩放）
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(size.x, size.y, size.z);

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}

void Hexahedron3D::Update(float dt) noexcept
{
    // 自定义 Update
	rotation.x += 2.0f * dt;
    rotation.y += 1.6f * dt;
    rotation.z += 1.2f * dt;
}

void Hexahedron3D::InitColor() noexcept
{
	SetColors({
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
		});
}