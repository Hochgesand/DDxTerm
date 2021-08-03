#include "KeyboardHook.h"

#include <iostream>

void registerHotkeyWithMethod(UINT button, UINT modButton, const std::function<void()> functionOnHotkey, std::shared_ptr<bool> running)
{
	if (RegisterHotKey(
		nullptr,
		1,
		modButton,
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
				functionOnHotkey();
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