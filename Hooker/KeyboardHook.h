#pragma once
#include <functional>
#include <Windows.h>

__declspec(dllexport) void register_hotkey_with_method(UINT, std::function<void()>, bool* running);