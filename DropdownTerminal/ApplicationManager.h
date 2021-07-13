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
	std::map<HWND, std::string> getOpenApps();
    void deselectTerm();
private:
    std::map<HWND, std::string> openApplications;
    std::unique_ptr<ApplicationPositioning> selectedApplication;
    bool terminator = true;
};

