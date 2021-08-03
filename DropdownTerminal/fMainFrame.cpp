#include "fMainFrame.h"
#include <future>

#include "ApplicationManager.h"
#include "HotkeyDefs.h"

wxBEGIN_EVENT_TABLE(fMainFrame, wxFrame)
	EVT_CLOSE(OnClose)
	EVT_BUTTON(1337, OnHookButtonPressed)
	EVT_COMBOBOX_DROPDOWN(501, OnAppComboboxOpen)
wxEND_EVENT_TABLE()

fMainFrame::fMainFrame() :
wxFrame(nullptr, 420, "DDxTerm", wxDefaultPosition, wxSize(1000, 200)),
AppManagerObserver(std::make_unique<ApplicationManager>(ApplicationManager()))
{
	panel = new wxPanel(this, -1);
	main_vbox = new wxBoxSizer(wxVERTICAL);
	auto hbox1 = new wxBoxSizer(wxHORIZONTAL);
	getAppManager()->attach(this);
	openAppsPtr = getAppManager()->getOpenApps();

	openAppsText = new wxStaticText(panel, wxID_ANY,"Open Applications: ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	m_combo_box1 = new wxComboBox(panel, 501, "select one", wxDefaultPosition, wxSize(300, 25));
	m_hotkey_control = new wxComboBox(panel, wxID_ANY, "select hotkey", wxDefaultPosition, wxSize(300, 25));
	m_button = new wxButton(panel, 1337, "HOOK");

	for (auto open_apps_string_arr : *openAppsPtr)
	{
		m_combo_box1->Append(open_apps_string_arr.second);
	}

	for (auto open_app : HOTKEYS)
	{
		m_hotkey_control->Append(open_app.first);
	}

	hbox1->Add(openAppsText, 1);
	hbox1->Add(m_combo_box1, 1);
	hbox1->Add(m_hotkey_control, 1);
	hbox1->Add(m_button, 1);
	
	main_vbox->Add(hbox1, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 8);

	panel->SetSizer(main_vbox);
	Centre();
}

fMainFrame::~fMainFrame() = default;

void fMainFrame::Update()
{
	auto hookedApps = getAppManager()->getHookedApps();

	for (auto hooked_and_shown_app : hookedAndShownApps)
	{
		hooked_and_shown_app.first->Destroy();
		hooked_and_shown_app.second->Destroy();
	}

	for (auto hooked_apps_line : hookedAppsLines)
	{
		main_vbox->Detach(hooked_apps_line);
		delete hooked_apps_line;
	}

	hookedAppsLines.clear();
	hookedAndShownApps.clear();

	unsigned int counter = 0;
	for (auto &element : hookedApps)
	{
		auto newHbox = new wxBoxSizer(wxHORIZONTAL);
		hookedAppsLines.push_back(newHbox);
		
		auto tempText = new wxStaticText(panel, wxID_ANY, element);
		auto tempButton = new wxButton(panel, offsetID + counter, "UNHOOK");
		tempButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &fMainFrame::OnUnhookButtonPressed, this);
		hookedAndShownApps.insert({ tempText, tempButton });
		
		newHbox->Add(tempText, 1, wxTOP, 10);
		newHbox->Add(tempButton, 1, wxTOP, 10);
		
		main_vbox->Add(newHbox, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 8);
		counter++;
	}
	panel->SetSizer(main_vbox);
	panel->Layout();
}

void fMainFrame::OnHookButtonPressed(wxCommandEvent& evt)
{
	uint32_t tempHotkey = 0xFF;
	auto getval = m_hotkey_control->GetValue().ToStdString();
	for (auto hotkeys : HOTKEYS)
	{
		if (hotkeys.first == getval)
		{
			tempHotkey = hotkeys.second;
			break;
		}
	}
	if (tempHotkey == 0xFF)
	{
		MessageBox(nullptr, L"No hotkey selected", L"Oopsie Woopsie", MB_OK | MB_ICONERROR);
		return;
	}
	
	getAppManager()->select_application_for_dd(m_combo_box1->GetValue().ToStdString(), tempHotkey);
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

void fMainFrame::OnAppComboboxOpen(wxCommandEvent& evt)
{
	m_combo_box1->Clear();
	for (auto open_apps_string_arr : *openAppsPtr)
	{
		m_combo_box1->Append(open_apps_string_arr.second);
	}
}