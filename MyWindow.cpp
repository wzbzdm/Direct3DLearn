#include "MyWindow.h"
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(int width, int height, const wchar_t* name) : width(width), height(height), name(name) {
	InitGdi();

	// 创建窗口
	hWnd = CreateBaseWindow();

	// 创建 DirexctX 管理类
	pGfx = std::make_unique<Graphics>(hWnd, width, height);

	// 初始化 imgui
	InitIMGUI();

	// 设置定时器
	SetTimer(hWnd, PAINT_TIMER, MS_PER_FRAME, nullptr);

	// 创建初始化环境
	NewEnv();

	// 环境初始化完成, 最后显示窗口
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

void Window::InitIMGUI() {
	// 初始化 IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// 设置 IMGUI 风格
	ImGui::StyleColorsDark();

	// 初始化 Win32 和 DirectX11 后端
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(Gfx().Device(), Gfx().Context());
}

void Window::ShowIMGUI() {
	// 开启新的 IMGUI 帧
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Show3DChoose();

	ShowCameraConf();
	
	ShowLightCof();

	// 渲染 IMGUI 界面
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}



void Window::Show3DChoose() {
	// *********************
	// 工具栏 1：物体选择
	// *********************
	if (ImGui::Begin("Objects Toolbar")) {  // 工具栏窗口的标题
		ImGui::Text("Add an object:");

		// 每个选项作为独立按钮
		if (ImGui::Button("Sphere")) {
			ActiveEnv()->AddShape(std::make_unique<Sphere3D>(Gfx()));
		}
		ImGui::SameLine();  // 使按钮位于同一行

		if (ImGui::Button("Cube")) {
			ActiveEnv()->AddShape(std::make_unique<Hexahedron3D>(Gfx()));
		}
		ImGui::SameLine();

		if (ImGui::Button("Plane")) {
			ActiveEnv()->AddShape(std::make_unique<Plane3D>(Gfx()));
		}
		ImGui::SameLine();

		if (ImGui::Button("Cylinder")) {
			ActiveEnv()->AddShape(std::make_unique<Cylinder3D>(Gfx()));
		}
	}
	ImGui::End();
}

void Window::ShowCameraConf() {
	if (ImGui::Begin("Camera Configuration")) {
		CameraData camera = ActiveEnv()->Camera().GetCameraData();

		// 计算相机的朝向向量
		DirectX::XMVECTOR cameraToTarget = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&camera.target), DirectX::XMLoadFloat3(&camera.position));
		DirectX::XMFLOAT3 direction;
		DirectX::XMStoreFloat3(&direction, cameraToTarget);

		// 默认的方向向量
		DirectX::XMVECTOR forward = DirectX::XMVector3Normalize(cameraToTarget); // 当前朝向
		DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&camera.upVector);           // 当前上向量
		DirectX::XMVECTOR right = DirectX::XMVector3Cross(up, forward);          // 右向量：朝向和上向量的叉积

		ImGui::Text("Position (X, Y, Z):");
		if (ImGui::InputFloat3("Position", &camera.position.x)) {
			ActiveEnv()->Camera().SetPosition(camera.position);
		}

		ImGui::Text("Target (X, Y, Z):");
		if (ImGui::InputFloat3("Target", &camera.target.x)) {
			ActiveEnv()->Camera().SetTarget(camera.target);
		}

		ImGui::Text("Up Vector (X, Y, Z):");
		if (ImGui::InputFloat3("Up Vector", &camera.upVector.x)) {
			ActiveEnv()->Camera().SetUpV(camera.upVector);
		}

		float yaw = 0, pitch = 0, roll = 0;

		// 更新Yaw后，绕局部坐标系旋转
		if (ImGui::SliderFloat("Yaw", &yaw, -DirectX::XM_PI, DirectX::XM_PI)) {
			// 旋转矩阵：绕相机的up轴旋转
			DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationAxis(up, yaw);
			DirectX::XMVECTOR newTarget = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&camera.target), rotationMatrix);
			DirectX::XMStoreFloat3(&camera.target, newTarget);
			ActiveEnv()->Camera().SetTarget(camera.target);
		}

		// 更新Pitch后，绕局部坐标系旋转
		if (ImGui::SliderFloat("Pitch", &pitch, -DirectX::XM_PI / 2, DirectX::XM_PI / 2)) {
			// 绕right（右）轴旋转
			DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationAxis(right, pitch);
			DirectX::XMVECTOR newTarget = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&camera.target), rotationMatrix);
			DirectX::XMStoreFloat3(&camera.target, newTarget);
			ActiveEnv()->Camera().SetTarget(camera.target);
		}

		// 更新Roll后，绕当前的前向（forward）轴旋转
		if (ImGui::SliderFloat("Roll", &roll, -DirectX::XM_PI, DirectX::XM_PI)) {
			// 绕前向轴旋转
			DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationAxis(forward, roll);
			DirectX::XMVECTOR newUp = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&camera.upVector), rotationMatrix);
			DirectX::XMStoreFloat3(&camera.upVector, newUp);
			ActiveEnv()->Camera().SetUpV(camera.upVector);
		}

		// FOV角度
		std::string fov_label = "FOV";
		float fov = camera.fieldOfView / DirectX::XM_PI * 360;
		if (ImGui::SliderFloat(fov_label.c_str(), &fov, 60.0f, 200.0f)) {
			float newF = fov / 360 * DirectX::XM_PI;
			ActiveEnv()->Camera().SetFieldOfView(newF);
		}

		// 远裁面
		std::string far_label = "FarPlane";
		if (ImGui::SliderFloat(far_label.c_str(), &camera.farPlane, 50.0f, 1000.0f)) {
			ActiveEnv()->Camera().SetFarPlane(camera.farPlane);
		}
	}
	ImGui::End();
}

void Window::ShowLightCof() {
	// *********************
	// 工具栏 3：光照配置
	// *********************
	if (ImGui::Begin("Light Configuration")) {
		auto lights = ActiveEnv()->Lights()->GetLightsData();  // 获取所有光源
		ImGui::Text("Lights Count: %d", static_cast<int>(lights.size()));

		// 遍历所有光照信息并显示
		for (size_t i = 0; i < lights.size(); ++i) {
			ImGui::Separator();
			ImGui::Text("Light #%d", static_cast<int>(i + 1));  // 光照编号

			DirectX::XMFLOAT4& position = lights[i].position;
			DirectX::XMFLOAT4& color = lights[i].color;
			float& intensity = lights[i].intensity;
			float& range = lights[i].range;

			// 光照位置编辑
			std::string pos_label = "Position##" + std::to_string(i);
			if (ImGui::InputFloat4(pos_label.c_str(), &position.x)) {
				ActiveEnv()->Lights()->UpdateLight(lights[i], i);
			}

			// 光照颜色编辑
			std::string color_label = "Color##" + std::to_string(i);
			if (ImGui::ColorEdit4(color_label.c_str(), &color.x)) {
				ActiveEnv()->Lights()->UpdateLight(lights[i], i);
			}

			// 光照强度编辑
			std::string intensity_label = "Intensity##" + std::to_string(i);
			if (ImGui::SliderFloat(intensity_label.c_str(), &intensity, 0.0f, 10.0f)) {
				ActiveEnv()->Lights()->UpdateLight(lights[i], i);
			}

			// 光照范围编辑
			std::string range_label = "Range##" + std::to_string(i);
			if (ImGui::SliderFloat(range_label.c_str(), &range, 0.0f, 1000.0f)) {
				ActiveEnv()->Lights()->UpdateLight(lights[i], i);
			}
		}

		ImGui::Separator();  // 窗口分割线

		if (ImGui::Button("Add Light")) {
			ActiveEnv()->Lights()->AddLight();
		}
	}
	ImGui::End();
}

std::optional<Mouse::Event> Window::ReadMouseEvent() noexcept {
	return mouse.Read();
}

void Window::InitGdi() {
	// 初始化 GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

HWND Window::CreateBaseWindow() {
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	HWND hWnd = CreateWindow(
		WindowClass::GetName(), (LPCWSTR)name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetIntsance(), this
	);

	CreateToolbar();

	return hWnd;
}

void Window::CreateToolbar() {
	// 创建工具栏
	toolBar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE | TBSTYLE_FLAT, // 工具栏风格
		0, 0, this->width, 50, this->hWnd, nullptr, WindowClass::GetIntsance(), NULL);

	// 初始化工具栏
	SendMessage(toolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(toolBar, TB_SETPADDING, 0, MAKELPARAM(20, 20));  // 填充
	SendMessage(toolBar, TB_SETBITMAPSIZE, 0, MAKELPARAM(30, 30));
	SendMessage(toolBar, TB_SETBUTTONSIZE, 0, MAKELPARAM(50, 50)); // 设置按钮大小以确保图标居中

	SetWindowPos(toolBar, NULL, 0, 0, this->width, 50, SWP_NOZORDER);  // 设置工具栏高度为45像素，宽度为窗口宽度
}

Window::~Window() {
	Gdiplus::GdiplusShutdown(gdiplusToken);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	DestroyWindow(hWnd);
}

void Window::SetTitle(const wchar_t* title) {
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
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return true;
	}
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
	{
		if (wParam == PAINT_TIMER) {
			Update();
			Draw();
		}
		break;
	}
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
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
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
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnLeftPressed(pt.x, pt.y);
		// bring window to foreground on lclick client region
		SetForegroundWindow(hWnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		break;
	}
	case WM_MBUTTONUP:
	{
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	case WM_SIZE:
	{
		if (wParam != SIZE_MINIMIZED) {
			width = LOWORD(lParam);
			height = HIWORD(lParam);
			Resize(width, height);
		}
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
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

void Window::TestInit() {
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{ 2.0f,PI * 2.0f };
	std::uniform_real_distribution<float> ddist{ 1.0f,PI * 0.5f };
	std::uniform_real_distribution<float> odist{ 1.0f,PI * 0.5f };
	std::uniform_real_distribution<float> rdist{ 3.0f,6.0f };
	ActiveEnv()->AddShape(std::make_unique<Box>(Gfx(), rng, adist, ddist, odist, rdist));
	ActiveEnv()->AddShape(std::make_unique<Sphere3D>(Gfx()));
}

void Window::Update() {
	auto dt = timer.Mark();
	ActiveEnv()->UpdateAll(kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
}

bool Window::HasArea() noexcept {
	return width != 0 && height != 0;
}

void Window::Draw() {
	if (!HasArea()) return;
	Gfx().ClearBuffer(1.0f, 0.5f, 0.5f);
	ActiveEnv()->DrawAll();
	
	ShowIMGUI();

	Gfx().EndFrame();
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
		this->width = width;
		this->height = height;
		Gfx().Resize(width, height);
		ActiveEnv()->Camera().Resize(width, height);
	}
}

void Window::LClick(POINT pt) {

}

void Window::LDClick(POINT pt) {

}

void Window::RClick(POINT pt) {

}

void Window::RDClick(POINT pt) {

}

void Window::LPMove(POINT pt) {

}

void Window::RPMove(POINT pt) {

}

void Window::MPMove(POINT pt) {

}

void Window::WheelDown() {

}

void Window::WheelUp() {

}

Window::WindowClass Window::WindowClass::wndClass;