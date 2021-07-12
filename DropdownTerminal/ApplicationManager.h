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
	std::map<HWND, std::string> get_open_apps();
    void deselect_term();
private:
    std::map<HWND, std::string> openApplications;
    std::unique_ptr<ApplicationPositioning> selectedApplication;
    bool terminator = true;
};

