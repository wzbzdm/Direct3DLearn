#include "PixelShader.h"

PixelShader::~PixelShader() {
	if (pPixelShader != nullptr) {
		pPixelShader->Release();
		pPixelShader = nullptr;
	}
}

PixelShader::PixelShader(Graphics& gtx, const std::wstring& path) {
	// Create pixel shader
	ID3DBlob* pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	GetDevice(gtx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	pBlob->Release();
}

void PixelShader::Bind(Graphics& gtx) noexcept {
	GetContext(gtx)->PSSetShader(pPixelShader, nullptr, 0u);
}