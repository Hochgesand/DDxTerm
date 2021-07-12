#include "App.h"

wxIMPLEMENT_APP(App);

App::App()
{
}

App::~App()
{
}

bool App::OnInit()
{
	m_frame1 = new altMain();
	m_frame1->Show();
	return true;
}
