#include "ApplicationHook.h"

Application_Hook::Application_Hook() = default;

Application_Hook::Application_Hook(const HWND hwnd, std::string appname)
{
	terminal_ = HookedWindow(hwnd, appname);
	GetWindowRect(terminal_.get_hwnd(), &terminal_pos_);
}

long Application_Hook::calc_rect_height() const
{
	return terminal_pos_.bottom - terminal_pos_.top;
}

void Application_Hook::refresh_terminal_position()
{
	GetWindowRect(terminal_.get_hwnd(), &terminal_pos_);
}

RECT* Application_Hook::getApplicationRect()
{
	return &terminal_pos_;
}

HookedWindow* Application_Hook::getApplicationInformation()
{
	return &terminal_;
}