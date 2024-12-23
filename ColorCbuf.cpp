#include "ColorCbuf.h"

ColorCbuf::ColorCbuf(Graphics& gfx, const Shape3DBase& shape) : shape(shape) {
	if (!colorBuffer) {
		colorBuffer = std::make_unique<PixelConstantBuffer<DirectX::XMFLOAT4>>(gfx, shape.GetColors());
	}
}

void ColorCbuf::Bind(Graphics& gfx, unsigned int start, unsigned int num) noexcept {
	colorBuffer->Update(gfx, shape.GetColors());
	colorBuffer->Bind(gfx, start, num);
}

std::unique_ptr<PixelConstantBuffer<DirectX::XMFLOAT4>> ColorCbuf::colorBuffer;