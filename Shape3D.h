#pragma once

#include "Drawable.h"
#include "Transformable.h"
#include "BindableBase.h"

class Shape3DBase : public Drawable, public Transformable {
public:
	Shape3DBase() = default;
};

template<class T>
class Shape3D : public Shape3DBase {
private:
	static void BindDefault(Graphics& gfx, Shape3DBase& base) noexcept
	{
		// 同时绑定到顶点着色器和像素着色器插槽0
		base.AddBind(std::make_unique<CameraCbuf>(gfx), 0, 1);
		// 绑定到像素着色器插槽1
		base.AddBind(std::make_unique<LightCbuf>(gfx), 1, 1);
	}

	static void BindCustom(Graphics& gfx, Shape3DBase& base) noexcept
	{
		// 材质, 插槽2
		AddBind(std::make_unique<MaterialCbuf>(gfx, base), 2, 1);
		// 颜色，插槽3
		AddBind(std::make_unique<ColorCbuf>(gfx, base), 3, 1);

		// 世界变换, 顶点插槽1
		AddBind(std::make_unique<TransformCbuf>(gfx, base), 1, 1);
	}

protected:
	static bool IsStaticInitialized() noexcept
	{
		return !staticBinds.empty();
	}

	static void BindAll(Graphics& gfx, Shape3DBase& base) noexcept
	{
		BindDefault(gfx, base);
		BindCustom(gfx, base);
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind, unsigned int start, unsigned int len) noexcept
	{
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		std::unique_ptr<BindableInfo> info = std::make_unique<BindableInfo>();
		info->bind = std::move(bind);
		info->start = start;
		info->num = len;
		staticBinds.push_back(std::move(info));
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept
	{
		AddStaticBind(std::move(bind), 0, 1);
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		std::unique_ptr<BindableInfo> info = std::make_unique<BindableInfo>();
		info->bind = std::move(ibuf);
		staticBinds.push_back(std::move(info));
	}
	void SetIndexFromStatic() noexcept
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b->bind.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}
private:
	const std::vector<std::unique_ptr<BindableInfo>>& GetStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<BindableInfo>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<BindableInfo>> Shape3D<T>::staticBinds;