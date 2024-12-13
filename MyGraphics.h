#pragma once

#include "MyCameraManager.h"
#include "MyLightManager.h"
#include "DirectXD.h"
#include <iostream>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <random>
#include <vector>
#include <memory>
#include <utility>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace wrl = Microsoft::WRL;

class Window;

class Graphics {
	friend class Bindable;
	friend class Shape3DBase;
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	void EndFrame();
	void SetCamera(std::shared_ptr<CameraManager> camera) noexcept;
	void SetLight(std::shared_ptr<LightManager> light) noexcept;
	CameraBuffer GetCameraBuffer() noexcept;
	std::vector<LightBuffer> GetLightBuffer() noexcept;
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawIndexed(UINT count) noexcept;
	DirectX::XMMATRIX GetCameraMatrix() const noexcept;
	void DrawTestTriangle(float angle);		//  测试
private:
	std::shared_ptr<CameraManager> cameras;          // 相机对象
	std::shared_ptr<LightManager> lights;		// 灯光管理对象
	wrl::ComPtr<IDXGISwapChain> swapChain;
	wrl::ComPtr<ID3D11Device> device;
	wrl::ComPtr<ID3D11DeviceContext> context;
	wrl::ComPtr<ID3D11RenderTargetView> renderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView> depthStencilView;
	wrl::ComPtr<ID3D11Resource> backBuffer;
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
};