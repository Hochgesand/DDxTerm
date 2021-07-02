#include "KeyboardHook.h"

#include <iostream>
#include "TerminalHook.h"

void register_hotkey_with_method(UINT button, std::function<void()> functionOnHotkey)
{
	if (RegisterHotKey(
		NULL,
		1,
		MOD_ALT,
		button))
	{
		std::cout << "Hotkey registered";
	}

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			functionOnHotkey();
		}
	}
}