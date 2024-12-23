#include "Drawable.h"
#include "IndexBuffer.h"

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
