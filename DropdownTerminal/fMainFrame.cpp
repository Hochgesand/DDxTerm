#include "fMainFrame.h"
#include <future>

#include "ApplicationManager.h"

wxBEGIN_EVENT_TABLE(fMainFrame, wxFrame)
	EVT_CLOSE(OnClose)
	EVT_COMBOBOX_CLOSEUP(1337, OnComboboxSelected)
wxEND_EVENT_TABLE()

std::unique_ptr<ApplicationManager> application_manager_;

fMainFrame::fMainFrame() : wxFrame(nullptr, 420, "DDxTerm", wxDefaultPosition, wxSize(400, 200))
{
	application_manager_ = std::make_unique<ApplicationManager>(ApplicationManager());
	std::map<HWND, std::string> openApps = *application_manager_->getOpenApps();

	openAppsText = new wxStaticText(this, wxID_ANY,"Open Applications", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	m_combo_box1 = new wxComboBox(this, 1337, "select one", wxPoint(30, 30), wxSize(30, 10));

	for (auto open_apps_string_arr : openApps)
	{
		m_combo_box1->Append(open_apps_string_arr.second);
	}

	grid->Add(openAppsText, 1, wxEXPAND | wxALL);
	grid->Add(m_combo_box1, 1, wxEXPAND | wxALL);

	this->SetSizer(grid);
	grid->Layout();
}

fMainFrame::~fMainFrame()
{
}

void fMainFrame::OnComboboxSelected(wxCommandEvent& evt)
{
	application_manager_->select_application_for_dd(m_combo_box1->GetValue().ToStdString());
	grid->SetRows(grid->GetRows() + 1);
	grid->Add(new wxStaticText(this, wxID_ANY, m_combo_box1->GetValue().ToStdString(), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL));
	this->SetSizer(grid);
	grid->Layout();
}

void fMainFrame::OnClose(wxCloseEvent& evt)
{
	application_manager_->deselectTerm();
	Destroy();
}
