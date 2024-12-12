#pragma once

#include "Bindable.h"

class PixelShader : public Bindable {
public:
	~PixelShader();
	PixelShader(Graphics& gtx, const std::wstring& path);
	void Bind(Graphics& gtx) noexcept override;
protected:
	ID3D11PixelShader* pPixelShader = nullptr;
};