#include <Windows.h>
#include "WindowGrabber.h"

std::vector<Window> open_windows;
BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
    int length = GetWindowTextLength(hWnd);
    TCHAR* buffer = new TCHAR[length + 1];
    GetWindowText(hWnd, buffer, length + 1);

    // List visible windows with a non-empty title
    if (IsWindowVisible(hWnd) && length != 0) {
        const std::unique_ptr<Window> foundWindow(new Window(hWnd, buffer));
        open_windows.push_back(*foundWindow);
    }
    return TRUE;
}

std::vector<Window> WindowGrabber::getOpenWindowsApplicationen() {
    open_windows = {};
    EnumWindows(enumWindowCallback, NULL);
    return open_windows;
}