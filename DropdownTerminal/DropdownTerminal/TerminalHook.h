#pragma once
#include "Window.h"
#include <functional>

void toggleTerminal();

class Terminal_hook
{
private:
	void dropTerminal();
	void hideTerminal();
	BOOL isOpen;
	double calcdrop(const double x, const double k) const;
	long calcRectHeight();
public:
	Terminal_hook();
	Terminal_hook(HWND, std::string);
	RECT terminal_pos;
	Window terminal_;
	void toggleTerminal();
	void getTerminalPosition();
};