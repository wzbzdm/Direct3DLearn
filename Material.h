#pragma once

#include "Bindable.h"

// 材质

class Material : public Bindable
{
public:
	Material(DirectX::XMFLOAT4 color) noexcept;
	Material(const std::wstring& texturePath) noexcept;
	void Bind(Graphics& gfx) noexcept override;
private:
	DirectX::XMFLOAT4 color;    // 单色材质
	std::wstring texturePath;  // 纹理路径
	bool useTexture;           // 是否使用纹理
};
