#pragma once
#include <vector>

#include "ApplicationManager.h"

class AppManagerObserver
{
	std::shared_ptr<class ApplicationManager> _applicationManager;
public:
	AppManagerObserver(std::shared_ptr<ApplicationManager> applicationManager)
	{
		_applicationManager = applicationManager;
	}

	virtual void Update() = 0;

protected:
	std::shared_ptr<class ApplicationManager> getAppManager()
	{
		return _applicationManager;
	}
};