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
AppManagerObserver(std::make_unique<ApplicationManager>(ApplicationManager())),
wxFrame(nullptr, 420, "DDxTerm", wxDefaultPosition, wxSize(600, 200))
{
	panel = new wxPanel(this, -1);
	main_vbox = new wxBoxSizer(wxVERTICAL);
	auto hbox1 = new wxBoxSizer(wxHORIZONTAL);
	getAppManager()->attach(this);

	openAppsText = new wxStaticText(panel, wxID_ANY,"Open Applications: ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	mComboboxOpenApps = new wxComboBox(panel, 501, "select one", wxDefaultPosition, wxSize(200, 25));
	mHotkeyModifier = new wxComboBox(panel, wxID_ANY, "mod hotkey", wxDefaultPosition, wxSize(50, 25));
	m_hotkey_control = new wxComboBox(panel, wxID_ANY, "select hotkey", wxDefaultPosition, wxSize(50, 25));
	m_button = new wxButton(panel, 1337, "HOOK");

	for (auto const open_apps_string_arr : *getAppManager()->getOpenApps())
	{
		mComboboxOpenApps->Append(open_apps_string_arr.second);
	}

	for (auto const open_app : HOTKEYS)
	{
		m_hotkey_control->Append(open_app.first);
	}

	for (auto const elem : KEYMODS)
	{
		mHotkeyModifier->Append(elem.first);
	}

	hbox1->Add(openAppsText, 1);
	hbox1->Add(mComboboxOpenApps, 1);
	hbox1->Add(mHotkeyModifier, 1);
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
	for (auto element : hookedApps)
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
	uint32_t tempModHotkey = 0xFF;
	const auto getValHotkey = m_hotkey_control->GetValue().ToStdString();
	const auto getValModHotkey = mHotkeyModifier->GetValue().ToStdString();
	for (auto const hotkey : HOTKEYS)
	{
		if (hotkey.first == getValHotkey)
		{
			tempHotkey = hotkey.second;
			break;
		}
	}
	for (auto const hotkey : KEYMODS)
	{
		if (hotkey.first == getValModHotkey)
		{
			tempModHotkey = hotkey.second;
			break;
		}
	}
	if (tempHotkey == 0xFF || tempModHotkey == 0xFF)
	{
		MessageBox(nullptr, L"Modkey and hotkey need to be selected", L"Oopsie Woopsie", MB_OK | MB_ICONERROR);
		return;
	}
	
	getAppManager()->select_application_for_dd(mComboboxOpenApps->GetValue().ToStdString(), tempHotkey, tempModHotkey);
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
	mComboboxOpenApps->Clear();
	getAppManager()->refreshRunningApps();
	for (auto open_apps_string_arr : *getAppManager()->getOpenApps())
	{
		mComboboxOpenApps->Append(open_apps_string_arr.second);
	}
}