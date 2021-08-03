#pragma once
#include <memory>
#include "HookedWindow.h"

__declspec(dllexport) void toggleTerminal();

class __declspec(dllexport) Application_Hook
{
private:
	RECT terminalPos{};
	HookedWindow terminal_;
public:
	Application_Hook();
	Application_Hook(HWND, std::string);
	void refreshTerminalPosition();
	long calcRectHeight() const;
	RECT* getApplicationRect();
	HookedWindow* getApplicationInformation();
};