#include "WindowGrabber.h"

#include <Windows.h>
#include <iostream>

#include "HookedWindow.h"

std::shared_ptr<std::map<HWND, std::string>> openWindows = std::make_shared<std::map<HWND, std::string>>();

BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
	const int length = GetWindowTextLength(hWnd);
    std::unique_ptr<TCHAR> buffer = std::unique_ptr<TCHAR>(new TCHAR[1 + length]);
    GetWindowText(hWnd, buffer.get(), length + 1);

    // List visible windows with a non-empty title
    if (IsWindowVisible(hWnd) && length != 0) {
        const std::shared_ptr<HookedWindow> foundWindow = std::make_shared<HookedWindow>(HookedWindow(hWnd, buffer.get()));
        openWindows->insert({ foundWindow->getHwnd(), foundWindow->getAppName() });
    }
    return TRUE;
}

std::shared_ptr<std::map<HWND, std::string>> WindowGrabber::getOpenWindowsApplications() {
    openWindows->clear();
    EnumWindows(enumWindowCallback, NULL);
    return openWindows;
}

