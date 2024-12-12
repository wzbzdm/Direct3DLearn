#pragma once

#include "DirectXD.h"
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <random>
#include <vector>
#include <memory>
#include <utility>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Graphics {
	friend class Bindable;
	friend class Shape3DBase;
public:
	Graphics(HWND hWnd);
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawIndexed(UINT count) noexcept;
	void RenderObject();
	void DrawTestTriangle(float angle);		//  测试
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
private:
	DirectX::XMMATRIX projection;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Resource* backBuffer = nullptr;
	ID3D11Texture2D* pDepthStencil = nullptr;
	ID3D11DepthStencilState* pDSState = nullptr;
};