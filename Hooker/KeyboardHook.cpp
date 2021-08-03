#include "KeyboardHook.h"

#include <iostream>

void registerHotkeyWithMethod(UINT button, const std::function<void()> function_on_hotkey, bool* running)
{
	if (RegisterHotKey(
		nullptr,
		1,
		MOD_ALT,
		button))
	{} else
	{
		*running = false;
		MessageBox(nullptr, "The hotkey is presumably already used by another Program.", "Oopsie Woopsie", MB_OK | MB_ICONERROR);
		return;
	}

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE) != 0)
		{
			if (msg.message == WM_HOTKEY)
			{
				function_on_hotkey();
			}
		}
		if (!(*running))
		{
			break;
		}
		Sleep(50);
	}
	UnregisterHotKey(nullptr, 1);
}