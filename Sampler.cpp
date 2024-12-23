#include "Sampler.h"

// 默认配置构造函数
Sampler::Sampler(Graphics& gfx) {
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0.0f;
    samplerDesc.BorderColor[1] = 0.0f;
    samplerDesc.BorderColor[2] = 0.0f;
    samplerDesc.BorderColor[3] = 0.0f;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    InitSampler(gfx, samplerDesc);
}

// 自定义配置构造函数
Sampler::Sampler(Graphics& gfx, const D3D11_SAMPLER_DESC& samplerDesc) {
    InitSampler(gfx, samplerDesc);
}

// 绑定采样器
void Sampler::Bind(Graphics& gfx, unsigned int start, unsigned int numSamplers) noexcept {
    GetContext(gfx)->PSSetSamplers(start, numSamplers, pSamplerState.GetAddressOf());
}

void Sampler::InitSampler(Graphics& gfx, const D3D11_SAMPLER_DESC& samplerDesc) {
    HRESULT hr = GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSamplerState);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create sampler state");
    }
}