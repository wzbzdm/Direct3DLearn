#pragma once

#pragma once
#include <vector>
#include <cstdint>
#include <d3d11.h>

struct TextureData {
    std::vector<uint8_t> data;  // 纹理数据
    size_t width = 0;           // 宽度
    size_t height = 0;          // 高度（对于 1D，高度为 1）
    size_t depth = 0;           // 深度（对于 1D 和 2D，深度为 1）
    DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN; // 图像格式

    TextureData(size_t w, size_t h = 1, size_t d = 1)
        : width(w), height(h), depth(d) {}

    size_t GetSize() const {
        return data.size();
    }

    bool IsValid() const {
        return !data.empty() && width > 0;
    }
};
