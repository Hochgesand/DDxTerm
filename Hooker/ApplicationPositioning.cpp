#include "ApplicationPositioning.h"
#include "KeyboardHook.h"

#include <thread>

ApplicationPositioning::ApplicationPositioning() = default;

ApplicationPositioning::ApplicationPositioning(Application_Hook application_hook, UINT hotkey, UINT modHotkey)
{
	_applicationHook = (std::make_shared<Application_Hook>(application_hook));
	hotkeyHandle = std::async(
		std::launch::async,
		registerHotkeyWithMethod,
		hotkey,
		modHotkey,
		[&] { toggleTerminal(); },
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
	_applicationHook->refreshTerminalPosition();
	const long oldPositionY = _applicationHook->getApplicationRect()->top;
	// oldPosition is equal to y in the beginning. OldPos is used for (m * x + b) calculation in the start of the while loop.
	double y = _applicationHook->getApplicationRect()->top;
	double count = 0.0;
	const double diff = (_applicationHook->getApplicationRect()->top + 30) * -1;

	while (y < -31.0)
	{
		// Essentially m * x + b where b is the old Position where the animation started and the diff is the way to go to drop the Application and x is count.
		y = calcDrop(count, 10) * diff + oldPositionY;

		movAppToPosNoResize(_applicationHook->getApplicationRect()->left, static_cast<long>(y));

		// Wait a bit or the animation is too fast
		Sleep(animation_speed);

		// This also impacts the speed of the animation, as seen when calculating y a couple line upwards
		count += 0.05;
	}
	// Grabs focus so you can type INSTANTLY
	SetFocus(_applicationHook->getApplicationInformation()->getHwnd());
	// Maybe there is a more elegant solution. If so, make a PR :)
	isOpen = TRUE;
}

// Sets WindowPosition to coordinates in params
void ApplicationPositioning::movAppToPosNoResize(const long x, const long y)
{
	_applicationHook->refreshTerminalPosition();
	SetWindowPos(_applicationHook->getApplicationInformation()->getHwnd(),
		HWND_TOPMOST,
		x,
		y,
		_applicationHook->getApplicationRect()->right - _applicationHook->getApplicationRect()->left,
		_applicationHook->getApplicationRect()->bottom - _applicationHook->getApplicationRect()->top,
		NULL);

	_applicationHook->refreshTerminalPosition();
}

void ApplicationPositioning::hideTerminal()
{
	_applicationHook->refreshTerminalPosition();
	long oldPositionY = _applicationHook->getApplicationRect()->top;
	double y = oldPositionY;
	double count = 0.0;
	const long rectHeight = _applicationHook->calcRectHeight() + 50;
	const double diff = rectHeight + oldPositionY + 50;


	while (y > rectHeight * -1 + 1)
	{
		y = calcDrop(count, -10) * diff - (rectHeight + 50);

		movAppToPosNoResize(_applicationHook->getApplicationRect()->left, static_cast<int>(y));

		Sleep(animation_speed);

		count += 0.05;
	}

	// When Application hides, it should get out of tha wheeyy
	unfocusApplication();

	isOpen = FALSE;
}


void ApplicationPositioning::toggleTerminal()
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
	_applicationHook->refreshTerminalPosition();
	SetWindowPos(_applicationHook->getApplicationInformation()->getHwnd(),
		HWND_NOTOPMOST,
		_applicationHook->getApplicationRect()->left,
		_applicationHook->getApplicationRect()->top,
		_applicationHook->getApplicationRect()->right - _applicationHook->getApplicationRect()->left,
		_applicationHook->getApplicationRect()->bottom - _applicationHook->getApplicationRect()->top,
		NULL);
}

std::shared_ptr<Application_Hook> ApplicationPositioning::getApplicationHook() const
{
	return _applicationHook;
}

void ApplicationPositioning::terminate()
{
	terminator = false;
}

std::shared_ptr<bool> ApplicationPositioning::getTerminator()
{
	return std::make_shared<bool>(terminator);
}
