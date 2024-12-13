#pragma once

#include "MyGraphics.h"
#include <DirectXMath.h>

class Bindable;

struct BindableInfo {
	std::unique_ptr<Bindable> bind;
	unsigned int start;
	unsigned int num;
};

class Drawable
{
	template<class T>
	friend class Shape3D;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx) const noexcept;
	virtual ~Drawable() = default;
protected:
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddBind(std::unique_ptr<Bindable> bind, unsigned int start, unsigned int len) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
private:
	virtual const std::vector<std::unique_ptr<BindableInfo>>& GetStaticBinds() const noexcept = 0;
protected:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<BindableInfo>> binds;
};