#pragma once

#include "ConstantBuffers.h"

class ColorCbuf : public Bindable {
public:
	ColorCbuf(Graphics& gfx, std::vector<DirectX::XMFLOAT4>& colors);
	void Bind(Graphics& gfx, unsigned int start, unsigned int num) noexcept override;

private:
	static std::unique_ptr<PixelConstantBuffer<DirectX::XMFLOAT4>> colorBuffer;
	std::vector<DirectX::XMFLOAT4>& colors;
};