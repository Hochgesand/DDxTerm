#pragma once
#include <map>
#include <string>
#include <ApplicationPositioning.h>

class ApplicationManager
{
    std::vector<class AppManagerObserver*> _observers;
    std::shared_ptr<std::map<HWND, std::string>> openApplications;
    std::vector<std::shared_ptr<ApplicationPositioning>> hookedApplications;
    void eraseSelectedApplication(std::shared_ptr<ApplicationPositioning>);

public:
    ApplicationManager();
    void refreshRunningApps();
    void select_application_for_dd(std::string, unsigned int hotkey, unsigned int modHotkey);
    void deselectTerm(std::string);
    void deselectTerms();
    void notify();
    void attach(AppManagerObserver* amo);
    void detach(AppManagerObserver* amo);
    std::shared_ptr<std::map<HWND, std::string>> getOpenApps();
    std::vector<std::shared_ptr<ApplicationPositioning>> getHookedApps();
};

