#pragma once

#include "Shape3DBase.h"
#include "ConstantBuffers.h"

class ColorCbuf : public Bindable {
public:
	ColorCbuf(Graphics& gfx, const Shape3DBase& shape);
	void Bind(Graphics& gfx, unsigned int start, unsigned int num) noexcept override;

private:
	static std::unique_ptr<PixelConstantBuffer<DirectX::XMFLOAT4>> colorBuffer;
	const Shape3DBase& shape;
};