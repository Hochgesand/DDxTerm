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
    for (auto& selectedApplication : selectedApplications)
    {
        selectedApplication->getApplicationHook()->refreshTerminalPosition();
	    if (*selectedApplication->getTerminator() == false)
	    {
            deselectTerm(selectedApplication->getApplicationHook()->getApplicationInformation()->getAppName());
	    }
    }
}

// dd steht nicht für DOPPEL D sondern für DropDown
void ApplicationManager::select_application_for_dd(std::string app_name, unsigned int hotkey, unsigned int modHotkey)
{
    for (auto const element : *openApplications)
    {
        if (element.second == app_name)
        {        	
            selectedApplications.push_back(std::make_shared<ApplicationPositioning>(Application_Hook(element.first, element.second), hotkey, modHotkey));
            notify();
            break;
        }
    }
}

std::shared_ptr<std::map<HWND, std::string>> ApplicationManager::getOpenApps()
{
    return openApplications;
}

void ApplicationManager::eraseSelectedApplication(std::shared_ptr<ApplicationPositioning> element)
{
    selectedApplications.erase(
        std::remove(selectedApplications.begin(), selectedApplications.end(), element),
        selectedApplications.end()
    );
}

void ApplicationManager::deselectTerm(std::string appname)
{
	// When UNHOOKed search for ApplicationPositioning object and terminate it.
	for (auto element : selectedApplications)
	{
		if(element == nullptr)
		{
            eraseSelectedApplication(element);
			continue;
		}
		if (element->getApplicationHook()->getApplicationInformation()->getAppName() == appname)
		{
            element->terminate();
            eraseSelectedApplication(element);
            break;
		}
	}
    notify();
}

void ApplicationManager::deselectTerm()
{
    for (auto element : selectedApplications)
    {
	    if (element == nullptr)
	    {
		    continue;
	    }
        element->dropTerminal();
        element->unfocusApplication();
        element->terminate();
    }
    selectedApplications.clear();
    notify();
}

std::vector<std::shared_ptr<ApplicationPositioning>> ApplicationManager::getHookedApps()
{
    refreshRunningApps();
	std::vector<std::shared_ptr<ApplicationPositioning>> hookedApps = {};
	for (auto const element : selectedApplications)
	{
        hookedApps.push_back(element);
	}
    
	return hookedApps;
}

void ApplicationManager::notify()
{
	for (auto element : _observers)
	{
        element->Update();
	}
}

void ApplicationManager::attach(AppManagerObserver* amo)
{
	_observers.push_back(amo);
}

void ApplicationManager::detach(AppManagerObserver* amo)
{
	_observers.erase(std::remove(_observers.begin(), _observers.end(), amo), _observers.end());
}

