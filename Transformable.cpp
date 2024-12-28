#include "Transformable.h"

void Transformable::SetPosition(const DirectX::XMFLOAT3& position)
{
	pos = position;
}

const DirectX::XMFLOAT3& Transformable::GetPosition() const noexcept {
	return this->pos;
}

void Transformable::Translate(const DirectX::XMFLOAT3& offset)
{
	pos.x += offset.x;
	pos.y += offset.y;
	pos.z += offset.z;
}

void Transformable::SetRotation(const DirectX::XMFLOAT3& rotation)
{
	this->rotation = rotation;
	// 使用欧拉角更新四元数
	DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	DirectX::XMStoreFloat4(&this->rotationQuaternion, quat);
}

const DirectX::XMFLOAT3& Transformable::GetRotation() const noexcept {
	return this->rotation;
}

void Transformable::Rotate(const DirectX::XMFLOAT3& delta)
{
	rotation.x += delta.x;
	rotation.y += delta.y;
	rotation.z += delta.z;
}

DirectX::XMFLOAT3 QuaternionToEuler(const DirectX::XMFLOAT4& quaternion)
{
	float qx = quaternion.x;
	float qy = quaternion.y;
	float qz = quaternion.z;
	float qw = quaternion.w;

	// 计算欧拉角
	float sinr_cosp = 2 * (qw * qx + qy * qz);
	float cosr_cosp = 1 - 2 * (qx * qx + qy * qy);
	float roll = std::atan2(sinr_cosp, cosr_cosp);

	float sinp = 2 * (qw * qy - qz * qx);
	float pitch;
	if (std::abs(sinp) >= 1)
		pitch = std::copysign(DirectX::XM_PI / 2, sinp); // 防止越界
	else
		pitch = std::asin(sinp);

	float siny_cosp = 2 * (qw * qz + qx * qy);
	float cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
	float yaw = std::atan2(siny_cosp, cosy_cosp);

	return { pitch, yaw, roll };
}

void Transformable::SetRotationQuaternion(DirectX::XMFLOAT4 quaternion) noexcept
{
	rotationQuaternion = quaternion;

	// 从四元数更新欧拉角
	rotation = QuaternionToEuler(rotationQuaternion);
}

DirectX::XMFLOAT4 Transformable::GetRotationQuaternion() const noexcept
{
	return rotationQuaternion;
}

bool Transformable::CanUpdate() const noexcept {
	return this->canUpdate;
}

void Transformable::SetUpdateState(bool state) noexcept {
	this->canUpdate = state;
}

float Transformable::GetRadius() const noexcept {
	return this->radius;
}

void Transformable::SetRadius(float r) {
	this->radius = r;
}

void Transformable::Zoom(float r) {
	this->radius *= r;
}

void Transformable::Update(float dt) noexcept
{
	rotation.x += 1.8f * dt;
	rotation.y += 1.6f * dt;
	rotation.z += 2.0f * dt;
	float speed = 0.5;
	float width = 1.0;
	// 半径为 0.5
	if (pos.x >= 0 && pos.y >= 0 && pos.x < width) {
		pos.x += dt * speed;
		pos.y += dt * speed;
	}
	else if (pos.x >= width && pos.y > -width) {
		pos.y -= dt * speed;
	}
	else if ((pos.x * pos.y <= 0) && pos.x > -width) {
		pos.x -= speed * dt;
		pos.y += speed * dt;
	}
	else if(pos.x <= -width && pos.y > -width) {
		pos.y -= speed * dt;
	}
	else {
		pos.x += speed * dt;
		pos.y += speed * dt;
	}
}