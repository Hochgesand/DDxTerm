#include "fMainFrame.h"
#include <future>

#include "ApplicationManager.h"

wxBEGIN_EVENT_TABLE(fMainFrame, wxFrame)
	EVT_CLOSE(OnClose)
	EVT_BUTTON(1337, OnHookButtonPressed)
wxEND_EVENT_TABLE()

fMainFrame::fMainFrame() :
wxFrame(nullptr, 420, "DDxTerm", wxDefaultPosition, wxSize(600, 200)),
AppManagerObserver(std::make_unique<ApplicationManager>(ApplicationManager()))
{
	panel = new wxPanel(this, -1);
	getAppManager()->attach(this);
	std::map<HWND, std::string> openApps = *getAppManager()->getOpenApps();

	openAppsText = new wxStaticText(panel, wxID_ANY,"Open Applications: ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	m_combo_box1 = new wxComboBox(panel, wxID_ANY, "select one", wxDefaultPosition, wxSize(300, 25));
	m_text_ctrl = new wxTextCtrl(panel, wxID_ANY, "");
	m_button = new wxButton(panel, 1337, "HOOK");

	for (auto open_apps_string_arr : openApps)
	{
		m_combo_box1->Append(open_apps_string_arr.second);
	}

	hbox1->Add(openAppsText, 1);
	hbox1->Add(m_combo_box1, 1);
	hbox1->Add(m_text_ctrl, 1);
	hbox1->Add(m_button, 1);

	vbox->Add(hbox1, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 8);

	panel->SetSizer(vbox);
	Centre();
}

fMainFrame::~fMainFrame() = default;

void fMainFrame::Update()
{
	auto hookedApps = getAppManager()->getHookedApps();
	for (int i = 1; i < vbox->GetItemCount() ; ++i)
	{
		vbox->Remove(i);
	}
	
	for (auto element : hookedApps)
	{
		vbox->Add(new wxStaticText(panel, wxID_ANY, element), 1, wxTOP, 10);
	}
	panel->SetSizer(vbox);
	panel->Layout();
}

void fMainFrame::OnHookButtonPressed(wxCommandEvent& evt)
{
	getAppManager()->select_application_for_dd(m_combo_box1->GetValue().ToStdString(), 0x42);
}

void fMainFrame::OnClose(wxCloseEvent& evt)
{
	getAppManager()->deselectTerm();
	Destroy();
}
