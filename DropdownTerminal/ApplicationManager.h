#pragma once
#include <ApplicationPositioner.h>

class ApplicationManager
{
    std::vector<class AppManagerObserver*> _observers;
    std::shared_ptr<std::vector<std::shared_ptr<HookedWindow>>> openApplications;
    std::vector<std::shared_ptr<ApplicationPositioner>> hookedApplications;
    void eraseSelectedApplication(std::shared_ptr<ApplicationPositioner>);
public:
    ApplicationManager();
    void refreshRunningApps();
    void select_application_for_dd(HWND, unsigned int hotkey, unsigned int modHotkey);
    void deselectTerm(HWND);
    void deselectTerms();
    std::shared_ptr<std::vector<std::shared_ptr<HookedWindow>>> getOpenApps() const;
    std::vector<std::shared_ptr<ApplicationPositioner>> getHookedApps();
    void notify();
    void attach(AppManagerObserver* amo);
    void detach(AppManagerObserver* amo);
};

