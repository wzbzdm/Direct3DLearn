#include "Diamond3D.h"
#include "BindableBase.h"

Diamond3D::Diamond3D(Graphics& gfx) {

}

void Diamond3D::SetSize(const DirectX::XMFLOAT3& size) {

}

void Diamond3D::Zoom(const DirectX::XMFLOAT3& scale) {

}

void Diamond3D::Update(float dt) noexcept {

}

DirectX::XMMATRIX Diamond3D::GetTransformMatrix() const noexcept {
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

bool Diamond3D::RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept {
	return false;
}

void Diamond3D::InitColor() noexcept {

}