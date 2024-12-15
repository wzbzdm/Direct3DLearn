#include "LightCbuf.h"

LightCbuf::LightCbuf(Graphics& gfx) noexcept {
	if (!lightsbuf) {
		lightsbuf = std::make_unique<PixelConstantBuffer<LightBuffer>>(gfx, gfx.GetLightBuffer());
	}
}

void LightCbuf::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
	lightsbuf->Update(gfx, gfx.GetLightBuffer());
	lightsbuf->Bind(gfx, start, len);
}

std::unique_ptr<PixelConstantBuffer<LightBuffer>> LightCbuf::lightsbuf;