#pragma once

#include "Bindable.h"
#include "TextureData.h"

class TextureBase : public Bindable {
protected:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11Resource> pTexture;

    static UINT BitsPerPixel(DXGI_FORMAT format) {
        switch (format) {
        case DXGI_FORMAT_R8G8B8A8_UNORM: return 32;
        case DXGI_FORMAT_R32_FLOAT: return 32;
        case DXGI_FORMAT_R16G16_FLOAT: return 32;
        default: throw std::runtime_error("Unsupported DXGI_FORMAT");
        }
    }

    void CreateShaderResourceView(Graphics& gfx, DXGI_FORMAT format, D3D11_SRV_DIMENSION dimension, void* resource) {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = format;
        srvDesc.ViewDimension = dimension;

        if (dimension == D3D11_SRV_DIMENSION_TEXTURE1D) {
            srvDesc.Texture1D.MostDetailedMip = 0;
            srvDesc.Texture1D.MipLevels = 1;
        }
        else if (dimension == D3D11_SRV_DIMENSION_TEXTURE2D) {
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = 1;
        }
        else if (dimension == D3D11_SRV_DIMENSION_TEXTURE3D) {
            srvDesc.Texture3D.MostDetailedMip = 0;
            srvDesc.Texture3D.MipLevels = 1;
        }

        GetDevice(gfx)->CreateShaderResourceView((ID3D11Resource*)resource, &srvDesc, &pShaderResourceView);
    }
};

class Texture1D : public TextureBase {
public:
    Texture1D(Graphics& gfx, const TextureData& textureData) {
        if (textureData.height != 1 || textureData.depth != 1) {
            throw std::runtime_error("Invalid data dimensions for Texture1D");
        }

        D3D11_TEXTURE1D_DESC desc = {};
        desc.Width = textureData.width;
        desc.MipLevels = 1u;
        desc.ArraySize = 1u;
        desc.Format = textureData.format;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0u;
        desc.MiscFlags = 0u;

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = textureData.data.data();

        GetDevice(gfx)->CreateTexture1D(&desc, &sd, (ID3D11Texture1D**)pTexture.GetAddressOf());
        CreateShaderResourceView(gfx, textureData.format, D3D11_SRV_DIMENSION_TEXTURE1D, pTexture.Get());
    }

    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override {
        GetContext(gfx)->PSSetShaderResources(start, len, pShaderResourceView.GetAddressOf());
    }
};

class Texture2D : public TextureBase {
public:
    Texture2D(Graphics& gfx, const TextureData& textureData) {
        if (textureData.height < 1 || textureData.depth != 1) {
            throw std::runtime_error("Invalid data dimensions for Texture2D");
        }

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = textureData.width;
        desc.Height = textureData.height;
        desc.MipLevels = 1u;
        desc.ArraySize = 1u;
        desc.Format = textureData.format;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0u;
        desc.MiscFlags = 0u;

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = textureData.data.data();
        sd.SysMemPitch = textureData.width * (BitsPerPixel(textureData.format) / 8);

        GetDevice(gfx)->CreateTexture2D(&desc, &sd, (ID3D11Texture2D**)pTexture.GetAddressOf());
        CreateShaderResourceView(gfx, textureData.format, D3D11_SRV_DIMENSION_TEXTURE2D, pTexture.Get());
    }

    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override {
        GetContext(gfx)->PSSetShaderResources(start, len, pShaderResourceView.GetAddressOf());
    }
};

class Texture3D : public TextureBase {
public:
    Texture3D(Graphics& gfx, const TextureData& textureData) {
        if (textureData.depth < 1) {
            throw std::runtime_error("Invalid data dimensions for Texture3D");
        }

        D3D11_TEXTURE3D_DESC desc = {};
        desc.Width = textureData.width;
        desc.Height = textureData.height;
        desc.Depth = textureData.depth;
        desc.MipLevels = 1u;
        desc.Format = textureData.format;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0u;
        desc.MiscFlags = 0u;

        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = textureData.data.data();
        sd.SysMemPitch = textureData.width * (BitsPerPixel(textureData.format) / 8);
        sd.SysMemSlicePitch = sd.SysMemPitch * textureData.height;

        GetDevice(gfx)->CreateTexture3D(&desc, &sd, (ID3D11Texture3D**)pTexture.GetAddressOf());
        CreateShaderResourceView(gfx, textureData.format, D3D11_SRV_DIMENSION_TEXTURE3D, pTexture.Get());
    }

    void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override {
        GetContext(gfx)->PSSetShaderResources(start, len, pShaderResourceView.GetAddressOf());
    }
};
