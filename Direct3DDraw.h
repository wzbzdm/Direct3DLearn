#pragma once

#include "resource.h"
#include <time.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

HWND hWnd;
IDXGISwapChain* swapChain;
ID3D11Device* device;
ID3D11DeviceContext* context;
ID3D11RenderTargetView* renderTargetView;
ID3D11Texture2D* backBuffer;

void InitD3D(HWND hWnd) {
    // 初始化 Direct3D
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferCount = 1;
    scDesc.BufferDesc.Width = SCREEN_WIDTH;
    scDesc.BufferDesc.Height = SCREEN_HEIGHT;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = hWnd;
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;
    scDesc.Windowed = TRUE;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    device = nullptr;
    context = nullptr;
    swapChain = nullptr;
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

    // 创建渲染目标视图
    renderTargetView = nullptr;
    backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    context->OMSetRenderTargets(1, &renderTargetView, nullptr);

    // 设置视口
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    context->RSSetViewports(1, &viewport);
}

void CleanD3D(void) {
    swapChain->SetFullscreenState(FALSE, NULL);

    // 释放资源
    renderTargetView->Release();
    swapChain->Release();
    context->Release();
    device->Release();
}

void RenderFrame(void) {
    // 清屏
    float clearColor[4] = { 0.2f, 0.3f, 0.4f, 1.0f };
    context->ClearRenderTargetView(renderTargetView, clearColor);

    // 交换前后缓冲区
    swapChain->Present(1, 0);
}

HRESULT CompileShaderFromFile(
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