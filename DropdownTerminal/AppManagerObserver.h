#pragma once
#include <vector>

#include "ApplicationManager.h"

class AppManagerObserver
{
	std::shared_ptr<class ApplicationManager> application_manager_;
public:
	AppManagerObserver(std::shared_ptr<ApplicationManager> application_manager)
	{
		application_manager_ = application_manager;
	}

	virtual void Update() = 0;

protected:
	std::shared_ptr<class ApplicationManager> getAppManager()
	{
		return application_manager_;
	}
};