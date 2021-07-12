#pragma once
#include <map>
#include <string>
#include <ApplicationPositioning.h>

class ApplicationManager
{
public:
    ApplicationManager();
    void refresh_running_apps();
    void select_application_for_dd(std::string);
	std::unique_ptr<std::map<HWND, std::string>> get_open_apps();
private:
    std::map<HWND, std::string> openApplications;
    std::unique_ptr<ApplicationPositioning> selectedApplication;
};

