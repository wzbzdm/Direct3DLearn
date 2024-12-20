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
	return false;
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
