#pragma once

#pragma once
#include "TextureData.h"

class TextureGeneratorBase {
public:
    virtual ~TextureGeneratorBase() = default;
    virtual TextureData Generate() const = 0; // 抽象生成接口
};
