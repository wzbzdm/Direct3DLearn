#pragma once

#include "TextureData.h"
#include <string>

class ImageLoader {
public:
    static TextureData Load1D(const std::string& filePath);
    static TextureData Load2D(const std::string& filePath);
    static TextureData Load3D(const std::vector<std::string>& filePaths);

private:
    static void WICInitialize(); // WIC 初始化方法
};
