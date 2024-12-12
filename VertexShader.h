#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	~VertexShader();
	VertexShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;
protected:
	ID3DBlob* pBytecodeBlob = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
};