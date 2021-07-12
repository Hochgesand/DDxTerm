#pragma once
#include <memory>
#include "HookedWindow.h"

__declspec(dllexport) void toggleTerminal();

class __declspec(dllexport) Application_Hook
{
private:
	RECT terminal_pos_{};
	HookedWindow terminal_;
public:
	Application_Hook();
	Application_Hook(HWND, std::string);
	void refresh_terminal_position();
	long calc_rect_height() const;
	RECT* getApplicationRect();
	HookedWindow* getApplicationInformation();
};