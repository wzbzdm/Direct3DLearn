#include "VertexShader.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path) {
	// Create the vertex shader
	D3DReadFileToBlob(path.c_str(), &pBytecodeBlob);
	GetDevice(gfx)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(), pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader);
}

void VertexShader::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->VSSetShader(pVertexShader, nullptr, 0u);
}

VertexShader::~VertexShader() {
	if (pVertexShader != nullptr) {
		pVertexShader->Release();
		pVertexShader = nullptr;
	}
	if (pBytecodeBlob != nullptr) {
		pBytecodeBlob->Release();
		pBytecodeBlob = nullptr;
	}
}

ID3DBlob* VertexShader::GetBytecode() const noexcept {
	return pBytecodeBlob;
}