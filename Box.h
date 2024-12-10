#pragma once

#include "Drawable.h"

class Box : public Drawable {
public:
	Box(Graphics& gfx, float size = 1.0f);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f, 1.0f, 1.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float size;
};