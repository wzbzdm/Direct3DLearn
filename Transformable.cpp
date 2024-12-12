#include "Transformable.h"

void Transformable::SetPosition(const DirectX::XMFLOAT3& position)
{
	pos = position;
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

void Transformable::Rotate(const DirectX::XMFLOAT3& delta)
{
	rotation.x += delta.x;
	rotation.y += delta.y;
	rotation.z += delta.z;
}

void Transformable::Update(float dt) noexcept
{
	rotation.x += 1.8f * dt;
	rotation.y += 1.6f * dt;
	rotation.z += 2.0f * dt;
}