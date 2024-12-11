#pragma once

#include "MyGraphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
	template<class T>
	friend class Shape3D;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	void Draw(Graphics& gfx) const noexcept;
	virtual void Update(float dt) noexcept = 0;
	virtual ~Drawable() = default;
protected:
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
protected:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};