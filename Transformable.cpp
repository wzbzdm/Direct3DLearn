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