#pragma once

#include "Bindable.h"
#include "Shape3DBase.h"
#include "ConstantBuffers.h"

class ShapeConf : public Bindable {
private:
	const Shape3DBase& parent;
	static std::unique_ptr<SharedConstantBuffer<ShapeConfig>> confBuffer;
public:
	ShapeConf(Graphics& gfx, const Shape3DBase& parent);
	void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override;
};