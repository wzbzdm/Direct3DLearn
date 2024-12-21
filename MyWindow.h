#pragma once

#include "MyGraphics.h"
#include "MyMouse.h"
#include "MyKeyboard.h"
#include "DrawUnitBase.h"
#include "MyTimer.h"
#include "MyEnv.h"	
#include "imguiBase.h"
#include <optional>
#include <memory>

#define     PAINT_TIMER			1
#define		HZ					90
#define		MS_PER_FRAME		1000 / HZ

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
	void SetTitle(const wchar_t* title);
	static std::optional<int> ProcessMessages() noexcept;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	Graphics& Gfx();

	void InitIMGUI();
	void ShowIMGUI();
	
	void Show3DChoose();
	void ShowCameraConf();
	void ShowLightCof();

	// Event
	std::optional<Mouse::Event> ReadMouseEvent() noexcept;

	void InitGdi();
	HWND CreateBaseWindow();
	bool HasArea() noexcept;

	// Env
	void SwitchEnv(int index) noexcept;
	void AddEnv(std::unique_ptr<Env> env) noexcept;
	void NewEnv() noexcept;
	std::unique_ptr<Env>& ActiveEnv();
	// Test
	void TestInit();
	// Handler
	void Update();
	void Draw();
	DirectX::XMFLOAT3 GetCurVector(int x, int y) noexcept;		// 通过屏幕上的点获取当前在3D空间投影屏幕的坐标
	void Resize(int width, int height) noexcept;			// 事件处理程序
	void LClick(POINT pt);
	void LDClick(POINT pt);
	void RClick(POINT pt);
	void RDClick(POINT pt);
	void LPMove(POINT pt);
	void RPMove(POINT pt);
	void MPMove(POINT pt);
	void WheelDown();
	void WheelUp();

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
	ImGuiIO io;
	ULONG_PTR gdiplusToken;
};

constexpr float PI = 3.14159265f;