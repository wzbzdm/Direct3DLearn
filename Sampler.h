#pragma once

#include "Bindable.h"

class Sampler : public Bindable {
public:
    // 默认配置构造函数
    Sampler(Graphics& gfx);

    // 自定义配置构造函数
    Sampler(Graphics& gfx, const D3D11_SAMPLER_DESC& samplerDesc);

    // 绑定采样器
    void Bind(Graphics& gfx, unsigned int start = 0, unsigned int numSamplers = 1) noexcept override;

private:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;

    void InitSampler(Graphics& gfx, const D3D11_SAMPLER_DESC& samplerDesc);
};
