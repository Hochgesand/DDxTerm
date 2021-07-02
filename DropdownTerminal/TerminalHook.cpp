#include "TerminalHook.h"
#include "WindowGrabber.h"
#include <cmath>
#include <chrono>
#include <thread>

Terminal_hook::Terminal_hook(){}

Terminal_hook::Terminal_hook(HWND hwnd, std::string appname)
{
	terminal_ = Window(hwnd, appname);
	
	GetWindowRect(terminal_.get_hwnd(), &terminal_pos);
}

double Terminal_hook::calcdrop(const double x, const double k) const
{
	const double x0 = 0.5;
	return ( 1 / (1 + exp(-k * (x - x0))) );
}

void Terminal_hook::dropTerminal()
{
	getTerminalPosition();
	SetWindowPos(terminal_.get_hwnd(),
		HWND_TOPMOST,
		terminal_pos.left,
		0 - calcRectHeight(),
		terminal_pos.right - terminal_pos.left,
		terminal_pos.bottom - terminal_pos.top,
		NULL);
	
	getTerminalPosition();
	long oldPositionY = terminal_pos.top;
	double y = terminal_pos.top;
	double count = 0.0;
	const double diff = (terminal_pos.top + 30)*-1;

	
	while (y < -31.0)
	{
 		getTerminalPosition();
		y = calcdrop(count, 10) * diff + oldPositionY;

		SetWindowPos(terminal_.get_hwnd(),
			HWND_TOPMOST,
			terminal_pos.left,
			y,
			terminal_pos.right - terminal_pos.left,
			terminal_pos.bottom - terminal_pos.top,
			NULL);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		
		count += 0.05;
	}
	
	isOpen = TRUE;
}

long Terminal_hook::calcRectHeight()
{
	return terminal_pos.bottom - terminal_pos.top;
}

void Terminal_hook::hideTerminal()
{
	getTerminalPosition();
	SetWindowPos(terminal_.get_hwnd(),
		HWND_TOPMOST,
		terminal_pos.left,
		terminal_pos.top,
		terminal_pos.right - terminal_pos.left,
		terminal_pos.bottom - terminal_pos.top,
		NULL);
	
	getTerminalPosition();
	long oldPositionY = terminal_pos.top;
	double y = oldPositionY;
	double count = 0.0;
	const long rectHeight = calcRectHeight();
	const double diff = rectHeight + oldPositionY;


	while (y > rectHeight * -1 + 1)
	{
		getTerminalPosition();
		double calcDropReturn = calcdrop(count, -10);
		y = calcDropReturn * diff - rectHeight;
	
		SetWindowPos(terminal_.get_hwnd(),
			HWND_TOPMOST,
			terminal_pos.left,
			y,
			terminal_pos.right - terminal_pos.left,
			terminal_pos.bottom - terminal_pos.top,
			NULL);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		count += 0.05;
	}

	SetWindowPos(terminal_.get_hwnd(),
		HWND_BOTTOM,
		terminal_pos.left,
		terminal_pos.top,
		terminal_pos.right - terminal_pos.left,
		terminal_pos.bottom - terminal_pos.top,
		NULL);

	isOpen = FALSE;
}


void Terminal_hook::toggleTerminal()
{
	if (isOpen)
	{
		hideTerminal();
	} else
	{
		dropTerminal();
	}
}

void Terminal_hook::getTerminalPosition()
{
	GetWindowRect(terminal_.get_hwnd(), &terminal_pos);
}
