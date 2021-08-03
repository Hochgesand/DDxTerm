#pragma once

#include <map>
#include <memory>
#include <string>
#include <Windows.h>

class __declspec(dllexport) WindowGrabber
{
public:
	static std::shared_ptr<std::map<HWND, std::string>> getOpenWindowsApplications();
};

