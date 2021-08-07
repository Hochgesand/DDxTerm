#include "ApplicationHook.h"

Application_Hook::Application_Hook() = default;

Application_Hook::Application_Hook(const HWND hwnd, std::string appname)
{
	terminal_ = std::make_shared<HookedWindow>(hwnd, appname);
	terminalPos = std::make_shared<RECT>();
	refreshTerminalPosition();
}

long Application_Hook::calcRectHeight() const
{
	return terminalPos->bottom - terminalPos->top;
}

void Application_Hook::refreshTerminalPosition() const
{
	GetWindowRect(terminal_->getHwnd(), terminalPos.get());
}

std::shared_ptr<RECT> Application_Hook::getApplicationRect() const
{
	return terminalPos;
}

std::shared_ptr<HookedWindow> Application_Hook::getApplicationInformation() const
{
	return terminal_;
}
