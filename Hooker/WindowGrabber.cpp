#include "WindowGrabber.h"

#include <Windows.h>
#include <iostream>
#include <map>

std::shared_ptr<std::vector<std::shared_ptr<HookedWindow>>> openWindows = std::make_shared<std::vector<std::shared_ptr<HookedWindow>>>();
std::shared_ptr<std::vector<std::shared_ptr<RECT>>> monitorCoordinates = std::make_shared<std::vector<std::shared_ptr<RECT>>>();

BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
    const int length = GetWindowTextLength(hWnd);
    std::unique_ptr<TCHAR> buffer = std::unique_ptr<TCHAR>(new TCHAR[1 + length]);
    GetWindowText(hWnd, buffer.get(), length + 1);

    // List visible windows with a non-empty title
    if (IsWindowVisible(hWnd) && length != 0) {
        const std::shared_ptr<HookedWindow> foundWindow = std::make_shared<HookedWindow>(HookedWindow(hWnd, buffer.get()));
        openWindows->push_back(foundWindow);
    }
    return TRUE;
}

BOOL CALLBACK Monitorenumproc(
    HMONITOR hmonitor,
    HDC hdc,
    LPRECT lprect,
    LPARAM lparam
) {
	const std::unique_ptr<MONITORINFOEX> monitorinfo = std::make_unique<MONITORINFOEX>();
    GetMonitorInfo(hmonitor, monitorinfo.get());
    monitorCoordinates->push_back(std::make_shared<RECT>(monitorinfo->rcWork));
	
    return true;
}



std::shared_ptr<std::vector<std::shared_ptr<HookedWindow>>> WindowGrabber::getOpenWindowsApplications() {
    openWindows->clear();
    EnumWindows(enumWindowCallback, 0);
    return openWindows;
}

std::shared_ptr<std::vector<std::shared_ptr<RECT>>> WindowGrabber::getDesktopsRectRelative()
{
    monitorCoordinates->clear();
    EnumDisplayMonitors(nullptr, nullptr, Monitorenumproc, 0);
    return monitorCoordinates;
}

