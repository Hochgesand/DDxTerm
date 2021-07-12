#include "fApp.h"

wxIMPLEMENT_APP(fApp);

fApp::fApp()
{
	
}

fApp::~fApp()
{
}

bool fApp::OnInit()
{
	m_frame1 = new fMainFrame();
	m_frame1->Show();

	return true;
}
