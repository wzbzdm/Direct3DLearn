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
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(this->radius * size.x, this->radius * size.y, 1.0);  // Z轴方向不缩放

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}

// 计算当前图形与射线的交点
// 位置 pos
// 大小 size
// 旋转 rotation
bool Plane3D::RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept {
    struct Position {
        DirectX::XMFLOAT3 pos;
    };
    Geometry<Position> poss = Plane::Create<Position>();
    DirectX::XMMATRIX transform = GetTransformMatrix();
    for (auto& v : poss.vertices) {
        DirectX::XMVECTOR vert = DirectX::XMLoadFloat3(&v.pos);
        vert = DirectX::XMVector3Transform(vert, transform);
        DirectX::XMStoreFloat3(&v.pos, vert);
    }

    bool hasIntersection = false;
    float lastZ = FLT_MAX;
    for (int i = 0; i < poss.indices.size() - 2; i += 3) {
        DirectX::XMFLOAT3 lastIn;
        float t;
        if (ray.RayIntersectsTriangle(
            poss.vertices[poss.indices[i]].pos,
            poss.vertices[poss.indices[i + 1]].pos,
            poss.vertices[poss.indices[i + 2]].pos,
            t, lastIn)) {
            if (t >= lastZ) continue;
            lastZ = t;
            intersectionPoint = {
                ray.GetOrigin().x + ray.GetDirection().x * t,
                ray.GetOrigin().y + ray.GetDirection().y * t,
                ray.GetOrigin().z + ray.GetDirection().z * t
            };
            hasIntersection = true;
        }
    }

    return hasIntersection;
}


void Plane3D::InitColor() noexcept {
    SetColors({
		{ 0.5f, 1.0f, 0.5f, 1.0f },
		{ 0.5f, 0.5f, 1.0f, 1.0f }
        });
}