#include "MyWindow.h"
#include <gdiplus.h>
#include "ImGuiFileDialog/ImGuiFileDialog.h"
#pragma comment (lib,"Gdiplus.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(int width, int height, const wchar_t* name) : width(width), height(height), name(name) {
	SetDllDirectory(L"DLL");
	InitGdi();

	// 创建窗口
	hWnd = CreateBaseWindow();

	// 创建 DirexctX 管理类
	pGfx = std::make_unique<Graphics>(hWnd, width, height);

	Camera::Init(height, 114.0);

	// 初始化 imgui
	InitIMGUI();

	// 设置定时器
	SetTimer(hWnd, PAINT_TIMER, MS_PER_FRAME, nullptr);
	SetTimer(hWnd, KEYLISTENER, MS_PER_SEC, nullptr);

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

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("source/simkai.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
	io.Fonts->Build(); // 构建字体纹理
	ImGui_ImplDX11_CreateDeviceObjects();
}

void Window::ShowIMGUI() {
	// 开启新的 IMGUI 帧
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	io = ImGui::GetIO();
	Show3DChoose();

	ShowCameraConf();
	
	ShowLightCof();

	ShowMaterialEditor();

	// 渲染 IMGUI 界面
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Window::Show3DChoose() {
	// *********************
	// 工具栏 1：物体选择
	// *********************
	if (ImGui::Begin("3D图形")) {  // 工具栏窗口的标题
		ImGui::Text("添加图形:");

		// 每个选项作为独立按钮
		if (ImGui::Button("球体")) {
			ActiveEnv()->AddShape(std::make_unique<Sphere3D>(Gfx()));
		}
		ImGui::SameLine();  // 使按钮位于同一行

		if (ImGui::Button("立方体")) {
			ActiveEnv()->AddShape(std::make_unique<Hexahedron3D>(Gfx()));
		}
		ImGui::SameLine();

		if (ImGui::Button("平面")) {
			ActiveEnv()->AddShape(std::make_unique<Plane3D>(Gfx()));
		}
		ImGui::SameLine();

		if (ImGui::Button("圆柱")) {
			ActiveEnv()->AddShape(std::make_unique<Cylinder3D>(Gfx()));
		}

		if (ImGui::Button("删除当前物体")) {
			ActiveEnv()->DeleteCurShape();
		}
	}
	ImGui::End();
}

void Window::ShowCameraConf() {
	if (ImGui::Begin("相机")) {
		CameraData camera = ActiveEnv()->Camera().GetCameraData();

		// 计算相机的朝向向量
		DirectX::XMVECTOR cameraToTarget = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&camera.target), DirectX::XMLoadFloat3(&camera.position));
		DirectX::XMFLOAT3 direction;
		DirectX::XMStoreFloat3(&direction, cameraToTarget);

		// 默认的方向向量
		DirectX::XMVECTOR forward = DirectX::XMVector3Normalize(cameraToTarget); // 当前朝向
		DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&camera.upVector);           // 当前上向量
		DirectX::XMVECTOR right = DirectX::XMVector3Cross(up, forward);          // 右向量：朝向和上向量的叉积

		ImGui::Text("位置:");
		if (ImGui::InputFloat3("##Position", &camera.position.x)) {
			ActiveEnv()->Camera().SetPosition(camera.position);
		}

		ImGui::Text("目标:");
		if (ImGui::InputFloat3("##Target", &camera.target.x)) {
			ActiveEnv()->Camera().SetTarget(camera.target);
		}

		ImGui::Text("上向量:");
		if (ImGui::InputFloat3("##Up Vector", &camera.upVector.x)) {
			ActiveEnv()->Camera().SetUpV(camera.upVector);
		}

		float yaw = 0, pitch = 0, roll = 0;
		float speedAngle = 0.01f;

		// 更新Pitch后，绕局部坐标系旋转
		if (ImGui::SliderFloat("Pitch##", &pitch, -5.0f, 5.0f)) {
			ActiveEnv()->Camera().AdjustOrientation(speedAngle * pitch, 0, 0);
		}

		// 更新Yaw后，绕局部坐标系旋转
		if (ImGui::SliderFloat("Yaw##", &yaw, -5.0f, 5.0f)) {
			ActiveEnv()->Camera().AdjustOrientation(0, speedAngle * yaw, 0);
		}

		// 更新Roll后，绕当前的前向（forward）轴旋转
		if (ImGui::SliderFloat("Roll##", &roll, -20.0f, 20.0f)) {
			ActiveEnv()->Camera().AdjustOrientation(0, 0, speedAngle * roll);
		}

		// FOV角度
		std::string fov_label = "FOV视角##";
		float fov = camera.fieldOfView / DirectX::XM_PI * 360;
		if (ImGui::SliderFloat(fov_label.c_str(), &fov, 10.0f, 300.0f)) {
			float newF = fov / 360 * DirectX::XM_PI;
			ActiveEnv()->Camera().SetFieldOfView(newF);
		}

		// 远裁面
		std::string far_label = "远裁面##";
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
	if (ImGui::Begin("光照")) {
		auto lights = ActiveEnv()->Lights()->GetLightsData();  // 获取所有光源
		ImGui::Text("灯光数量: %d", static_cast<int>(lights.size()));

		// 遍历所有光照信息并显示
		for (unsigned int i = 0; i < lights.size(); ++i) {
			ImGui::Separator();
			ImGui::Text("Light #%d", static_cast<int>(i + 1));  // 光照编号

			DirectX::XMFLOAT4& position = lights[i].position;
			DirectX::XMFLOAT4& color = lights[i].color;
			float& intensity = lights[i].intensity;
			float& range = lights[i].range;

			// 光照位置编辑
			std::string pos_label = "位置##" + std::to_string(i);
			if (ImGui::InputFloat4(pos_label.c_str(), &position.x)) {
				ActiveEnv()->Lights()->UpdateLight(lights[i], i);
			}

			// 光照颜色编辑
			std::string color_label = "颜色##" + std::to_string(i);
			if (ImGui::ColorEdit4(color_label.c_str(), &color.x)) {
				ActiveEnv()->Lights()->UpdateLight(lights[i], i);
			}

			// 光照强度编辑
			std::string intensity_label = "光照强度##" + std::to_string(i);
			if (ImGui::SliderFloat(intensity_label.c_str(), &intensity, 0.0f, 10.0f)) {
				ActiveEnv()->Lights()->UpdateLight(lights[i], i);
			}

			//// 光照范围编辑
			//std::string range_label = "Range##" + std::to_string(i);
			//if (ImGui::SliderFloat(range_label.c_str(), &range, 0.0f, 1000.0f)) {
			//	ActiveEnv()->Lights()->UpdateLight(lights[i], i);
			//}
		}

		ImGui::Separator();  // 窗口分割线

		if (ImGui::Button("添加新光照")) {
			ActiveEnv()->Lights()->AddLight();
		}
	}
	ImGui::End();
}

void Window::ShowMaterialEditor() {
	if (showActiveWindow && ActiveEnv()->HasSelect()) {
		std::optional<Shape3DBase*> selected = ActiveEnv()->GetSelectedShape();
		if (!selected.has_value()) return;
		Shape3DBase* selectedObject = selected.value();
		MaterialProperties material = selectedObject->GetMaterialProperties();  // 获取选中物体的材质信息
		ShapeConfig conf = selectedObject->GetShapeConfig();  // 获取选中物体的配置信息
		D3D11_SAMPLER_DESC samplec = selectedObject->GetSamplerConf();

		// 获取变换信息
		DirectX::XMFLOAT3 position = selectedObject->GetPosition();
		DirectX::XMFLOAT3 rotation = selectedObject->GetRotation();
		float radius = selectedObject->GetRadius();

		// 使用ImGui显示一个弹出窗口来编辑材质属性
		// 使用ImGui显示一个弹出窗口来编辑属性
		if (ImGui::Begin("当前物体信息")) {
			// ---- 材质编辑 ----
			if (ImGui::CollapsingHeader("材质", ImGuiTreeNodeFlags_DefaultOpen)) {
				// 环境光
				if (ImGui::ColorEdit4("Ambient", &material.ambient.x)) {
					selectedObject->SetMaterialProperties(material);
				}

				// 漫反射
				if (ImGui::ColorEdit4("Diffuse", &material.diffuse.x)) {
					selectedObject->SetMaterialProperties(material);
				}

				// 镜面反射
				if (ImGui::ColorEdit4("Specular", &material.specular.x)) {
					selectedObject->SetMaterialProperties(material);
				}

				// 高光
				if (ImGui::SliderFloat("Shininess", &material.shininess, 0.0f, 100.0f)) {
					selectedObject->SetMaterialProperties(material);
				}
			}

			// ---- 变换编辑 ----
			if (ImGui::CollapsingHeader("变换", ImGuiTreeNodeFlags_DefaultOpen)) {
				// 位置编辑
				if (ImGui::DragFloat3("Position", &position.x, 0.001f)) {
					selectedObject->SetPosition(position);
				}

				// 旋转编辑
				if (ImGui::DragFloat3("Rotation", &rotation.x, 0.001f, 0, DirectX::XM_2PI)) {
					selectedObject->SetRotation(rotation);
				}

				// 缩放比例编辑
				if (ImGui::SliderFloat("Scale", &radius, 0.1f, 10.0f)) {  // 缩放比例限制在 [0.1, 10.0]
					selectedObject->SetRadius(radius);
				}
			}

			// --- 颜色 ---
			if (ImGui::CollapsingHeader("颜色", ImGuiTreeNodeFlags_DefaultOpen)) {
				std::vector<DirectX::XMFLOAT4> colors = selectedObject->GetRealColors();
				for (unsigned int i = 0; i < colors.size(); ++i) {
					std::string color_label = "Color##" + std::to_string(i);
					if (ImGui::ColorEdit4(color_label.c_str(), &colors[i].x)) {
						selectedObject->SetColor(colors[i], i);
					}
				}
			}

			if (ImGui::CollapsingHeader("纹理", ImGuiTreeNodeFlags_DefaultOpen)) {
				// 当前纹理
				ImGui::Text("当前纹理:");
				ImGui::Image((ImTextureID)selectedObject->GetTextureView(), ImVec2(100, 100));
				// 纹理编辑
				if (ImGui::Button("设置纹理")) {
					IGFD::FileDialogConfig config;
					config.path = ".";
					ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "选择文件", ".png,.jpg", config);
				}
				
				if (ImGui::Checkbox("打开纹理", &conf.useTexture)) {
					selectedObject->SetShapeConfig(conf);
				}
				
				// 显示
				if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
					if (ImGuiFileDialog::Instance()->IsOk()) { // 如果OK，进行操作
						std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
						std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

						std::wstring wFilePathName(filePathName.begin(), filePathName.end());
						std::wstring wFilePath(filePath.begin(), filePath.end());
						// 执行操作
						selectedObject->SetTexturePath(wFilePathName);
					}

					// 关闭对话框
					ImGuiFileDialog::Instance()->Close();
				}

				// 纹理采样设置
				if (ImGui::CollapsingHeader("纹理配置", ImGuiTreeNodeFlags_DefaultOpen)) {
					// 设置UV缩放
					if (ImGui::InputFloat2("纹理缩放", &conf.textureWeight.x)) {
						selectedObject->SetShapeConfig(conf);
					}

					// 纹理偏移
					if (ImGui::InputFloat2("纹理偏移", &conf.textureOff.x)) {
						selectedObject->SetShapeConfig(conf);
					}

					// 纹理过滤方式字典
					std::unordered_map<D3D11_FILTER, std::string> filter_dict = {
						{D3D11_FILTER_MIN_MAG_MIP_POINT, "点过滤"},
						{D3D11_FILTER_MIN_MAG_MIP_LINEAR, "线性过滤"},
						{D3D11_FILTER_ANISOTROPIC, "各向异性过滤"},
						{D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, "最小点 + 线性 MIP 过滤"},
						{D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, "最小 + 线性点 MIP 过滤"},
						{D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, "最小线性 + 点 MIP 过滤"},
						{D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, "最小 + 线性 + 点 MIP 过滤"},
						{D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, "最小线性 + 点 + 线性 MIP 过滤"},
						{D3D11_FILTER_COMPARISON_ANISOTROPIC, "比较各向异性过滤"},
						{D3D11_FILTER_MAXIMUM_ANISOTROPIC, "最大各向异性过滤"},
						{D3D11_FILTER_MINIMUM_ANISOTROPIC, "最小各向异性过滤"},
						// Add more D3D11_FILTER values as needed
					};

					std::vector<const char*> filter_items;
					for (const auto& filter : filter_dict) {
						filter_items.push_back(filter.second.c_str());
					}

					int selectedFilterIndex = -1;

					// 查找当前选择的过滤项的索引
					for (int i = 0; i < filter_items.size(); i++) {
						if (filter_dict.find(samplec.Filter) != filter_dict.end() && filter_dict.at(samplec.Filter) == filter_items[i]) {
							selectedFilterIndex = i;
							break;
						}
					}

					// 显示过滤方式选择框
					if (ImGui::Combo("选择纹理过滤方式", &selectedFilterIndex, filter_items.data(), filter_items.size())) {
						// 当用户选择了新的过滤方式时，根据选择的索引来更新 samplec.Filter 的值
						auto it = filter_dict.begin();
						std::advance(it, selectedFilterIndex); // 通过索引获取对应的过滤方式
						samplec.Filter = it->first;  // 更新 samplec.Filter
					}

					// 设置地址模式
					const char* address_modes[] = {
						"无",                            // 空字符串占位
						"循环模式",                      // D3D11_TEXTURE_ADDRESS_WRAP
						"镜像模式",                      // D3D11_TEXTURE_ADDRESS_MIRROR
						"夹紧模式",                      // D3D11_TEXTURE_ADDRESS_CLAMP
						"边界模式",                      // D3D11_TEXTURE_ADDRESS_BORDER
						"单次镜像模式"                   // D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
					};
					if (ImGui::Combo("地址模式", (int*)&samplec.AddressU, address_modes, IM_ARRAYSIZE(address_modes))) {
						samplec.AddressV = samplec.AddressW = samplec.AddressU;
					}

					// 设置边界颜色（如果需要）
					ImGui::ColorEdit4("边界颜色", samplec.BorderColor);

					// 更新纹理配置
					selectedObject->SetSamplerConf(samplec);
				}
			}
		}

		ImGui::End();  // 结束编辑窗口
	}
}

std::optional<Mouse::Event> Window::ReadMouseEvent() noexcept {
	return mouse.ReadEvent();
}

void Window::InitGdi() {
	// 初始化 GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

HWND Window::CreateBaseWindow() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

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

	return hWnd;
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
		else if (wParam == KEYLISTENER) {
			KeyEventHandler();
		}
		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		// 当Imgui 想要捕获 鼠标事件或键盘事件 时，不再传递给下面的处理程序
		if (io.WantCaptureKeyboard) return true;
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (io.WantCaptureKeyboard) return true;
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		if (io.WantCaptureKeyboard) return true;
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
		// 当Imgui 想要捕获 鼠标事件或键盘事件 时，不再传递给下面的处理程序
		if (io.WantCaptureMouse) return true;
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnLeftPressed(pt.x, pt.y);
		// bring window to foreground on lclick client region
		SetForegroundWindow(hWnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (io.WantCaptureMouse) return true;
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (io.WantCaptureMouse) return true;
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		if (io.WantCaptureMouse) return true;
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		if (io.WantCaptureMouse) return true;
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnMidPressed(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		if (io.WantCaptureMouse) return true;
		const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		mouse.OnMidReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (io.WantCaptureMouse) return true;
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

		// 初始化相机配置
		ActiveEnv()->Camera().Resize(width, height);
	}
}

void Window::AddEnv(std::unique_ptr<Env> env) noexcept {
	envs.push_back(std::move(env));
}

std::unique_ptr<Env>& Window::ActiveEnv() {
	return envs[activeEnv];
}

void Window::NewEnv() noexcept {
	activeEnv = (int)envs.size();
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
	// ActiveEnv()->AddShape(std::make_unique<Sphere3D>(Gfx()));
	// ActiveEnv()->AddShape(std::make_unique<Hexahedron3D>(Gfx()));
	std::unique_ptr<Sphere3D> ts = std::make_unique<Sphere3D>(Gfx());
	ts->SetPosition(DirectX::XMFLOAT3(0, 0, 0.0));
	ts->SetMaterialProperties(MATERIAL_CERAMIC);
	ActiveEnv()->AddShape(std::move(ts));
}

// 相机: 
// 上下左右: 键盘按键 ( WSAD/上下左右键 )
// 绕朝向旋转: 键盘按键控制( 空格+SHIFT )
// 运动方向: 鼠标中键按下更新朝向
void Window::KeyEventHandler() noexcept {
	float speedPos = 0.1f;
	float speedAngle = 0.01f;
	// 修正 
	// 前 W: 0x57
	if (kbd.KeyIsPressed(0x57)) {
		ActiveEnv()->Camera().Move(speedPos, 0, 0);
	}
	// 后 S: 0x53
	if (kbd.KeyIsPressed(0x53)) {
		ActiveEnv()->Camera().Move(-speedPos, 0, 0);
	}
	// 左 A: 0x41
	if (kbd.KeyIsPressed(0x41)) {
		ActiveEnv()->Camera().Move(0, speedPos, 0);
	}
	// 右 D: 0x44
	if (kbd.KeyIsPressed(0x44)) {
		ActiveEnv()->Camera().Move(0, -speedPos, 0);
	}
	// 上 Q: 0x51
	if (kbd.KeyIsPressed(0x51)) {
		ActiveEnv()->Camera().Move(0, 0, speedPos);
	}
	// 下 E: 0x45
	if (kbd.KeyIsPressed(0x45)) {
		ActiveEnv()->Camera().Move(0, 0, -speedPos);
	}
	//  | VK_UP
	if (kbd.KeyIsPressed(VK_UP)) {
		ActiveEnv()->Camera().AdjustOrientation(speedAngle, 0, 0);
	}
	//  | VK_DOWN
	if (kbd.KeyIsPressed(VK_DOWN)) {
		ActiveEnv()->Camera().AdjustOrientation(-speedAngle, 0, 0);
	}
	//  | VK_LEFT
	if (kbd.KeyIsPressed(VK_LEFT)) {
		ActiveEnv()->Camera().AdjustOrientation(0, speedAngle, 0);
	}
	//  | VK_RIGHT
	if (kbd.KeyIsPressed(VK_RIGHT)) {
		ActiveEnv()->Camera().AdjustOrientation(0, -speedAngle, 0);
	}
	// 空格, 旋转
	if (kbd.KeyIsPressed(VK_SPACE)) {
		// 逆时针
		if (kbd.KeyIsPressed(VK_SHIFT)) {
			ActiveEnv()->Camera().AdjustOrientation(0, 0, -speedAngle);
		}
		// 顺时针
		else {
 			ActiveEnv()->Camera().AdjustOrientation(0, 0, speedAngle);
		}
	}

	// 删除
	if (kbd.KeyIsPressed(VK_DELETE) || kbd.KeyIsPressed(VK_BACK)) {
		ActiveEnv()->DeleteCurShape();
	}
}

void Window::Update() {
	auto dt = timer.Mark();
	ActiveEnv()->UpdateAll(dt);
}

bool Window::HasArea() noexcept {
	return width != 0 && height != 0;
}

void Window::Draw() {
	if (!HasArea()) return;
	Gfx().StartFrame();

	Gfx().ClearBuffer(1.0f, 0.5f, 0.5f);
	ActiveEnv()->DrawAll();
	ShowIMGUI();

	Gfx().EndFrame();
}

DirectX::XMFLOAT3 Window::GetCurVector(int x, int y) noexcept {
	// 屏幕坐标到 NDC
	float xNDC = (2.0f * x) / width - 1.0f;
	float yNDC = 1.0f - (2.0f * y) / height;
	return ActiveEnv()->Camera().GetCurDir(xNDC, yNDC);
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

void Window::LClick(Mouse::Event& mevent) {
	POINT pt = { mevent.GetPosX(), mevent.GetPosY() };
	showActiveWindow = false;
	DirectX::XMFLOAT3 direction = GetCurVector(pt.x, pt.y);
	DirectX::XMFLOAT3 origin = ActiveEnv()->Camera().GetPos();
	Ray cur(origin, direction);
	int index = ActiveEnv()->GetClickIndex(cur);
	ActiveEnv()->SetActiveShape(index);
}

void Window::LDClick(Mouse::Event& mevent) {
	POINT pt = { mevent.GetPosX(), mevent.GetPosY() };
	DirectX::XMFLOAT3 direction = GetCurVector(pt.x, pt.y);
	DirectX::XMFLOAT3 origin = ActiveEnv()->Camera().GetPos();
	Ray cur(origin, direction);
	int index = ActiveEnv()->GetClickIndex(cur);
	if (index == ActiveEnv()->GetActiveShapeIndex()) {
		showActiveWindow = true;
	}
}

void Window::RClick(Mouse::Event& mevent) {

}

void Window::RDClick(Mouse::Event& mevent) {

}

// from 为屏幕上的上一个点， to为平面的下一个点，pos为当前选中物体的中心点，求物体应该移动的距离
DirectX::XMFLOAT3 Window::GetCurrentOffInActiveShapePlane(POINT from, POINT to, const DirectX::XMFLOAT3& pos) noexcept {
	// from 射线方向
	DirectX::XMFLOAT3 fromDir = GetCurVector(from.x, from.y);
	// to 射线方向
	DirectX::XMFLOAT3 toDir = GetCurVector(to.x, to.y);
	// 相机方向
	DirectX::XMFLOAT3 cameraDir = ActiveEnv()->Camera().GetDirection();
	// 相机位置
	DirectX::XMFLOAT3 origin = ActiveEnv()->Camera().GetPos();

	// 平面的法向量是相机的方向
	DirectX::XMFLOAT3 planeNormal = cameraDir;

	// 计算相机到平面的距离，平面方程：ax + by + cz = d，其中 (a, b, c) 是法向量
	float d = planeNormal.x * pos.x + planeNormal.y * pos.y + planeNormal.z * pos.z;

	// 计算射线 fromDir 与平面的交点
	DirectX::XMFLOAT3 fromJ;
	float t_from = (d - (planeNormal.x * origin.x + planeNormal.y * origin.y + planeNormal.z * origin.z)) /
		(planeNormal.x * fromDir.x + planeNormal.y * fromDir.y + planeNormal.z * fromDir.z);
	fromJ.x = origin.x + t_from * fromDir.x;
	fromJ.y = origin.y + t_from * fromDir.y;
	fromJ.z = origin.z + t_from * fromDir.z;

	// 计算射线 toDir 与平面的交点
	DirectX::XMFLOAT3 toJ;
	float t_to = (d - (planeNormal.x * origin.x + planeNormal.y * origin.y + planeNormal.z * origin.z)) /
		(planeNormal.x * toDir.x + planeNormal.y * toDir.y + planeNormal.z * toDir.z);
	toJ.x = origin.x + t_to * toDir.x;
	toJ.y = origin.y + t_to * toDir.y;
	toJ.z = origin.z + t_to * toDir.z;

	// 计算偏移量，平面上的两个交点 toJ - fromJ
	DirectX::XMFLOAT3 offset = { toJ.x - fromJ.x, toJ.y - fromJ.y, toJ.z - fromJ.z };

	return offset;
}

// 左键按下移动，控制当前选中物体移动
void Window::LPMove(Mouse::Event& mevent) {
	if (ActiveEnv()->HasSelect()) {
		std::optional<Shape3DBase*> selected = ActiveEnv()->GetSelectedShape();
		if (!selected.has_value()) return;
		Shape3DBase* select = selected.value();
		POINT nowp = { mevent.GetPosX(), mevent.GetPosY() };
		POINT lastp = { mevent.GetPosX() - mevent.GetOffX(), mevent.GetPosY() - mevent.GetOffY() };
		// 移动
		// 通过两个点的射线与当前视线方向的垂直的过当前选中物体的pos的平面的两个交点的偏移
		DirectX::XMFLOAT3 off = GetCurrentOffInActiveShapePlane(lastp, nowp, select->GetPosition());
		select->Translate(off);
	}
}

void Window::RPMove(Mouse::Event& mevent) {
	if (ActiveEnv()->HasSelect()) {
		auto selected = ActiveEnv()->GetSelectedShape();
		if (!selected.has_value()) return;

		Shape3DBase* select = selected.value();
		POINT nowp = { mevent.GetPosX(), mevent.GetPosY() };
		POINT lastp = { mevent.GetPosX() - mevent.GetOffX(), mevent.GetPosY() - mevent.GetOffY() };

		// 移动偏移的计算
		DirectX::XMFLOAT3 off = GetCurrentOffInActiveShapePlane(lastp, nowp, select->GetPosition());
		if (off.x == 0 && off.y == 0 && off.z == 0) return;

		float lenf = std::sqrtf(off.x * off.x + off.y * off.y + off.z * off.z);
		if (lenf < 1e-6f) return; // 避免计算非常小的角度导致的问题

		// 计算旋转轴（用相机视线方向和偏移方向的叉积表示）
		DirectX::XMFLOAT3 czvector = Camera::CrossProduct(ActiveEnv()->Camera().GetDirection(), off);
		float axisLength = std::sqrtf(czvector.x * czvector.x + czvector.y * czvector.y + czvector.z * czvector.z);
		if (axisLength < 1e-6f) return; // 旋转轴长度接近零时，忽略操作

		// 规范化旋转轴
		czvector.x /= axisLength;
		czvector.y /= axisLength;
		czvector.z /= axisLength;

		// 当前物体的旋转（四元数表示）
		DirectX::XMFLOAT4 currentQuat = select->GetRotationQuaternion();

		// 计算旋转四元数（表示新的旋转）
		DirectX::XMVECTOR axis = DirectX::XMLoadFloat3(&czvector);
		DirectX::XMVECTOR quatRot = DirectX::XMQuaternionRotationAxis(axis, -lenf); // 使用旋转轴和角度计算四元数
		DirectX::XMVECTOR quatCurrent = DirectX::XMLoadFloat4(&currentQuat);

		// 组合旋转（四元数相乘）
		DirectX::XMVECTOR quatNew = DirectX::XMQuaternionMultiply(quatRot, quatCurrent);

		// 将旋转结果存储回四元数
		DirectX::XMFLOAT4 newQuat;
		DirectX::XMStoreFloat4(&newQuat, quatNew);

		// 更新物体的旋转
		select->SetRotationQuaternion(newQuat);
	}
}

// TODO: 根据两次位置调整偏移角度
void Window::MPMove(Mouse::Event& mevent) {
	POINT pt = { mevent.GetPosX(), mevent.GetPosY() };
	POINT off = { mevent.GetOffX(), mevent.GetOffY() };
	// 中键按下调整角度
	float speedAngle = 0.003f;
	float speedLR = -off.x * speedAngle;
	float speedTB = off.y * speedAngle;
	ActiveEnv()->Camera().AdjustOrientation(speedTB, speedLR, 0);
}
   
void Window::WheelDown(Mouse::Event& mevent) {
	std::optional<Shape3DBase*> selected = ActiveEnv()->GetSelectedShape();
	if (!selected.has_value()) return;
	Shape3DBase* selectedObject = selected.value();
	float speed = 0.1f;
	selectedObject->Zoom(1 - speed);
}

void Window::WheelUp(Mouse::Event& mevent) {
	std::optional<Shape3DBase*> selected = ActiveEnv()->GetSelectedShape();
	if (!selected.has_value()) return;
	Shape3DBase* selectedObject = selected.value();
	float speed = 0.1f;
	selectedObject->Zoom(1 + speed);
}

Window::WindowClass Window::WindowClass::wndClass;