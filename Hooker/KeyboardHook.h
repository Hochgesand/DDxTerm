#pragma once
#include <functional>
#include <Windows.h>

__declspec(dllexport) void registerHotkeyWithMethod(UINT, std::function<void()>, bool* running);