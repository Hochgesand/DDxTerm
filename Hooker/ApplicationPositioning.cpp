#include "ApplicationPositioning.h"
#include "KeyboardHook.h"

#include <thread>

ApplicationPositioning::ApplicationPositioning() = default;

ApplicationPositioning::ApplicationPositioning(Application_Hook application_hook, UINT hotkey, UINT modHotkey)
{
	application_hook_ = (std::make_unique<Application_Hook>(application_hook));
	hotkeyHandle = std::async(
		std::launch::async,
		registerHotkeyWithMethod,
		hotkey,
		modHotkey,
		[&] { toggle_terminal(); },
		getTerminator()
	);
}

double ApplicationPositioning::calcDrop(const double x, const double k)
{
	// https://en.wikipedia.org/wiki/Logistic_function
	// If x0 == 0.5 and k == 10 the function will return values between 0 and 1 as double
	// If k == -10 the function is reversed.
	const double x0 = 0.5;
	return (1 / (1 + exp(-k * (x - x0))));
}

void ApplicationPositioning::dropTerminal()
{
	application_hook_->refreshTerminalPosition();
	const long oldPositionY = application_hook_->getApplicationRect()->top;
	// oldPosition is equal to y in the beginning. OldPos is used for (m * x + b) calculation in the start of the while loop.
	double y = application_hook_->getApplicationRect()->top;
	double count = 0.0;
	const double diff = (application_hook_->getApplicationRect()->top + 30) * -1;

	while (y < -31.0)
	{
		// Essentially m * x + b where b is the old Position where the animation started and the diff is the way to go to drop the Application and x is count.
		y = calcDrop(count, 10) * diff + oldPositionY;

		movAppToPosNoResize(application_hook_->getApplicationRect()->left, static_cast<long>(y));

		// Wait a bit or the animation is too fast
		Sleep(animation_speed);

		// This also impacts the speed of the animation, as seen when calculating y a couple line upwards
		count += 0.05;
	}
	// Grabs focus so you can type INSTANTLY
	SetFocus(application_hook_->getApplicationInformation()->getHwnd());
	// Maybe there is a more elegant solution. If so, make a PR :)
	isOpen = TRUE;
}

// Sets WindowPosition to coordinates in params
void ApplicationPositioning::movAppToPosNoResize(const long x, const long y)
{
	application_hook_->refreshTerminalPosition();
	SetWindowPos(application_hook_->getApplicationInformation()->getHwnd(),
		HWND_TOPMOST,
		x,
		y,
		application_hook_->getApplicationRect()->right - application_hook_->getApplicationRect()->left,
		application_hook_->getApplicationRect()->bottom - application_hook_->getApplicationRect()->top,
		NULL);

	application_hook_->refreshTerminalPosition();
}

void ApplicationPositioning::hideTerminal()
{
	application_hook_->refreshTerminalPosition();
	long oldPositionY = application_hook_->getApplicationRect()->top;
	double y = oldPositionY;
	double count = 0.0;
	const long rectHeight = application_hook_->calcRectHeight() + 50;
	const double diff = rectHeight + oldPositionY + 50;


	while (y > rectHeight * -1 + 1)
	{
		y = calcDrop(count, -10) * diff - (rectHeight + 50);

		movAppToPosNoResize(application_hook_->getApplicationRect()->left, static_cast<int>(y));

		Sleep(animation_speed);

		count += 0.05;
	}

	// When Application hides, it should get out of tha wheeyy
	unfocusApplication();

	isOpen = FALSE;
}


void ApplicationPositioning::toggle_terminal()
{
	if (isOpen)
	{
		hideTerminal();
	}
	else
	{
		dropTerminal();
	}
}

void ApplicationPositioning::unfocusApplication()
{
	application_hook_->refreshTerminalPosition();
	SetWindowPos(application_hook_->getApplicationInformation()->getHwnd(),
		HWND_NOTOPMOST,
		application_hook_->getApplicationRect()->left,
		application_hook_->getApplicationRect()->top,
		application_hook_->getApplicationRect()->right - application_hook_->getApplicationRect()->left,
		application_hook_->getApplicationRect()->bottom - application_hook_->getApplicationRect()->top,
		NULL);
}

void ApplicationPositioning::terminate()
{
	terminator = false;
}

bool* ApplicationPositioning::getTerminator()
{
	return &terminator;
}
