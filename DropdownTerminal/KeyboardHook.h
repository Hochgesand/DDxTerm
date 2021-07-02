#pragma once
#include <functional>

#include "Window.h"

void register_hotkey_with_method(UINT button, std::function<void()> functionOnHotkey);



