#pragma once

#include "Bindable.h"
#include "TextureData.h"
#include "Shape3DBase.h"
#include "UpdateNotifier.h"

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

class Texture2D : public TextureBase, public UpdateNotifier {
private:
    Shape3DBase& parent;
	Graphics& gfx;
    void InitializeTexture(const std::wstring& path);
public:
    // Texture2D(Graphics& gfx, const TextureData& textureData);
    Texture2D(Graphics& gfx, Shape3DBase& parent);
    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override;
	void OnUpdate() override;
    ID3D11ShaderResourceView* GetTextureView() const noexcept;
};

class Texture3D : public TextureBase {
public:
    Texture3D(Graphics& gfx, const TextureData& textureData);
    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept;
};
