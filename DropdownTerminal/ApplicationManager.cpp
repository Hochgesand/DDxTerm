#include "ApplicationManager.h"

#include <future>
#include <memory>
#include <KeyboardHook.h>
#include <WindowGrabber.h>
#include <algorithm>

ApplicationManager::ApplicationManager()
{
	refreshRunningApps();
}

void ApplicationManager::refreshRunningApps()
{
    openApplications = WindowGrabber::getOpenWindowsApplications();
}

// dd steht nicht für DOPPEL D sondern für DropDown
void ApplicationManager::select_application_for_dd(std::string app_name)
{
    for (auto element : openApplications)
    {
        if (element.second == app_name)
        {        	
            selectedApplications.push_back(std::make_unique<ApplicationPositioning>(Application_Hook(element.first, element.second)));
            selectedApplications.back()->hotkeyHandle = std::async(
                std::launch::async,
                registerHotkeyWithMethod,
                0x42,
                [&] { selectedApplications.back()->toggle_terminal(); },
                selectedApplications.back()->getTerminator()
            );
        }
    }
}

std::map<HWND, std::string>* ApplicationManager::getOpenApps()
{
    return &openApplications;
}

void ApplicationManager::deselectTerm(std::string appname)
{
	for (auto& element : selectedApplications)
	{
		if (element->getApplicationHook()->getApplicationInformation()->getAppName() == appname)
		{
            element->unfocusApplication();
            element->terminate();
            selectedApplications.erase(
                std::remove(selectedApplications.begin(), selectedApplications.end(), element),
                selectedApplications.end()
            );
		}
	}
}

void ApplicationManager::deselectTerm()
{
    for (auto& element : selectedApplications)
    {
        element->unfocusApplication();
        element->terminate();
        selectedApplications.clear();
    }
}
