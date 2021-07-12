#pragma once
#include <functional>
#include <Windows.h>

__declspec(dllexport) void register_hotkey_with_method(UINT, std::function<void()>);
__declspec(dllexport) void check_if_window_moved(HWND, std::function<void()>);