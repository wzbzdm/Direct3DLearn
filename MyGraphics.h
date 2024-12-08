#pragma once

#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Graphics {
public:
	Graphics(HWND hWnd);
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTestTriangle();
	static HRESULT CompileShaderFromFile(
		LPCWSTR srcFile,        // 着色器文件路径
		LPCSTR entryPoint,      // 着色器入口点，例如 "VSMain"
		LPCSTR profile,         // 着色器版本，例如 "vs_5_0"
		ID3DBlob** blobOut      // 输出着色器字节码
	);
private:
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11Texture2D* backBuffer = nullptr;
};