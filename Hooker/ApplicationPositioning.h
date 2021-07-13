#pragma once

#include <future>

#include "ApplicationHook.h"

class __declspec(dllexport) ApplicationPositioning
{
private:
	BOOL isOpen{true};
	const unsigned int animation_speed{ 1 };
	void dropTerminal();
	void hideTerminal();
	static double calcDrop(const double x, const double k);
	std::unique_ptr<Application_Hook> application_hook_;
	bool terminator = true;
public:
	ApplicationPositioning();
	ApplicationPositioning(Application_Hook);
	void toggle_terminal();
	void movAppToPosNoResize(const long x, const long y);
	void unfocusApplication();
	std::future<void> hotkeyHandle;
	Application_Hook* getApplicationHook() const
	{
		return application_hook_.get();
	}

	void terminate();
	bool* getTerminator();
};

