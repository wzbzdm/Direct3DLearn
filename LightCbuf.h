#pragma once

#include "MyLightManager.h"
#include "Bindable.h"
#include "ConstantBuffers.h"

class LightCbuf : public Bindable {
public:
	LightCbuf(Graphics& gfx) noexcept;
	void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override;

private:
	static std::unique_ptr<PixelConstantBuffer<std::vector<LightBuffer>>> lightsbuf;
};