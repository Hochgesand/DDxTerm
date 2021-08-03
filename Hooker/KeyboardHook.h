#pragma once
#include <functional>
#include <Windows.h>

__declspec(dllexport) void registerHotkeyWithMethod(UINT, UINT, std::function<void()>, bool* running);