#pragma once
#include <vector>
#include <Windows.h>

#define IDBASE  100

typedef void* MenuItemHandler;

#define ARGS_NONE           0
typedef void (*MenuItemHandlerN)();

#define ARGS_HWND           1
typedef void (*MenuItemHandlerH)(HWND hwnd);

class RightManager {
public:
    typedef enum RigthMenuType {
        RigthMenuInit,
        RightMenuNone,
    } RigthMenuType;
    typedef enum HandlerType {
        HANDLER_NONE = ARGS_NONE,
        HANDLER_HWND = ARGS_HWND,
    } HandlerType;
    typedef struct MenuItemData {
        MenuItemHandler handler;
        HandlerType type;
    } MenuItemData, * MenuItemDataPTR;
    typedef struct RightMenuStyle {
        int xinterval;
    } RightMenuStyle;
    static MenuItemData MENUITEMDATANULL;
    RightManager() = default;
    RightManager(HWND hwnd);
    void Init(HWND hwnd);
    void DestroyRightMenuM();
    void ClearRightMenu();
    void AddMenuItem(const wchar_t* text, MenuItemData& data);
    void ShowMenu(const POINT& pt, RigthMenuType type);
    void InitMenuStyle(RightMenuStyle& style, int xinterval);
    void InitRightMenu(RigthMenuType type);
    void CustomTrackPopupMenu(const POINT& pt);
    MenuItemData& GetMenuItemData(UINT item);
private:
    void InitRightMenuNone();

private:
    int id;
    std::vector<MenuItemData> rightMenuData;
    RigthMenuType rightMenuType;
    RightMenuStyle style;
    HMENU rightPopMenu;
    HWND hwnd;
};