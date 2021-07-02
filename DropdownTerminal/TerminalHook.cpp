#include "TerminalHook.h"
#include "WindowGrabber.h"
#include <cmath>
#include <chrono>
#include <thread>

Terminal_hook::Terminal_hook(){}

Terminal_hook::Terminal_hook(HWND hwnd, std::string appname)
{
	terminal_ = Window(hwnd, appname);
	
	GetWindowRect(terminal_.get_hwnd(), &terminal_pos_);
}

double Terminal_hook::calcdrop(const double x, const double k) const
{
	const double x0 = 0.5;
	return ( 1 / (1 + exp(-k * (x - x0))) );
}

void Terminal_hook::drop_terminal()
{
	get_terminal_position();
	SetWindowPos(terminal_.get_hwnd(),
		HWND_TOPMOST,
		terminal_pos_.left,
		0 - calc_rect_height(),
		terminal_pos_.right - terminal_pos_.left,
		terminal_pos_.bottom - terminal_pos_.top,
		NULL);
	
	get_terminal_position();
	long oldPositionY = terminal_pos_.top;
	double y = terminal_pos_.top;
	double count = 0.0;
	const double diff = (terminal_pos_.top + 30)*-1;

	
	while (y < -31.0)
	{
 		get_terminal_position();
		y = calc_drop(count, 10) * diff + oldPositionY;

		SetWindowPos(terminal_.get_hwnd(),
			HWND_TOPMOST,
			terminal_pos_.left,
			y,
			terminal_pos_.right - terminal_pos_.left,
			terminal_pos_.bottom - terminal_pos_.top,
			NULL);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		
		count += 0.05;
	}
	
	isOpen = TRUE;
}

long Terminal_hook::calc_rect_height()
{
	return terminal_pos_.bottom - terminal_pos_.top;
}

void Terminal_hook::hide_terminal()
{
	get_terminal_position();
	SetWindowPos(terminal_.get_hwnd(),
		HWND_TOPMOST,
		terminal_pos_.left,
		terminal_pos_.top,
		terminal_pos_.right - terminal_pos_.left,
		terminal_pos_.bottom - terminal_pos_.top,
		NULL);
	
	get_terminal_position();
	long oldPositionY = terminal_pos_.top;
	double y = oldPositionY;
	double count = 0.0;
	const long rectHeight = calc_rect_height();
	const double diff = rectHeight + oldPositionY;


	while (y > rectHeight * -1 + 1)
	{
		get_terminal_position();
		double calcDropReturn = calc_drop(count, -10);
		y = calcDropReturn * diff - rectHeight;
	
		SetWindowPos(terminal_.get_hwnd(),
			HWND_TOPMOST,
			terminal_pos_.left,
			y,
			terminal_pos_.right - terminal_pos_.left,
			terminal_pos_.bottom - terminal_pos_.top,
			NULL);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		count += 0.05;
	}

	SetWindowPos(terminal_.get_hwnd(),
		HWND_BOTTOM,
		terminal_pos_.left,
		terminal_pos_.top,
		terminal_pos_.right - terminal_pos_.left,
		terminal_pos_.bottom - terminal_pos_.top,
		NULL);

	isOpen = FALSE;
}


void Terminal_hook::toggle_terminal()
{
	if (isOpen)
	{
		hide_terminal();
	} else
	{
		drop_terminal();
	}
}

void Terminal_hook::get_terminal_position()
{
	GetWindowRect(terminal_.get_hwnd(), &terminal_pos_);
}
