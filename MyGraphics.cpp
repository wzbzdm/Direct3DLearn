#include "MyGraphics.h"

Graphics::Graphics(HWND hWnd, int width, int height) {
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferCount = 2;
	scDesc.BufferDesc.Width = width;
	scDesc.BufferDesc.Height = height;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = hWnd;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.Windowed = TRUE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// 使用新的 Flip-model
	scDesc.Flags = 0;

	UINT swapCreateFlags = 0;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scDesc,
		&swapChain,
		&device,
		nullptr,
		&context
	);

	swapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&backBuffer);
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	device->CreateDepthStencilState(&dsDesc, &pDSState);

	context->OMSetDepthStencilState(pDSState.Get(), 1u);

	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	device->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &depthStencilView);

	context->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), depthStencilView.Get());

	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	context->RSSetViewports(1u, &vp);
}

void Graphics::StartFrame() {
	context->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), depthStencilView.Get());
}

void Graphics::EndFrame() {
	swapChain->Present(1u, 0u);
}

void Graphics::SetCamera(std::shared_ptr<CameraManager> camera) noexcept {
	cameras = camera;
}

void Graphics::SetLight(std::shared_ptr<LightManager> light) noexcept {
	lights = light;
}

CameraBuffer Graphics::GetCameraBuffer() noexcept {
	return cameras->GetCurrentCamera().GetCameraBufferData();
}

std::vector<LightBuffer> Graphics::GetLightBuffer() noexcept {
	return lights->GetLightBufferData();
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept {
	const float color[] = { red, green, blue, 1.0f };
	context->ClearRenderTargetView(renderTargetView.Get(), color);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count) noexcept {
	context->DrawIndexed(count, 0u, 0u);
}

DirectX::XMMATRIX Graphics::GetCameraMatrix() const noexcept {
	return cameras->GetCameraMatrix();
}

DirectX::XMMATRIX Graphics::GetProjectionMatrix() const noexcept {
	return cameras->GetProjectionMatrix();
}

void Graphics::DrawTestTriangle(float angle) {
<<<<<<< HEAD
	struct Vertex {
=======
    struct Vertex {
>>>>>>> origin/main
		struct {
			float x;
			float y;
		} pos;
		struct {
			float r;
			float g;
			float b;
			float a;
		} color;
<<<<<<< HEAD
	};

	// 顶点缓冲区
	const Vertex vertices[] = {
=======
    };

	// 顶点缓冲区
    const Vertex vertices[] = {
>>>>>>> origin/main
		{ 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
		{ -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
		{ 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f}
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	ID3D11Buffer* pVertexBuffer;
	device->CreateBuffer(&bd, &sd, &pVertexBuffer);

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

	// 索引缓冲区
	const unsigned short indexs[] = {
		0,1,2,
		2,1,3,
		2,3,4,
		3,1,5
	};

	D3D11_BUFFER_DESC idb = {};
	idb.Usage = D3D11_USAGE_DEFAULT;
	idb.ByteWidth = sizeof(indexs);
	idb.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idb.CPUAccessFlags = 0u;
	idb.MiscFlags = 0u;
	idb.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indexs;
	ID3D11Buffer* pIndexBuffer;
	device->CreateBuffer(&idb, &isd, &pIndexBuffer);

	context->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);

	// 常数缓冲区
	struct ConstantBuffer {
		struct {
			float element[4][4];
		} transformation;
	};

	const ConstantBuffer cb = {
		{
			{
				(3.0f / 4.0f) * std::cos(angle), std::sin(angle), 0.0f, 0.0f,
<<<<<<< HEAD
				(3.0f / 4.0f) * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
=======
				(3.0f / 4.0f) * - std::sin(angle), std::cos(angle), 0.0f, 0.0f,
>>>>>>> origin/main
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			}
		}
	};

	ID3D11Buffer* pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	device->CreateBuffer(&cbd, &csd, &pConstantBuffer);

	context->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);

	// 顶点着色器
	ID3DBlob* pBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	ID3D11VertexShader* pVertexShader;
	device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	context->VSSetShader(pVertexShader, nullptr, 0u);

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		// 位置数据
		{ "Position", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
		// 颜色数据										相对上一个数据的偏移
		{ "Color", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0u }
	};

	ID3D11InputLayout* pInputLayout;
	device->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	context->IASetInputLayout(pInputLayout);

	pBlob->Release();

	// 像素着色器
	ID3D11PixelShader* pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	context->PSSetShader(pPixelShader, nullptr, 0u);

	pBlob->Release();

	context->OMSetRenderTargets(1u, &renderTargetView, nullptr);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp = {};
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
<<<<<<< HEAD
	context->RSSetViewports(1u, &vp);
=======
	context->RSSetViewports(1u, &vp);	
>>>>>>> origin/main

	context->DrawIndexed((UINT)std::size(indexs), 0u, 0u);

	// 回收内存
	pVertexBuffer->Release();
	pIndexBuffer->Release();
	pVertexShader->Release();
	pInputLayout->Release();
	pPixelShader->Release();
}

void Graphics::BindGlobal() const noexcept {
	
	//void BindDefault(Graphics & gfx) noexcept
	//{
	//	// 同时绑定到顶点着色器和像素着色器插槽0
	//	this->AddBind(std::make_unique<CameraCbuf>(gfx), 0, 1);
	//	// 绑定到像素着色器插槽1
	//	this->AddBind(std::make_unique<LightCbuf>(gfx), 1, 1);
	//}
}

void Graphics::Resize(int width, int height) noexcept {
	// 1. 释放旧的资源
	renderTargetView.Reset();
	depthStencilView.Reset();
	pDepthStencil.Reset();
	backBuffer.Reset();

	// 2. 调整交换链大小
	swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

	// 3. 重新获取后缓冲区并创建渲染目标视图
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);

	// 4. 创建深度缓冲区和视图
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &depthStencilView);

	// 5. 设置渲染目标和深度缓冲
	context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	// 6. 更新视口
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	context->RSSetViewports(1, &vp);
}

ID3D11Device* Graphics::Device() noexcept {
	return device.Get();
}

ID3D11DeviceContext* Graphics::Context() noexcept {
	return context.Get();
}