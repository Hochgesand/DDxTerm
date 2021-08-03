#include "fMainFrame.h"
#include <future>

#include "ApplicationManager.h"

wxBEGIN_EVENT_TABLE(fMainFrame, wxFrame)
	EVT_CLOSE(OnClose)
	EVT_BUTTON(1337, OnHookButtonPressed)
wxEND_EVENT_TABLE()

UINT pressedButton = 0;

// WinAPI things, get pressed keys https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
LRESULT CALLBACK HostWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		pressedButton = wParam;
		break;
	case WM_KEYUP:
		pressedButton = 0;
		break;
		
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

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

	hookedAppsButtonsAndText.clear();
	for (auto hooked_app : hookedApps)
	{
		vbox->Destroy();
	}
	

	unsigned int counter = 0;
	for (auto &element : hookedApps)
	{
		auto newHbox = new wxBoxSizer(wxHORIZONTAL);
		newHbox->Add(new wxStaticText(panel, wxID_ANY, element), 1, wxTOP, 10);
		auto newButton = new wxButton(panel, offsetID + counter, "UNHOOK");
		newButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &fMainFrame::OnUnhookButtonPressed, this);
		newHbox->Add(newButton);
		vbox->Add(newHbox, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 8);

		counter++;
	}
	panel->SetSizer(vbox);
	panel->Layout();
}

void fMainFrame::OnHookButtonPressed(wxCommandEvent& evt)
{
	if (m_text_ctrl->GetLineLength(0) == 0)
	{
		MessageBox(nullptr, L"No hotkey typed", L"Oopsie Woopsie", MB_OK | MB_ICONERROR);
		return;
	}
	getAppManager()->select_application_for_dd(m_combo_box1->GetValue().ToStdString(), (UINT) m_text_ctrl->GetValue().ToStdString()[0]);
}

void fMainFrame::OnUnhookButtonPressed(wxCommandEvent& evt)
{
	auto hookedApps = getAppManager()->getHookedApps();
	const auto getButtonId = evt.GetId();
	// this magic number is there to substract the offset ID as seen in line 55.
	const auto getAppNameToClose = hookedApps.at(getButtonId - offsetID);
	getAppManager()->deselectTerm(getAppNameToClose);
}

void fMainFrame::OnClose(wxCloseEvent& evt)
{
	getAppManager()->deselectTerm();
	Destroy();
}
