#pragma once
#include <map>
#include <string>
#include <ApplicationPositioning.h>

class ApplicationManager
{
    std::vector<class AppManagerObserver*> observers_;
    std::map<HWND, std::string> openApplications;
    std::vector<std::unique_ptr<ApplicationPositioning>> selectedApplications;
    void eraseSelectedApplication(std::unique_ptr<ApplicationPositioning>&);

public:
    ApplicationManager();
    void refreshRunningApps();
    void select_application_for_dd(std::string, unsigned int hotkey);
    void deselectTerm(std::string);
    void deselectTerm();
    void notify();
    void attach(AppManagerObserver* amo);
    void detach(AppManagerObserver* amo);
    std::map<HWND, std::string>* getOpenApps();
    std::vector<std::string> getHookedApps();
};

