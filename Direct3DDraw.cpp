// Direct3DDraw.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "MyWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Window w1(800, 600, L"Window1");

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        w1.Gfx().DrawTestTriangle();
		w1.Gfx().EndFrame();
    }

    return (int) msg.wParam;
}