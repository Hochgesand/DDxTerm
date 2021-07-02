#pragma once
#include <Windows.h>
#include <string>

class Window
{
	HWND hwnd_;
	std::string app_name_;

public:
	Window() {}
	Window(HWND hwnd, std::string app_name)
	{
		hwnd_ = hwnd;
		app_name_ = app_name;
	}

	HWND get_hwnd()
	{
		return hwnd_;
	}

	std::string get_app_name()
	{
		return app_name_;
	}
};

