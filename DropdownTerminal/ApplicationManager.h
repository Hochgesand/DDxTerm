#pragma once
#include <map>
#include <string>
#include <ApplicationPositioning.h>

class ApplicationManager
{
public:
    ApplicationManager();
    void refreshRunningApps();
    void select_application_for_dd(std::string);
	std::map<HWND, std::string>* getOpenApps();
    void deselectTerm(std::string);
    void deselectTerm();
    std::vector<std::string> getHookedApps();
private:
    std::map<HWND, std::string> openApplications;
    std::vector<std::unique_ptr<ApplicationPositioning>> selectedApplications;
};

