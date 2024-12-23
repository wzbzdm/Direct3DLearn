#pragma once

#include "Bindable.h"
#include "TextureData.h"

class TextureBase : public Bindable {
protected:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11Resource> pTexture;

    static UINT BitsPerPixel(DXGI_FORMAT format);
    void CreateShaderResourceView(Graphics& gfx, DXGI_FORMAT format, D3D11_SRV_DIMENSION dimension, void* resource);
};

class Texture1D : public TextureBase {
public:
    Texture1D(Graphics& gfx, const TextureData& textureData);
    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override;
};

class Texture2D : public TextureBase {
public:
    Texture2D(Graphics& gfx, const TextureData& textureData);
    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override;
};

class Texture3D : public TextureBase {
public:
    Texture3D(Graphics& gfx, const TextureData& textureData);
    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept;
};
