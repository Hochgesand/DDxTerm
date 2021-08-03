#pragma once

#include <future>

#include "ApplicationHook.h"

class __declspec(dllexport) ApplicationPositioning
{
private:
	BOOL isOpen{true};
	const unsigned int animation_speed{ 1 };
	static double calcDrop(const double x, const double k);
	std::shared_ptr<Application_Hook> _applicationHook;
	std::vector<uint32_t> hotkeys;
	int hookedAppOffset = -30;
	std::shared_ptr<bool> terminator = std::make_shared<bool>(true);
public:
	ApplicationPositioning();
	ApplicationPositioning(Application_Hook application_hook, UINT hotkey, UINT modHotkey);
	void toggleTerminal();
	void dropTerminal();
	void hideTerminal();
	void movAppToPosNoResize(const long x, const long y) const;
	void unfocusApplication() const;
	std::vector<uint32_t> getSelectedHotkeys() const;
	std::future<void> hotkeyHandle;
	std::shared_ptr<Application_Hook> getApplicationHook() const;

	void terminate();
	std::shared_ptr<bool> getTerminator() const;
};

