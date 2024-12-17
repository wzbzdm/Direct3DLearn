#include "RightMenuManager.h"

void MENUHANDLERNONE() {
    return;
}

RightManager::MenuItemData RightManager::MENUITEMDATANULL = { (MenuItemHandler)MENUHANDLERNONE, RightManager::HANDLER_NONE };

RightManager::RightManager(HWND hwnd) : hwnd(hwnd) {
    rightPopMenu = CreatePopupMenu();
    rightMenuType = RigthMenuInit;
    style.xinterval = 0;
    id = IDBASE;
}

void RightManager::Init(HWND hwnd) {
    this->hwnd = hwnd;
}

void RightManager::DestroyRightMenuM() {
    DestroyMenu(rightPopMenu);
    rightPopMenu = NULL;
    rightMenuData.clear();
}

void RightManager::ClearRightMenu() {
    id = IDBASE;
    if (rightPopMenu) {
        // 删除现有菜单项
        DestroyMenu(rightPopMenu);
        rightPopMenu = CreatePopupMenu();

        rightMenuData.clear();
    }
    else {
        // 如果菜单句柄为NULL，创建新的菜单
        rightPopMenu = CreatePopupMenu();
    }
}

void RightManager::AddMenuItem(const wchar_t* text, MenuItemData& data) {
    MENUITEMINFO itemInfo;
    ZeroMemory(&itemInfo, sizeof(MENUITEMINFO));
    itemInfo.cbSize = sizeof(MENUITEMINFO);
    itemInfo.fMask = MIIM_DATA;
    size_t index = rightMenuData.size();
    itemInfo.dwItemData = (ULONG_PTR)index;

    rightMenuData.push_back(data);
    AppendMenu(rightPopMenu, MF_STRING, id, text);
    int s = SetMenuItemInfo(rightPopMenu, id++, FALSE, &itemInfo);
}

void RightManager::ShowMenu(const POINT& pt, RigthMenuType type) {
    POINT p = pt;
    ClientToScreen(hwnd, &p);
    if (rightMenuType != type) {
        InitRightMenu(type);
    }
    CustomTrackPopupMenu(p);
}

void RightManager::InitRightMenu(RigthMenuType type) {
    switch (type) {
    case RightMenuNone:
    {
        InitRightMenuNone();
        break;
    }
    }
}

RightManager::MenuItemData& RightManager::GetMenuItemData(UINT item) {
    MENUITEMINFO itemInfo;
    ZeroMemory(&itemInfo, sizeof(MENUITEMINFO));
    itemInfo.cbSize = sizeof(MENUITEMINFO);
    itemInfo.fMask = MIIM_DATA;
    if (GetMenuItemInfo(rightPopMenu, LOWORD(item), FALSE, &itemInfo)) {
        int index = (int)itemInfo.dwItemData;
        return rightMenuData[index];
    }

    return MENUITEMDATANULL;
}

void RightManager::InitMenuStyle(RightMenuStyle& style, int xinterval) {
    style.xinterval = xinterval;
}

void MenuTest(HWND hwnd) {
    MessageBox(hwnd, L"测试", L"测试", MB_OK);
}

void RightManager::InitRightMenuNone() {
    InitMenuStyle(style, 0);

    MenuItemData data;
    data.handler = (MenuItemHandler)&MenuTest;
    data.type = HANDLER_HWND;

    AddMenuItem(L"测试", data);
}

void RightManager::CustomTrackPopupMenu(const POINT& pt) {
    TrackPopupMenu(rightPopMenu, TPM_RIGHTBUTTON, pt.x + style.xinterval, pt.y, 0, hwnd, NULL);
}