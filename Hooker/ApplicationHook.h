#pragma once
#include <memory>
#include "HookedWindow.h"

__declspec(dllexport) void toggleTerminal();

class __declspec(dllexport) Application_Hook
{
private:
	std::shared_ptr<RECT> terminalPos;
	std::shared_ptr<HookedWindow> terminal_;
public:
	Application_Hook();
	Application_Hook(HWND, std::string);
	void refreshTerminalPosition() const;
	long calcRectHeight() const;
	std::shared_ptr<RECT> getApplicationRect() const;
	std::shared_ptr<HookedWindow> getApplicationInformation() const;
};