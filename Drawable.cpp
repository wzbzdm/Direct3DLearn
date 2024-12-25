#include "Drawable.h"
#include "IndexBuffer.h"
#include "Texture.h"

// 获取材质属性
MaterialProperties Drawable::GetMaterialProperties() const noexcept {
	return materialProperties;
}

// 设置材质属性
void Drawable::SetMaterialProperties(const MaterialProperties& properties) noexcept {
	materialProperties = properties;
}

void Drawable::SetColors(const std::vector<DirectX::XMFLOAT4>& colors) noexcept {
	this->colors = colors;
}

void Drawable::SetColor(const DirectX::XMFLOAT4& color, int index) {
	if (index >= 0 && index < colors.size()) {
		colors[index] = color;
	}
}

void Drawable::AddColor(const DirectX::XMFLOAT4& color) noexcept {
	colors.push_back(color);
}

void Drawable::AddColors(const std::vector<DirectX::XMFLOAT4>& colors) noexcept {
	this->colors.insert(this->colors.end(), colors.begin(), colors.end());
}

void Drawable::AddColor(const DirectX::XMFLOAT4& color, int num) {
	for (int i = 0; i < num; i++) {
		colors.push_back(color);
	}
}

void Drawable::SetTexturePath(std::wstring str) noexcept {
	conf.useTexture = true;
	texturePath = str;
	NotifyObservers();
}

const std::wstring& Drawable::GetTexturePath() const noexcept {
	return texturePath;
}

void Drawable::SetSamplerConf(const D3D11_SAMPLER_DESC& conf) noexcept {
	samplerConf = conf;
	NotifyObservers();
}

const D3D11_SAMPLER_DESC& Drawable::GetSamplerConf() const noexcept {
	return samplerConf;
}

std::vector<DirectX::XMFLOAT4> Drawable::GetColors() const noexcept {
	std::vector<DirectX::XMFLOAT4> tempcolors(colors);  // 创建副本
	Drawable::AdjustColors(tempcolors, DEFAULTCOLORBUFSIZE);  // 调整副本
	return tempcolors;  // 返回调整后的副本
}

const std::vector<DirectX::XMFLOAT4> Drawable::GetRealColors() const noexcept {
	return colors;
}

void Drawable::AdjustColors(std::vector<DirectX::XMFLOAT4>& colors, size_t sized = DEFAULTCOLORBUFSIZE) {
	if (colors.size() < sized) {
		// 初始化随机引擎
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);

		// 填充随机颜色
		while (colors.size() < sized) {
			DirectX::XMFLOAT4 randomColor = {
				dist(gen), // R
				dist(gen), // G
				dist(gen), // B
				1.0f       // A (默认不透明)
			};
			colors.push_back(randomColor);
		}
	}
	else if (colors.size() > sized) {
		// 移除多余的颜色
		colors.resize(sized);
	}
}

void Drawable::AddObserver(UpdateNotifier* observer) {
	observers.push_back(observer);
}

ID3D11ShaderResourceView* Drawable::GetTextureView() const noexcept {
	for (auto& b : binds) {
		if (const auto p = dynamic_cast<Texture2D*>(b->bind.get())) {
			return p->GetTextureView();
		}
	}
}

ShapeConfig Drawable::GetShapeConfig() const noexcept {
	return conf;
}

void Drawable::SetShapeConfig(const ShapeConfig& conf) noexcept {
	this->conf = conf;
}

void Drawable::Draw(Graphics& gfx) const noexcept {
	for (auto& b : binds) {
		b->bind->Bind(gfx, b->start, b->num);
	}
	// 绑定不同类别的静态资源
	for (auto& b : GetStaticBinds())
	{
		b->bind->Bind(gfx, b->start, b->num);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind, unsigned int start, unsigned int len) noexcept {
	// 创建一个 BindableInfo 对象
	std::unique_ptr<BindableInfo> info = std::make_unique<BindableInfo>();
	info->bind = std::move(bind);  // 转移绑定的所有权
	info->start = start;
	info->num = len;

	// 将 info 插入到 vector 中
	binds.push_back(std::move(info));  // 使用 std::move 将 info 的所有权转移到 vector 中
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept {
	// 默认插槽0，数目1
	AddBind(std::move(bind), 0, 1);
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept {
	pIndexBuffer = ibuf.get();
	AddBind(std::move(ibuf), 0, 0);
}

void Drawable::NotifyObservers() {
	for (auto& o : observers) {
		o->OnUpdate();
	}
}
