#include "ApplicationPositioner.h"
#include "KeyboardHook.h"

#include <thread>

#include "WindowGrabber.h"

ApplicationPositioner::ApplicationPositioner(HWND appHwnd, std::string appName, UINT hotkey, UINT modHotkey)
{
	_applicationHook = std::make_shared<Application_Hook>(appHwnd, appName);
	hookedAppOffset = getHightestPointInCurrentMonitor();
	hotkeys.push_back(modHotkey);
	hotkeys.push_back(hotkey);
	hotkeyHandle = std::async(
		std::launch::async,
		registerHotkeyWithMethod,
		hotkey,
		modHotkey,
		[&] { toggleTerminal(); },
		getTerminator()
	);
}

long ApplicationPositioner::getHightestPointInFullDesktop()
{
	long getHighestMonitor = 10000000;
	auto monitorsRelPos = WindowGrabber::getDesktopsRectRelative();
	for (const auto& monitors_rel_po : *monitorsRelPos)
	{
		if (monitors_rel_po->top < getHighestMonitor)
		{
			getHighestMonitor = monitors_rel_po->top;
		}
	}
	return getHighestMonitor;
}

long ApplicationPositioner::getHightestPointInCurrentMonitor()
{
	long getHighestMonitor = 10000000;
	const auto monitorsRelPos = WindowGrabber::getDesktopsRectRelative();
	_applicationHook->refreshTerminalPosition();
	for (const auto monitors_rel_po : *monitorsRelPos)
	{
		auto appTop = _applicationHook->getApplicationRect()->top;
		auto monitorTop = monitors_rel_po->bottom;
		if (appTop <= monitorTop)
		{
			getHighestMonitor = monitors_rel_po->top;
		}
	}
	return getHighestMonitor;
}


// If x0 == 0.5 and k == 10 the function will return values between 0 and 1 as double
// If k == -10 the function is reversed.
double ApplicationPositioner::calcDrop(const double x, const double k, const double x0)
{
	// https://en.wikipedia.org/wiki/Logistic_function
	return (1 / (1 + exp(-k * (x - x0))));
}

void ApplicationPositioner::dropTerminal()
{
	auto terminalHwnd = _applicationHook->getApplicationInformation()->getHwnd();
	ShowWindow(terminalHwnd, SW_RESTORE);
	_applicationHook->refreshTerminalPosition();
	const long oldPositionY = _applicationHook->getApplicationRect()->top;
	// oldPosition is equal to y in the beginning. OldPos is used for (m * x + b) calculation in the start of the while loop.
	double y = _applicationHook->getApplicationRect()->top;
	double count = 0.0;
	const double diff = (-1 * hookedAppOffset + _applicationHook->getApplicationRect()->top) * -1;

	while (y < -1 + hookedAppOffset)
	{
		// Essentially m * x + b where b is the old Position where the animation started and the diff is the way to go to drop the Application and x is count.
		y = calcDrop(count, 10, 0.5) * diff + oldPositionY;

		movAppToPosNoResize(_applicationHook->getApplicationRect()->left, static_cast<long>(y));

		// Wait a bit or the animation is too fast
		Sleep(animation_speed);

		// This also impacts the speed of the animation, as seen when calculating y a couple line upwards
		count += 0.05;
	}
	// Grabs focus so you can type INSTANTLY
	SetFocus(terminalHwnd);
	// Maybe there is a more elegant solution. If so, make a PR :)
	isOpen = true;
}

void ApplicationPositioner::hideTerminal()
{
	auto terminalHwnd = _applicationHook->getApplicationInformation()->getHwnd();
	_applicationHook->refreshTerminalPosition();
	long oldPositionY = _applicationHook->getApplicationRect()->top;
	double y = oldPositionY;
	double count = 0.0;
	const long rectHeight = _applicationHook->calcRectHeight() - getHightestPointInFullDesktop();
	const double diff = rectHeight + oldPositionY;


	while (y > rectHeight * -1 + 1)
	{
		y = calcDrop(count, -10, 0.5) * diff - (rectHeight + 50);

		movAppToPosNoResize(_applicationHook->getApplicationRect()->left, static_cast<int>(y));

		Sleep(animation_speed);

		count += 0.05;
	}
	ShowWindow(terminalHwnd, SW_HIDE);
	ShowWindow(terminalHwnd, SW_MINIMIZE);

	// When Application hides, it should get out of tha wheeyy
	unfocusApplication();

	isOpen = false;
}

// Sets WindowPosition to coordinates in params
void ApplicationPositioner::movAppToPosNoResize(const long x, const long y) const
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

void ApplicationPositioner::toggleTerminal()
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

void ApplicationPositioner::unfocusApplication() const
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

std::vector<uint32_t> ApplicationPositioner::getSelectedHotkeys() const
{
	return hotkeys;
}

std::shared_ptr<Application_Hook> ApplicationPositioner::getApplicationHook() const
{
	return _applicationHook;
}

void ApplicationPositioner::terminate()
{
	hookedAppOffset = 0;
	dropTerminal();
	unfocusApplication();
	*terminator = false;
}

std::shared_ptr<bool> ApplicationPositioner::getTerminator() const
{
	return terminator;
}
