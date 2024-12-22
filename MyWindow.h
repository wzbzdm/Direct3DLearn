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

#define		KEYLISTENER			2
#define		KEYHZ					100
#define		MS_PER_SEC			1000 / KEYHZ

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
	void ShowMaterialEditor();

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
	void KeyEventHandler() noexcept;
	void Update();
	void Draw();
	DirectX::XMFLOAT3 GetCurVector(int x, int y) noexcept;		// 通过屏幕上的点获取当前在3D空间投影屏幕的坐标
	void Resize(int width, int height) noexcept;			// 事件处理程序
	void LClick(Mouse::Event& mevent);
	void LDClick(Mouse::Event& mevent);
	void RClick(Mouse::Event& mevent);
	void RDClick(Mouse::Event& mevent);
	void LPMove(Mouse::Event& mevent);
	void RPMove(Mouse::Event& mevent);
	void MPMove(Mouse::Event& mevent);
	void WheelDown(Mouse::Event& mevent);
	void WheelUp(Mouse::Event& mevent);

	// 其他


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
	bool showActiveWindow = false;

	std::unique_ptr<Graphics> pGfx = nullptr;
	ImGuiIO io;
	ULONG_PTR gdiplusToken;
};

constexpr float PI = 3.14159265f;

// 控制逻辑
// 相机: 
// 上下左右: 键盘按键 ( WSAD/上下左右键 )
// 绕朝向旋转: 键盘按键控制( 空格+SHIFT )
// 运动方向: 鼠标中键按下更新朝向

// 物体控制
// 左键按下: 根据摄像头朝向垂直平面移动
// 右键按下: 物体旋转相关?