#include "ApplicationHook.h"

Application_Hook::Application_Hook() = default;

Application_Hook::Application_Hook(const HWND hwnd, std::string appname)
{
	terminal_ = HookedWindow(hwnd, appname);
	GetWindowRect(terminal_.getHwnd(), &terminalPos);
}

long Application_Hook::calcRectHeight() const
{
	return terminalPos.bottom - terminalPos.top;
}

void Application_Hook::refreshTerminalPosition()
{
	GetWindowRect(terminal_.getHwnd(), &terminalPos);
}

RECT* Application_Hook::getApplicationRect()
{
	return &terminalPos;
}

HookedWindow* Application_Hook::getApplicationInformation()
{
	return &terminal_;
}