#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gtx, const std::wstring& path) {
	// Create pixel shader
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	GetDevice(gtx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
}

void PixelShader::Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept {
	GetContext(gtx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}