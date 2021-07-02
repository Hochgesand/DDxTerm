#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "Window.h"

class WindowGrabber
{
public:
	static std::vector<Window> get_open_windows_applications();
};

