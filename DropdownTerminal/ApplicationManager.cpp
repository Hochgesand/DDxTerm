#include "ApplicationManager.h"

#include <future>
#include <memory>
#include <KeyboardHook.h>
#include <WindowGrabber.h>

ApplicationManager::ApplicationManager()
{
	refreshRunningApps();
}

void ApplicationManager::refreshRunningApps()
{
    openApplications = WindowGrabber::get_open_windows_applications();
}

// dd steht nicht f�r DOPPEL D sondern f�r DropDown
void ApplicationManager::select_application_for_dd(std::string app_name)
{
    terminator = true;
    for (auto element : openApplications)
    {
        if (element.second == app_name)
        {
            selectedApplication = std::make_unique<ApplicationPositioning>(Application_Hook(element.first, element.second));
            selectedApplication->hotkey_handle = std::async(
                std::launch::async,
                register_hotkey_with_method,
                0x42,
                [&] { selectedApplication->toggle_terminal(); },
                &terminator
            );
        }
    }
}

std::map<HWND, std::string> ApplicationManager::getOpenApps()
{
    return std::map<HWND, std::string>(openApplications);
}

void ApplicationManager::deselectTerm()
{
    terminator = false;
    if (selectedApplication != nullptr)
    {
        selectedApplication->unfocus_application();
    }
}