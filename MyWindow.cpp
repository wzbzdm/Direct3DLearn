#include "MyWindow.h"

Window::Window(int width, int height, const wchar_t* name) : width(width), height(height), name(name) {
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	hWnd = CreateWindow(
		WindowClass::GetName(), (LPCWSTR)name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetIntsance(), this
	);

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(hWnd);

	// 默认环境
	NewEnv();
	Env::Initialize(pGfx.get());	// 初始化环境
}

Window::~Window() {
	DestroyWindow(hWnd);
}

void Window::SetTitle(const char* title) {
	SetWindowText(hWnd, (LPCWSTR)title);
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else {
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		// bring window to foreground on lclick client region
		SetForegroundWindow(hWnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	//case WM_SIZE:
	//{
	//	if (wParam != SIZE_MINIMIZED) {
	//		width = LOWORD(lParam);
	//		height = HIWORD(lParam);
	//		// Resize(width, height);
	//	}
	//	break;
	//}
	return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

Graphics& Window::Gfx() {
	return *pGfx;
}

void Window::SwitchEnv(int index) noexcept {
	if (index >= 0 && index < envs.size()) {
		activeEnv = index;

		// 设置 Camera 和 Light 对象到 Graphics 中
		pGfx->SetCamera(std::shared_ptr<CameraManager>(ActiveEnv()->cameraManager.get())); // 将相机传递给Graphics
		pGfx->SetLight(std::shared_ptr<LightManager>(ActiveEnv()->lightManager.get()));   // 将灯光管理器传递给Graphics
	}
}

void Window::AddEnv(std::unique_ptr<Env> env) noexcept {
	envs.push_back(std::move(env));
}

std::unique_ptr<Env>& Window::ActiveEnv() {
	return envs[activeEnv];
}

void Window::NewEnv() noexcept {
	activeEnv = envs.size();
	envs.push_back(std::make_unique<Env>());
	SwitchEnv(activeEnv);
	Env::Initialize(pGfx.get());
}

void Window::RefreshGlobal() {
	ActiveEnv()->RefreshBind();
}

void Window::TestInit() {
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{ 2.0f,PI * 2.0f };
	std::uniform_real_distribution<float> ddist{ 1.0f,PI * 0.5f };
	std::uniform_real_distribution<float> odist{ 1.0f,PI * 0.5f };
	std::uniform_real_distribution<float> rdist{ 3.0f,6.0f };
	ActiveEnv()->AddShape(std::make_unique<Box>(Gfx(), rng, adist, ddist, odist, rdist));
}

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetIntsance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(GetName(), GetIntsance());
}

const wchar_t* Window::WindowClass::GetName() noexcept {
	return L"MyWindow";
}

HINSTANCE Window::WindowClass::GetIntsance() noexcept {
	return wndClass.hInst;
}

void Window::Resize(int width, int height) noexcept {
	if (pGfx != nullptr) {
		Gfx().Resize(width, height);
		ActiveEnv()->Camera().Resize(width, height);
	}
	
}

Window::WindowClass Window::WindowClass::wndClass;