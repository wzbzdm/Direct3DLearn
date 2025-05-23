#pragma once

#include "Bindable.h"

class InputLayout : public Bindable {
public:
	InputLayout(Graphics& gtx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
	void Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};