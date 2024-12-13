#include "ImageLoader.h"
#include <wincodec.h>
#include <stdexcept>
#include <vector>

#pragma comment(lib, "windowscodecs.lib")

void ImageLoader::WICInitialize() {
    static bool initialized = false;
    if (!initialized) {
        CoInitialize(nullptr);
        initialized = true;
    }
}

TextureData ImageLoader::Load1D(const std::wstring& filePath) {
    WICInitialize();

    TextureData textureData(256);
	textureData.format = DXGI_FORMAT_R8G8B8A8_UNORM;

    textureData.data.resize(textureData.width * 4); // RGBA
    for (size_t i = 0; i < textureData.width; ++i) {
        float t = static_cast<float>(i) / (textureData.width - 1);
        textureData.data[i * 4 + 0] = static_cast<uint8_t>(255 * t); // Red
        textureData.data[i * 4 + 1] = static_cast<uint8_t>(255 * (1 - t)); // Green
        textureData.data[i * 4 + 2] = 0; // Blue
        textureData.data[i * 4 + 3] = 255; // Alpha
    }

    return textureData;
}

TextureData ImageLoader::Load2D(const std::wstring& filePath) {
    WICInitialize();

    IWICImagingFactory* pFactory = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;

    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
    if (FAILED(hr)) throw std::runtime_error("Failed to create WIC Imaging Factory");

    hr = pFactory->CreateDecoderFromFilename(filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
    if (FAILED(hr)) throw std::runtime_error("Failed to load image");

    hr = pDecoder->GetFrame(0, &pFrame);
    if (FAILED(hr)) throw std::runtime_error("Failed to get image frame");

    hr = pFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr)) throw std::runtime_error("Failed to create format converter");

    hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) throw std::runtime_error("Failed to initialize format converter");

    UINT width, height;
    pConverter->GetSize(&width, &height);

    // Load data
    TextureData textureData(width, height);
    textureData.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureData.data.resize(width * height * 4);
    pConverter->CopyPixels(nullptr, width * 4, width * height * 4, textureData.data.data());

    // Cleanup
    if (pConverter) pConverter->Release();
    if (pFrame) pFrame->Release();
    if (pDecoder) pDecoder->Release();
    if (pFactory) pFactory->Release();

    return textureData;
}

TextureData ImageLoader::Load3D(const std::vector<std::wstring>& filePaths) {
    WICInitialize();

    if (filePaths.empty()) throw std::runtime_error("No file paths provided for 3D texture");

    size_t width = 0, height = 0;
    std::vector<std::vector<uint8_t>> layers;

    // Load each layer
    for (const auto& filePath : filePaths) {
        TextureData layer = Load2D(filePath);

        if (width == 0 && height == 0) {
            width = layer.width;
            height = layer.height;
        }
        else if (layer.width != width || layer.height != height) {
            throw std::runtime_error("Mismatched layer dimensions in 3D texture");
        }

        layers.push_back(std::move(layer.data));
    }

    // Combine layers into 3D texture
    TextureData textureData(width, height, layers.size());
    textureData.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureData.data.resize(width * height * layers.size() * 4);

    for (size_t z = 0; z < layers.size(); ++z) {
        std::copy(layers[z].begin(), layers[z].end(),
            textureData.data.begin() + z * width * height * 4);
    }

    return textureData;
}

TextureData ImageLoader::Load3D(const std::wstring filePath, int num) {
	WICInitialize();

	std::vector<std::wstring> filePaths;
	for (int i = 0; i < num; ++i) {
		filePaths.push_back(filePath);
	}

	return Load3D(filePaths);
}
