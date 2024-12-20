#include "Plane.h"
#include "Plane3D.h"
#include "BindableBase.h"

Plane3D::Plane3D(Graphics& gfx)
{
    InitColor();
    if (!IsStaticInitialized()) {
        auto model = Plane().CreateD();

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

void Plane3D::SetDimensions(const DirectX::XMFLOAT2& size)
{
    this->size = size;  // 设置平面尺寸（长、宽）
}

void Plane3D::ScaleDimensions(const DirectX::XMFLOAT2& factors)
{
    size.x *= factors.x;
    size.y *= factors.y;  // 缩放操作，根据传入的比例因子来调整尺寸
}

DirectX::XMMATRIX Plane3D::GetTransformMatrix() const noexcept
{
    // 获取平移矩阵
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(rotation.x);
    DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(rotation.y);
    DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    // 获取缩放矩阵（按 X, Y 三个方向的缩放）
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(size.x, size.y, 1.0);  // Z轴方向不缩放

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}

// 计算当前图形与射线的交点
// 位置 pos
// 大小 size
// 旋转 rotation
bool Plane3D::RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept {
    using namespace DirectX;

    // 平面的初始法向量 (0, 0, 1)
    XMVECTOR initialNormal = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    // 获取平面的旋转变换矩阵
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

    // 应用旋转变换到法向量
    XMVECTOR transformedNormal = XMVector3TransformNormal(initialNormal, rotationMatrix);

    // 取平面上的一个点（中心点 pos）
    XMVECTOR planePoint = XMLoadFloat3(&pos);

    // 射线起点和方向
    XMVECTOR rayOrigin = XMLoadFloat3(&ray.GetOrigin());
    XMVECTOR rayDir = XMVector3Normalize(XMLoadFloat3(&ray.GetDirection()));

    // 计算射线方向和法向量的点积
    float denom = XMVectorGetX(XMVector3Dot(transformedNormal, rayDir));

    // 如果点积接近 0，则射线与平面平行
    if (fabs(denom) < 1e-6f) {
        return false; // 平行，无交点
    }

    // 计算 t 参数
    float t = XMVectorGetX(XMVector3Dot(planePoint - rayOrigin, transformedNormal)) / denom;

    // 如果 t < 0，交点在射线的反方向
    if (t < 0) {
        return false;
    }

    // 计算交点
    XMVECTOR intersection = rayOrigin + t * rayDir;
    XMStoreFloat3(&intersectionPoint, intersection);

    return true;
}


void Plane3D::InitColor() noexcept {
    SetColors({
		{ 0.5f, 1.0f, 0.5f, 1.0f },
		{ 0.5f, 0.5f, 1.0f, 1.0f }
        });
}