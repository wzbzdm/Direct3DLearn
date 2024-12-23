#include "MaterialCbuf.h"

MaterialCbuf::MaterialCbuf(Graphics& gfx, const Shape3DBase& parent) : parent(parent) {
	if (!pixelConstantBuffer) {
		pixelConstantBuffer = std::make_unique<PixelConstantBuffer<MaterialProperties>>(gfx);
	}
}

void MaterialCbuf::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
	MaterialProperties mp = parent.GetMaterialProperties();
	pixelConstantBuffer->Update(gfx, mp);
	pixelConstantBuffer->Bind(gfx, start, len);
}

std::unique_ptr<PixelConstantBuffer<MaterialProperties>> MaterialCbuf::pixelConstantBuffer;