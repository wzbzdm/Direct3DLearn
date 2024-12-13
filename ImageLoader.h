#pragma once

#include "TextureData.h"
#include <string>

#define TESTIMG L"TextureImg\\test.png"

class ImageLoader {
public:
    static TextureData Load1D(const std::wstring& filePath);
    static TextureData Load2D(const std::wstring& filePath);
    static TextureData Load3D(const std::vector<std::wstring>& filePaths);
    static TextureData Load3D(const std::wstring, int num);

private:
    static void WICInitialize(); // WIC 初始化方法
};
