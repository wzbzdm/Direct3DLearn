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
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(radius * size.x, radius * size.y, radius * size.z);

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}

// 计算当前图形与射线的交点
// 位置 pos
// 大小 size
// 旋转 rotation
bool Hexahedron3D::RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept {
    // 1. 获取六面体的世界空间 8 个顶点
    DirectX::XMMATRIX transform = GetTransformMatrix();
    std::vector<DirectX::XMFLOAT3> vertices = {
        {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
        {0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f},
        {0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}
    };

    // 变换到世界空间
    for (auto& v : vertices) {
        DirectX::XMVECTOR vert = DirectX::XMLoadFloat3(&v);
        vert = DirectX::XMVector3Transform(vert, transform);
        DirectX::XMStoreFloat3(&v, vert);
    }

    // 2. 定义六面体的 6 个平面，每个平面由 2 个三角形组成
    static const int indices[12][3] = {
        {0, 1, 2}, {0, 2, 3}, // -Z 面
        {4, 6, 5}, {4, 7, 6}, // +Z 面
        {0, 4, 5}, {0, 5, 1}, // -Y 面
        {2, 6, 7}, {2, 7, 3}, // +Y 面
        {0, 3, 7}, {0, 7, 4}, // -X 面
        {1, 5, 6}, {1, 6, 2}  // +X 面
    };

    bool hasIntersection = false;
    float minT = FLT_MAX;

    // 3. 射线与每个三角形进行检测
    for (int i = 0; i < 12; ++i) {
        const auto& tri = indices[i];

        // 加载三角形顶点
        DirectX::XMFLOAT3 v0 = vertices[tri[0]];
        DirectX::XMFLOAT3 v1 = vertices[tri[1]];
        DirectX::XMFLOAT3 v2 = vertices[tri[2]];

        // 计算射线与三角形的交点
        DirectX::XMFLOAT3 barycentricCoords;
        float t;
        if (ray.RayIntersectsTriangle(v0, v1, v2, t, barycentricCoords)) {
            if (t < minT && t > 0) { // 检查最近的交点且必须是前方
                minT = t;
                intersectionPoint = {
                    ray.GetOrigin().x + ray.GetDirection().x * t,
                    ray.GetOrigin().y + ray.GetDirection().y * t,
                    ray.GetOrigin().z + ray.GetDirection().z * t
                };
                hasIntersection = true;
            }
        }
    }

    return hasIntersection;
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