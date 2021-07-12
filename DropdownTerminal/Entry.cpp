#include <FrontendEntry.h>
#include "ApplicationManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::unique_ptr<ApplicationManager> application_manager = std::make_unique<ApplicationManager>(ApplicationManager());
	application_manager->select_application_for_dd("MobaXterm");

	initFrontend();
	
    return 0;
}
