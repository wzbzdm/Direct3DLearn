#pragma once
#include "TextureGeneratorBase.h"
#include <functional>

class TextureGenerator1D : public TextureGeneratorBase {
public:
    TextureGenerator1D(size_t width, uint32_t colorStart, uint32_t colorEnd);

    TextureData Generate() const override;

private:
    size_t width;
    uint32_t colorStart;
    uint32_t colorEnd;
};

class TextureGenerator2D : public TextureGeneratorBase {
public:
    TextureGenerator2D(size_t width, size_t height,
        std::function<void(size_t, size_t, uint8_t*)> pixelGenerator = nullptr);

    TextureData Generate() const override;

private:
    size_t width, height;
    std::function<void(size_t, size_t, uint8_t*)> pixelGenerator;
};

class TextureGenerator3D : public TextureGeneratorBase {
public:
    TextureGenerator3D(size_t width, size_t height, size_t depth,
        std::function<void(size_t, size_t, size_t, uint8_t*)> voxelGenerator = nullptr);

    TextureData Generate() const override;

private:
    size_t width, height, depth;
    std::function<void(size_t, size_t, size_t, uint8_t*)> voxelGenerator;
};
