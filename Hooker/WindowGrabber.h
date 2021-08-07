#pragma once

#include <memory>
#include <vector>
#include <Windows.h>

#include "HookedWindow.h"

class __declspec(dllexport) WindowGrabber
{
public:
	static std::shared_ptr<std::vector<std::shared_ptr<HookedWindow>>> getOpenWindowsApplications();
	static std::shared_ptr<std::vector<std::shared_ptr<RECT>>> getDesktopsRectRelative();
};

