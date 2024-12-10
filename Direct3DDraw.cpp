// Direct3DDraw.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Draw.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Draw draw1;

    MSG msg;

    // 主消息循环:
	// 使用 PeekMessage 代替 GetMessage, 使程序一直运行， 不被阻塞
    while (true) {
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return (int)msg.wParam;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        
    }

    return (int) msg.wParam;
}