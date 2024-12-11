#pragma once

#include "MyGraphics.h"
#include "MyMouse.h"
#include <optional>
#include <memory>

class Window {
private:
	class WindowClass {
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetIntsance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		HINSTANCE hInst;
		static WindowClass wndClass;
	};
public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	void SetTitle(const char* title);
	static std::optional<int> ProcessMessages() noexcept;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	Graphics& Gfx();
public:
	Mouse mouse;
private:
	int width;
	int height;
	HWND hWnd;
	const wchar_t* name;

	std::unique_ptr<Graphics> pGfx;
};