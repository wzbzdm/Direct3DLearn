#include "ColorCbuf.h"

ColorCbuf::ColorCbuf(Graphics& gfx, std::vector<DirectX::XMFLOAT4>& colors) : colors(colors) {
	if (!colorBuffer) {
		colorBuffer = std::make_unique<PixelConstantBuffer<DirectX::XMFLOAT4>>(gfx);
	}
}

void ColorCbuf::Bind(Graphics& gfx, unsigned int start, unsigned int num) noexcept {
	colorBuffer->Update(gfx, colors);
	colorBuffer->Bind(gfx, start, num);
}

std::unique_ptr<PixelConstantBuffer<DirectX::XMFLOAT4>> ColorCbuf::colorBuffer;