#include "TextureGenerators.h"

TextureGenerator1D::TextureGenerator1D(size_t width, uint32_t colorStart, uint32_t colorEnd)
    : width(width), colorStart(colorStart), colorEnd(colorEnd) {}

TextureData TextureGenerator1D::Generate() const {
    TextureData textureData(width);
	textureData.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureData.data.resize(width * 4);

    for (size_t i = 0; i < width; ++i) {
        float t = static_cast<float>(i) / (width - 1);
        uint32_t color = (colorStart & 0xFFFFFF) * (1 - t) + (colorEnd & 0xFFFFFF) * t;
        uint8_t* pixel = &textureData.data[i * 4];
        pixel[0] = (color >> 24) & 0xFF;
        pixel[1] = (color >> 16) & 0xFF;
        pixel[2] = (color >> 8) & 0xFF;
        pixel[3] = color & 0xFF;
    }

    return textureData;
}

TextureGenerator2D::TextureGenerator2D(size_t width, size_t height,
    std::function<void(size_t, size_t, uint8_t*)> pixelGenerator)
    : width(width), height(height), pixelGenerator(pixelGenerator) {}

TextureData TextureGenerator2D::Generate() const {
    TextureData textureData(width, height);
	textureData.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureData.data.resize(width * height * 4);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            uint8_t* pixel = &textureData.data[(y * width + x) * 4];
            if (pixelGenerator) {
                pixelGenerator(x, y, pixel);
            }
            else {
                pixel[0] = 255;
                pixel[1] = 255;
                pixel[2] = 255;
                pixel[3] = 255;
            }
        }
    }

    return textureData;
}

TextureGenerator3D::TextureGenerator3D(size_t width, size_t height, size_t depth,
    std::function<void(size_t, size_t, size_t, uint8_t*)> voxelGenerator)
    : width(width), height(height), depth(depth), voxelGenerator(voxelGenerator) {}

TextureData TextureGenerator3D::Generate() const {
    TextureData textureData(width, height, depth);
	textureData.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureData.data.resize(width * height * depth * 4);

    for (size_t z = 0; z < depth; ++z) {
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                uint8_t* voxel = &textureData.data[(z * width * height + y * width + x) * 4];
                if (voxelGenerator) {
                    voxelGenerator(x, y, z, voxel);
                }
                else {
                    voxel[0] = 255;
                    voxel[1] = 255;
                    voxel[2] = 255;
                    voxel[3] = 255;
                }
            }
        }
    }

    return textureData;
}
