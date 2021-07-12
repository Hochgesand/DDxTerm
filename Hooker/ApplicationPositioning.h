#pragma once

#include <future>

#include "ApplicationHook.h"

class __declspec(dllexport) ApplicationPositioning
{
private:
	BOOL isOpen{};
	const unsigned int animation_speed{ 1 };
	void drop_terminal();
	void hide_terminal();
	static double calc_drop(const double x, const double k);
	std::unique_ptr<Application_Hook> application_hook_;
public:
	ApplicationPositioning();
	ApplicationPositioning(Application_Hook);
	void toggle_terminal();
	void mov_app_to_pos_no_resize_in_focus(const long x, const long y);
	void unfocus_application();
	std::future<void> hotkey_handle;
};

