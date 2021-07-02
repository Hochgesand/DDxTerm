// DropdownTerminal.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "KeyboardHook.h"
#include "WindowGrabber.h"
#include "TerminalHook.h"

std::unique_ptr<Terminal_hook> terminal_hook;


int main()
{
    WindowGrabber window_grabber;
    for (auto element : window_grabber.get_open_windows_applications())
    {
	    if (element.get_app_name() == "MobaXterm")
	    {
            terminal_hook = std::make_unique<Terminal_hook>(element.get_hwnd(), element.get_app_name());
            registerHotkeyWithMethod(0x42, []{ terminal_hook->toggleTerminal(); });
	    }
    }
    return 0;
}