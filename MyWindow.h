#pragma once

#include "MyGraphics.h"
#include "MyMouse.h"
#include "MyKeyboard.h"
#include "DrawUnitBase.h"
#include "MyTimer.h"
#include "MyEnv.h"	
#include <optional>
#include <memory>

class Window {
	friend class Graphics;
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
	void SwitchEnv(int index) noexcept;
	void AddEnv(std::unique_ptr<Env> env) noexcept;
	void NewEnv() noexcept;
	std::unique_ptr<Env>& ActiveEnv();
	void RefreshGlobal();
	void TestInit();

	// 事件处理程序
	void Resize(int width, int height) noexcept;

public:
	Mouse mouse;
	Keyboard kbd;
	std::vector<std::unique_ptr<Env>> envs;
	int activeEnv = 0;
private:
	int width;
	int height;
	HWND hWnd;
	const wchar_t* name;
	Timer timer;

	std::unique_ptr<Graphics> pGfx = nullptr;
};

constexpr float PI = 3.14159265f;