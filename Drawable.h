#pragma once

#include "MyGraphics.h"
#include "UpdateNotifier.h"
#include <DirectXMath.h>
#include <random>

class Bindable;

#define TESTIMG L"TextureImg\\test.png"

struct BindableInfo {
	std::unique_ptr<Bindable> bind;
	unsigned int start;
	unsigned int num;
};

// 定义材质的具体数据结构
struct MaterialProperties {
	DirectX::XMFLOAT4 ambient;  // 环境光反射系数
	DirectX::XMFLOAT4 diffuse;  // 漫反射系数
	DirectX::XMFLOAT4 specular; // 镜面反射系数
	float shininess;            // 高光系数
	float pad[3];			   // 填充字节
};

// 定义配置结构
struct ShapeConfig {
	bool useTexture = false; // 是否使用纹理
	bool pad[15];            // 填充字节
};

#define DEFAULTCOLORBUFSIZE		20

class Drawable
{
	template<class T>
	friend class Shape3D;
public:
	Drawable() = default;

	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx) const noexcept;
	// 获取材质属性
	MaterialProperties GetMaterialProperties() const noexcept;
	// 设置材质属性
	void SetMaterialProperties(const MaterialProperties& properties) noexcept;
	void SetColors(const std::vector<DirectX::XMFLOAT4>& colors) noexcept;
	void SetColor(const DirectX::XMFLOAT4& color, int index);
	void AddColor(const DirectX::XMFLOAT4& color) noexcept;
	void AddColors(const std::vector<DirectX::XMFLOAT4>& colors) noexcept;
	void AddColor(const DirectX::XMFLOAT4& color, int num);
	void SetTexturePath(std::wstring str) noexcept;
	const std::wstring& GetTexturePath() const noexcept;
	void SetSamplerConf(const D3D11_SAMPLER_DESC& conf) noexcept;
	const D3D11_SAMPLER_DESC& GetSamplerConf() const noexcept;
	std::vector<DirectX::XMFLOAT4> GetColors() const noexcept;
	const std::vector<DirectX::XMFLOAT4> GetRealColors() const noexcept;
	static void AdjustColors(std::vector<DirectX::XMFLOAT4>& colors, size_t sized);
	void AddObserver(UpdateNotifier* observer);
	ID3D11ShaderResourceView* GetTextureView() const noexcept;

	// 配置相关
	ShapeConfig GetShapeConfig() const noexcept;
	void SetShapeConfig(const ShapeConfig& conf) noexcept;

	virtual ~Drawable() = default;
protected:
	void NotifyObservers();
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddBind(std::unique_ptr<Bindable> bind, unsigned int start, unsigned int len) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
private:
	virtual const std::vector<std::unique_ptr<BindableInfo>>& GetStaticBinds() const noexcept = 0;
protected:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::wstring texturePath = TESTIMG;
	ShapeConfig conf;
	std::vector<UpdateNotifier*> observers;
	std::vector<std::unique_ptr<BindableInfo>> binds;
	// 采样器配置
	D3D11_SAMPLER_DESC samplerConf = {
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		1,
		D3D11_COMPARISON_ALWAYS,
		{ 0, 0, 0, 0},
		0.0f,
		D3D11_FLOAT32_MAX
	};
	// 颜色数据
	std::vector<DirectX::XMFLOAT4> colors;
	MaterialProperties materialProperties = {
	{ 0.2f, 0.2f, 0.2f, 1.0f }, // 默认环境光反射
	{ 0.8f, 0.8f, 0.8f, 1.0f }, // 默认漫反射
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // 默认镜面反射
	32.0f                       // 默认高光系数  [ 16, 64 ]
	};
};

// 玻璃
static constexpr MaterialProperties MATERIAL_GLASS = {
	{ 0.1f, 0.1f, 0.1f, 1.0f },   // 环境光反射
	{ 0.05f, 0.05f, 0.05f, 1.0f }, // 漫反射
	{ 0.9f, 0.9f, 0.9f, 1.0f },    // 镜面反射
	128.0f                         // 高光系数
};

// 金属
static constexpr MaterialProperties MATERIAL_METAL = {
	{ 0.1f, 0.1f, 0.1f, 1.0f },   // 环境光反射
	{ 0.8f, 0.8f, 0.8f, 1.0f },   // 漫反射
	{ 1.0f, 1.0f, 1.0f, 1.0f },   // 镜面反射
	16.0f                          // 高光系数
};

// 木材
static constexpr MaterialProperties MATERIAL_WOOD = {
	{ 0.2f, 0.2f, 0.2f, 1.0f },   // 环境光反射
	{ 0.7f, 0.5f, 0.3f, 1.0f },   // 漫反射
	{ 0.2f, 0.2f, 0.2f, 1.0f },   // 镜面反射
	16.0f                          // 高光系数
};

// 陶瓷
static constexpr MaterialProperties MATERIAL_CERAMIC = {
	{ 0.1f, 0.1f, 0.1f, 1.0f },   // 环境光反射
	{ 0.7f, 0.7f, 0.7f, 1.0f },   // 漫反射
	{ 0.5f, 0.5f, 0.5f, 1.0f },   // 镜面反射
	32.0f                          // 高光系数
};

// 皮革
static constexpr MaterialProperties MATERIAL_LEATHER = {
	{ 0.2f, 0.2f, 0.2f, 1.0f },   // 环境光反射
	{ 0.6f, 0.4f, 0.3f, 1.0f },   // 漫反射
	{ 0.2f, 0.2f, 0.2f, 1.0f },   // 镜面反射
	8.0f                           // 高光系数
};