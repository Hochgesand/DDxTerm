#include "ApplicationPositioning.h"

#include <thread>

ApplicationPositioning::ApplicationPositioning() = default;

ApplicationPositioning::ApplicationPositioning(Application_Hook application_hook)
{
	application_hook_ = (std::make_unique<Application_Hook>(application_hook));
}

double ApplicationPositioning::calc_drop(const double x, const double k)
{
	// https://en.wikipedia.org/wiki/Logistic_function
	// If x0 == 0.5 and k == 10 the function will return values between 0 and 1 as double
	// If k == -10 the function is reversed.
	const double x0 = 0.5;
	return (1 / (1 + exp(-k * (x - x0))));
}

void ApplicationPositioning::drop_terminal()
{
	application_hook_->refresh_terminal_position();
	const long oldPositionY = application_hook_->getApplicationRect()->top;
	// oldPosition is equal to y in the beginning. OldPos is used for (m * x + b) calculation in the start of the while loop.
	double y = application_hook_->getApplicationRect()->top;
	double count = 0.0;
	const double diff = (application_hook_->getApplicationRect()->top + 30) * -1;

	while (y < -31.0)
	{
		// Essentially m * x + b where b is the old Position where the animation started and the diff is the way to go to drop the Application and x is count.
		y = calc_drop(count, 10) * diff + oldPositionY;

		mov_app_to_pos_no_resize_in_focus(application_hook_->getApplicationRect()->left, static_cast<long>(y));

		// Wait a bit or the animation is too fast
		Sleep(animation_speed);

		// This also impacts the speed of the animation, as seen when calculating y a couple line upwards
		count += 0.05;
	}
	// Grabs focus so you can type INSTANTLY
	SetFocus(application_hook_->getApplicationInformation()->get_hwnd());
	// Maybe there is a more elegant solution. If so, make a PR :)
	isOpen = TRUE;
}

// Sets WindowPosition to coordinates in params
void ApplicationPositioning::mov_app_to_pos_no_resize_in_focus(const long x, const long y)
{
	application_hook_->refresh_terminal_position();
	SetWindowPos(application_hook_->getApplicationInformation()->get_hwnd(),
		HWND_TOPMOST,
		x,
		y,
		application_hook_->getApplicationRect()->right - application_hook_->getApplicationRect()->left,
		application_hook_->getApplicationRect()->bottom - application_hook_->getApplicationRect()->top,
		NULL);

	application_hook_->refresh_terminal_position();
}

void ApplicationPositioning::hide_terminal()
{
	application_hook_->refresh_terminal_position();
	long oldPositionY = application_hook_->getApplicationRect()->top;
	double y = oldPositionY;
	double count = 0.0;
	const long rectHeight = application_hook_->calc_rect_height() + 50;
	const double diff = rectHeight + oldPositionY + 50;


	while (y > rectHeight * -1 + 1)
	{
		y = calc_drop(count, -10) * diff - (rectHeight + 50);

		mov_app_to_pos_no_resize_in_focus(application_hook_->getApplicationRect()->left, static_cast<int>(y));

		Sleep(animation_speed);

		count += 0.05;
	}

	// When Application hides, it should get out of tha wheeyy
	unfocus_application();

	isOpen = FALSE;
}


void ApplicationPositioning::toggle_terminal()
{
	if (isOpen)
	{
		hide_terminal();
	}
	else
	{
		drop_terminal();
	}
}

void ApplicationPositioning::unfocus_application()
{
	application_hook_->refresh_terminal_position();
	SetWindowPos(application_hook_->getApplicationInformation()->get_hwnd(),
		HWND_BOTTOM,
		application_hook_->getApplicationRect()->left,
		application_hook_->getApplicationRect()->top,
		application_hook_->getApplicationRect()->right - application_hook_->getApplicationRect()->left,
		application_hook_->getApplicationRect()->bottom - application_hook_->getApplicationRect()->top,
		NULL);
}