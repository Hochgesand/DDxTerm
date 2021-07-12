#pragma once

#include <map>
#include <string>
#include <Windows.h>

class __declspec(dllexport) WindowGrabber
{
public:
	static std::map<HWND, std::string> get_open_windows_applications();
};

