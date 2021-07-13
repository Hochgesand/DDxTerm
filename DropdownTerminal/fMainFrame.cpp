#include "fMainFrame.h"
#include <future>

#include "ApplicationManager.h"

wxBEGIN_EVENT_TABLE(fMainFrame, wxFrame)
	EVT_CLOSE(OnClose)
	EVT_COMBOBOX_CLOSEUP(1337, OnComboboxSelected)
wxEND_EVENT_TABLE()

std::unique_ptr<ApplicationManager> application_manager_;
std::future<void> app_logic;

fMainFrame::fMainFrame() : wxFrame(nullptr, 420, "DDxTerm")
{
	application_manager_ = std::make_unique<ApplicationManager>(ApplicationManager());
	std::map<HWND, std::string> openApps = application_manager_->getOpenApps();
	
	m_combo_box1 = new wxComboBox(this, 1337, "OpenApps", wxPoint(30, 30), wxSize(30, 10));

	for (auto open_apps_string_arr : openApps)
	{
		m_combo_box1->Append(open_apps_string_arr.second);
	}
}

fMainFrame::~fMainFrame()
{
}

void fMainFrame::OnComboboxSelected(wxCommandEvent& evt)
{
	application_manager_->deselectTerm();
	app_logic = std::async(
		std::launch::async,
		[&] {application_manager_->select_application_for_dd(m_combo_box1->GetValue().ToStdString()); }
	);
}

void fMainFrame::OnClose(wxCloseEvent& evt)
{
	application_manager_->deselectTerm();
	Destroy();
}
