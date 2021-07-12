#include "KeyboardHook.h"

#include <iostream>

void register_hotkey_with_method(UINT button, const std::function<void()> function_on_hotkey)
{
	if (RegisterHotKey(
		nullptr,
		1,
		MOD_ALT,
		button))
	{} else
	{
		MessageBox(nullptr, "The hotkey is presumably already used by another Program.", "Oopsie Woopsie", MB_OK | MB_ICONERROR);
	}

	MSG msg;

	while (GetMessage(&msg, nullptr, 0u, 0u) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			function_on_hotkey();
		}
	}
}

void check_if_window_moved(HWND hwnd, std::function<void()> function_to_exec)
{
}
