#include "MyGraphics.h"

Graphics::Graphics(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferCount = 1;
    scDesc.BufferDesc.Width = 0;
    scDesc.BufferDesc.Height = 0;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 0;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = hWnd;
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;
    scDesc.Windowed = TRUE;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = 0;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scDesc,
        &swapChain,
        &device,
        nullptr,
        &context
    );

	FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
    FAILED(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView));
}

Graphics::~Graphics() {
	if (swapChain != nullptr) {
		swapChain->Release();
	}
	if (context != nullptr) {
		context->Release();
	}
	if (device != nullptr) {
		device->Release();
	}
	if (renderTargetView != nullptr) {
		renderTargetView->Release();
	}
	if (backBuffer != nullptr) {
		backBuffer->Release();
	}
}

void Graphics::EndFrame() {
	swapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept {
	const float color[] = { red, green, blue, 1.0f };
	context->ClearRenderTargetView(renderTargetView, color);
}


HRESULT Graphics::CompileShaderFromFile(
	LPCWSTR srcFile,        // 着色器文件路径
	LPCSTR entryPoint,      // 着色器入口点，例如 "VSMain"
	LPCSTR profile,         // 着色器版本，例如 "vs_5_0"
	ID3DBlob** blobOut      // 输出着色器字节码
) {
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG; // 启用调试标志
#endif

	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		srcFile,            // 着色器文件
		nullptr,            // 宏定义（可选）
		nullptr,            // 包含者接口（可选）
		entryPoint,         // 入口点函数
		profile,            // 着色器模型
		shaderFlags,        // 编译标志
		0,                  // 效率标志
		blobOut,            // 输出着色器字节码
		&errorBlob          // 错误信息（可选）
	);

	if (errorBlob) {
		OutputDebugStringA((char*)errorBlob->GetBufferPointer()); // 输出错误信息
		errorBlob->Release();
	}

	return hr;
}

void Graphics::DrawTestTriangle() {
    struct Vertex {
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
    };

    const Vertex vertices[] = {
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
	vp.Width = 600;
	vp.Height = 400;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 100;
	vp.TopLeftY = 0;
	context->RSSetViewports(1u, &vp);	

	context->DrawIndexed((UINT)std::size(indexs), 0u, 0u);

	// 回收内存
	pVertexBuffer->Release();
	pIndexBuffer->Release();
	pVertexShader->Release();
	pInputLayout->Release();
	pPixelShader->Release();
}