#include "WindowGrabber.h"

#include <Windows.h>
#include <iostream>

#include "HookedWindow.h"

std::map<HWND, std::string> open_windows;

BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
	const int length = GetWindowTextLength(hWnd);
    TCHAR* buffer = (TCHAR*) (malloc(1 + length));
    GetWindowText(hWnd, buffer, length + 1);

    // List visible windows with a non-empty title
    if (IsWindowVisible(hWnd) && length != 0) {
        const std::unique_ptr<HookedWindow> foundWindow(new HookedWindow(hWnd, buffer));
        open_windows.insert({ foundWindow->get_hwnd(), foundWindow->get_app_name() });
    }
    free(buffer);
    return TRUE;
}

std::map<HWND, std::string> WindowGrabber::get_open_windows_applications() {
    open_windows = {};
    EnumWindows(enumWindowCallback, NULL);
    return open_windows;
}

