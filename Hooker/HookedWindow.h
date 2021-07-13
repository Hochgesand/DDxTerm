#pragma once

#include <Windows.h>
#include <string>

class __declspec(dllexport) HookedWindow
{
	HWND hwnd_{};
	std::string app_name_;

public:
	HookedWindow(){};
	HookedWindow(HWND hwnd, std::string app_name)
	{
		hwnd_ = hwnd;
		app_name_ = app_name;
	}

	HWND getHwnd()
	{
		return hwnd_;
	}

	std::string getAppName()
	{
		return app_name_;
	}
};

