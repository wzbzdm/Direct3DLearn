#pragma once

#include "Drawable.h"
#include "Transformable.h"
#include "IndexBuffer.h"

class Shape3DBase : public Drawable, public Transformable {
public:
	Shape3DBase() = default;
};

template<class T>
class Shape3D : public Shape3DBase {
protected:
	static bool IsStaticInitialized() noexcept
	{
		return !staticBinds.empty();
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