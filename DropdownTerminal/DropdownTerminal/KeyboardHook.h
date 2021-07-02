#pragma once
#include <functional>

#include "Window.h"

void registerHotkeyWithMethod(UINT button, std::function<void()> functionOnHotkey);



