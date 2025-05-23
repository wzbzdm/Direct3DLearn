#include "Texture.h"
#include "ImageLoader.h"

UINT TextureBase::BitsPerPixel(DXGI_FORMAT format) {
    switch (format) {
    case DXGI_FORMAT_R8G8B8A8_UNORM: return 32;
    case DXGI_FORMAT_R32_FLOAT: return 32;
    case DXGI_FORMAT_R16G16_FLOAT: return 32;
    default: throw std::runtime_error("Unsupported DXGI_FORMAT");
    }
}

void TextureBase::CreateShaderResourceView(Graphics& gfx, DXGI_FORMAT format, D3D11_SRV_DIMENSION dimension, void* resource) {
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

Texture1D::Texture1D(Graphics& gfx, const TextureData& textureData) {
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

void Texture1D::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
    GetContext(gfx)->PSSetShaderResources(start, len, pShaderResourceView.GetAddressOf());
}

//Texture2D::Texture2D(Graphics& gfx, const TextureData& textureData) {
//    if (textureData.height < 1 || textureData.depth != 1) {
//        throw std::runtime_error("Invalid data dimensions for Texture2D");
//    }
//
//    D3D11_TEXTURE2D_DESC desc = {};
//    desc.Width = textureData.width;
//    desc.Height = textureData.height;
//    desc.MipLevels = 1u;
//    desc.ArraySize = 1u;
//    desc.Format = textureData.format;
//    desc.SampleDesc.Count = 1;
//    desc.SampleDesc.Quality = 0;
//    desc.Usage = D3D11_USAGE_DEFAULT;
//    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//    desc.CPUAccessFlags = 0u;
//    desc.MiscFlags = 0u;
//
//    D3D11_SUBRESOURCE_DATA sd = {};
//    sd.pSysMem = textureData.data.data();
//    sd.SysMemPitch = textureData.width * (BitsPerPixel(textureData.format) / 8);
//
//    GetDevice(gfx)->CreateTexture2D(&desc, &sd, (ID3D11Texture2D**)pTexture.GetAddressOf());
//    CreateShaderResourceView(gfx, textureData.format, D3D11_SRV_DIMENSION_TEXTURE2D, pTexture.Get());
//}

Texture2D::Texture2D(Graphics& gfx, Shape3DBase& parent)
    : gfx(gfx), parent(parent) { // 初始化为nullptr
    const std::wstring& path = parent.GetTexturePath();
    parent.AddObserver(this);
    if (!path.empty()) {
        InitializeTexture(path);
    }
}

void Texture2D::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
    if (pShaderResourceView) { // 检查资源视图是否存在
        GetContext(gfx)->PSSetShaderResources(start, len, pShaderResourceView.GetAddressOf());
    }
}

void Texture2D::OnUpdate() {
    try {
        const std::wstring& path = parent.GetTexturePath();
        if (path.empty()) {
            // 纹理路径为空时，清除绑定资源
            pShaderResourceView.Reset();
            pTexture.Reset();
            return;
        }
        InitializeTexture(path); // 路径有效时重新加载纹理
    }
    catch (const std::exception& e) {
        // 错误处理（可根据需要记录日志或输出错误信息）
    }
}

ID3D11ShaderResourceView* Texture2D::GetTextureView() const noexcept {
	return pShaderResourceView.Get();
}

void Texture2D::InitializeTexture(const std::wstring& path) {
    TextureData textureData = ImageLoader::Load2D(path);

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

Texture3D::Texture3D(Graphics& gfx, const TextureData& textureData) {
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

void Texture3D::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
    GetContext(gfx)->PSSetShaderResources(start, len, pShaderResourceView.GetAddressOf());
}