#include "Hexahedron.h"
#include "BoxTest.h"

Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
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

	SetMaterialProperties(MATERIAL_GLASS);
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 10.0f);
}

bool Box::RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept {
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

void Box::InitColor() noexcept {
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

void Box::SetSize(const DirectX::XMFLOAT3& size) {

}

void Box::Zoom(const DirectX::XMFLOAT3& scale) {

}
