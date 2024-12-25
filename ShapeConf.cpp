#include "ShapeConf.h"

ShapeConf::ShapeConf(Graphics& gfx, const Shape3DBase& parent) : parent(parent) {
	if (!confBuffer) {
		confBuffer = std::make_unique<SharedConstantBuffer<ShapeConfig>>(gfx);
	}
}

void ShapeConf::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
	confBuffer->Update(gfx, parent.GetShapeConfig());
	confBuffer->Bind(gfx, start, len);
}

std::unique_ptr<SharedConstantBuffer<ShapeConfig>> ShapeConf::confBuffer;