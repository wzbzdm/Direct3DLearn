#include "LightCbuf.h"

LightCbuf::LightCbuf(Graphics& gfx) noexcept {
	if (!lightsbuf) {
		lightsbuf = std::make_unique<PixelConstantBuffer<std::vector<LightBuffer>>>(gfx);
	}
}

void LightCbuf::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
	std::vector<LightBuffer> lights = gfx.GetLightBuffer();
	lightsbuf->Update(gfx, lights);
	lightsbuf->Bind(gfx, start, len);
}

std::unique_ptr<PixelConstantBuffer<std::vector<LightBuffer>>> LightCbuf::lightsbuf;