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
    for (auto& selectedApplication : hookedApplications)
    {
        selectedApplication->getApplicationHook()->refreshTerminalPosition();
	    if (*selectedApplication->getTerminator() == false)
	    {
            deselectTerm(selectedApplication->getApplicationHook()->getApplicationInformation()->getHwnd());
	    }
    }
}

// dd steht nicht für DOPPEL D sondern für DropDown
void ApplicationManager::select_application_for_dd(const HWND appHwnd, unsigned int hotkey, unsigned int modHotkey)
{
    for (auto [fst, snd] : *openApplications)
    {
        if (fst == appHwnd)
        {        	
            hookedApplications.push_back(std::make_shared<ApplicationPositioning>(fst, snd, hotkey, modHotkey));
            notify();
            break;
        }
    }
}

// Gets apps which are opened in general
std::shared_ptr<std::map<HWND, std::string>> ApplicationManager::getOpenApps() const
{
    return openApplications;
}

// Removes a specific Application from hookedApplications. Should only be used by deselectTerm(s)
void ApplicationManager::eraseSelectedApplication(std::shared_ptr<ApplicationPositioning> element)
{
    hookedApplications.erase(
        std::remove(hookedApplications.begin(), hookedApplications.end(), element),
        hookedApplications.end()
    );
}

// When UNHOOKed search for ApplicationPositioning object and terminate it.
// If its a nullpointer it shall be removed.
// When Apps close without a UNHOOK its a nullpointer
void ApplicationManager::deselectTerm(HWND appHwnd)
{
	for (const auto& element : hookedApplications)
	{
		if(element == nullptr)
		{
            eraseSelectedApplication(element);
			continue;
		}
		if (element->getApplicationHook()->getApplicationInformation()->getHwnd() == appHwnd)
		{
            element->terminate();
            eraseSelectedApplication(element);
            break;
		}
	}
    notify();
}

// iterates over all hookedApplications and terminates them
// This behavior is for example used while closing the app
// if it finds nullpointer if just skips them.
void ApplicationManager::deselectTerms()
{
	
    for (auto element : hookedApplications)
    {
	    if (element == nullptr)
	    {
		    continue;
	    }
        element->terminate();
    }
    hookedApplications.clear();
    notify();
}

/// <summary>
/// gets all hooked apps as pointer to ApplicationPositionings
/// </summary>
/// <returns>vector of pointer of ApplicationPositionings</returns>
std::vector<std::shared_ptr<ApplicationPositioning>> ApplicationManager::getHookedApps()
{
    refreshRunningApps();
	std::vector<std::shared_ptr<ApplicationPositioning>> hookedApps = {};
	for (auto const& element : hookedApplications)
	{
        hookedApps.push_back(element);
	}
    
	return hookedApps;
}

// Everything down here is observerpattern
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

