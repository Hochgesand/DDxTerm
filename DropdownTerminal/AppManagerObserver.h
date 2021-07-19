#pragma once
#include <vector>

#include "ApplicationManager.h"

class AppManagerObserver
{
	std::unique_ptr<class ApplicationManager> application_manager_;
public:
	AppManagerObserver(std::unique_ptr<ApplicationManager> application_manager)
	{
		application_manager_.swap(application_manager);
	}

	virtual void Update() = 0;

protected:
	ApplicationManager* getAppManager()
	{
		return application_manager_.get();
	}
};