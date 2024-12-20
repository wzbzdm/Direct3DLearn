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

// pos 为位置
// radius为半径
// 计算射线与球是否有交点
bool Sphere3D::RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept {
    using namespace DirectX;

    XMVECTOR sphereCenter = XMLoadFloat3(&pos);
    XMVECTOR rayOrigin = XMLoadFloat3(&ray.GetOrigin());
    XMVECTOR rayDir = XMVector3Normalize(XMLoadFloat3(&ray.GetDirection()));

    // L 是射线起点到球心的向量
    XMVECTOR L = sphereCenter - rayOrigin;

    // 射线方向在 L 上的投影长度
    float tca = XMVectorGetX(XMVector3Dot(L, rayDir));

    // 投影的平方减去 L 的平方就是半径方向的平方
    float d2 = XMVectorGetX(XMVector3Dot(L, L)) - tca * tca;
    float radius2 = radius * radius;

    if (d2 > radius2) {
        // 射线未与球体相交
        return false;
    }

    // 计算交点距离
    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    // 如果 t0 和 t1 都小于 0，射线在球体背后
    if (t0 < 0 && t1 < 0) return false;

    // 选择最近的正交点
    float t = (t0 >= 0) ? t0 : t1;
    XMVECTOR intersection = rayOrigin + t * rayDir;

    XMStoreFloat3(&intersectionPoint, intersection);
    return true;
}


void Sphere3D::InitColor() noexcept
{
	SetColors({
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }
		});
}