#pragma once

#include "Bindable.h"

class InputLayout : public Bindable {
public:
	~InputLayout();
	InputLayout(Graphics& gtx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
	void Bind(Graphics& gtx) noexcept override;
protected:
	ID3D11InputLayout* pInputLayout = nullptr;
};