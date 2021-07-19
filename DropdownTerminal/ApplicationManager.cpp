#include "ApplicationManager.h"

#include <future>
#include <memory>
#include <WindowGrabber.h>
#include <algorithm>

#include "AppManagerObserver.h"

ApplicationManager::ApplicationManager()
{
	refreshRunningApps();
}

void ApplicationManager::refreshRunningApps()
{
    openApplications = WindowGrabber::getOpenWindowsApplications();
    for (auto& selected_application : selectedApplications)
    {
	    if (*selected_application->getTerminator() == false)
	    {
            deselectTerm(selected_application->getApplicationHook()->getApplicationInformation()->getAppName());
	    }
    }
}

// dd steht nicht für DOPPEL D sondern für DropDown
void ApplicationManager::select_application_for_dd(std::string app_name, unsigned int hotkey)
{
    for (auto element : openApplications)
    {
        if (element.second == app_name)
        {        	
            selectedApplications.push_back(std::make_unique<ApplicationPositioning>(Application_Hook(element.first, element.second), hotkey));
            notify();
            break;
        }
    }
}

std::map<HWND, std::string>* ApplicationManager::getOpenApps()
{
    return &openApplications;
}

void ApplicationManager::eraseSelectedApplication(std::unique_ptr<ApplicationPositioning>& element)
{
    selectedApplications.erase(
        std::remove(selectedApplications.begin(), selectedApplications.end(), element),
        selectedApplications.end()
    );
}

void ApplicationManager::deselectTerm(std::string appname)
{
	for (std::unique_ptr<ApplicationPositioning>& element : selectedApplications)
	{
		if(element == nullptr)
		{
            eraseSelectedApplication(element);
			continue;
		}
		if (element->getApplicationHook()->getApplicationInformation()->getAppName() == appname && !*element->getTerminator())
		{
            element->unfocusApplication();
            element->terminate();
            eraseSelectedApplication(element);
		}
	}
    notify();
}

void ApplicationManager::deselectTerm()
{
    for (auto& element : selectedApplications)
    {
        element->unfocusApplication();
        element->terminate();
    }
    selectedApplications.clear();
    notify();
}

std::vector<std::string> ApplicationManager::getHookedApps()
{
    refreshRunningApps();
	std::vector<std::string> hookedApps;
	for (auto& element : selectedApplications)
	{
		hookedApps.push_back(element->getApplicationHook()->getApplicationInformation()->getAppName());
	}
    
	return hookedApps;
}

void ApplicationManager::notify()
{
	for (auto element : observers_)
	{
        element->Update();
	}
}

void ApplicationManager::attach(AppManagerObserver* amo)
{
	observers_.push_back(amo);
}

void ApplicationManager::detach(AppManagerObserver* amo)
{
	observers_.erase(std::remove(observers_.begin(), observers_.end(), amo), observers_.end());
}

