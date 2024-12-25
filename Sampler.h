#pragma once

#include "Bindable.h"
#include "Shape3DBase.h"
#include "UpdateNotifier.h"

class Sampler : public Bindable, public UpdateNotifier {
public:
    // 默认配置构造函数
    // Sampler(Graphics& gfx);

    // 自定义配置构造函数
    Sampler(Graphics& gfx, Shape3DBase& parent);

    // 绑定采样器
    void Bind(Graphics& gfx, unsigned int start = 0, unsigned int numSamplers = 1) noexcept override;
	void OnUpdate() override;
private:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
    Shape3DBase& parent;
	Graphics& gfx;
    void InitSampler(Graphics& gfx, const D3D11_SAMPLER_DESC& samplerDesc);
};
