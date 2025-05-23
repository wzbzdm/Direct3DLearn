#pragma once

#include "Bindable.h"

class PixelShader : public Bindable {
public:
	PixelShader(Graphics& gtx, const std::wstring& path);
	void Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};