#pragma once

#include <map>
#include <string>
#include <Windows.h>

class __declspec(dllexport) WindowGrabber
{
public:
	static std::map<HWND, std::string> getOpenWindowsApplications();
};

