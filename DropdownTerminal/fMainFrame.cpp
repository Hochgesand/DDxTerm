#include "fMainFrame.h"
#include <future>

#include "ApplicationManager.h"
#include "HotkeyDefs.h"

wxBEGIN_EVENT_TABLE(fMainFrame, wxFrame)
	EVT_CLOSE(OnClose)
	EVT_BUTTON(1337, OnHookButtonPressed)
	EVT_BUTTON(1338, OnRefreshPosButtonPressed)
	EVT_COMBOBOX_DROPDOWN(501, OnAppComboboxOpen)
wxEND_EVENT_TABLE()

fMainFrame::fMainFrame() :
AppManagerObserver(std::make_shared<ApplicationManager>(ApplicationManager())),
wxFrame(nullptr, 420, "DDxTerm", wxDefaultPosition, wxSize(600, 200))
{
	panel = new wxPanel(this, -1);
	mainVbox = new wxBoxSizer(wxVERTICAL);
	auto hbox1 = new wxBoxSizer(wxHORIZONTAL);
	getAppManager()->attach(this);

	openAppsText = new wxStaticText(panel, wxID_ANY,"Open Applications: ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	mComboboxOpenApps = new wxComboBox(panel, 501, "select one", wxDefaultPosition, wxSize(200, 25));
	mHotkeyModifier = new wxComboBox(panel, wxID_ANY, "mod hotkey", wxDefaultPosition, wxSize(50, 25));
	mhotkeyControl = new wxComboBox(panel, wxID_ANY, "select hotkey", wxDefaultPosition, wxSize(50, 25));
	mbutton = new wxButton(panel, 1337, "HOOK");
	mbuttonRefresh = new wxButton(panel, 1338, "reload pos");

	for (auto const& open_apps_string_arr : *getAppManager()->getOpenApps())
	{
		mComboboxOpenApps->Append(open_apps_string_arr->getAppName());
	}

	for (auto [fst, snd] : HOTKEYS)
	{
		mhotkeyControl->Append(fst);
	}

	for (auto [fst, snd] : KEYMODS)
	{
		mHotkeyModifier->Append(fst);
	}

	hbox1->Add(openAppsText, 1);
	hbox1->Add(mComboboxOpenApps, 1);
	hbox1->Add(mHotkeyModifier, 1);
	hbox1->Add(mhotkeyControl, 1);
	hbox1->Add(mbutton, 1);
	hbox1->Add(mbuttonRefresh, 1);
	
	mainVbox->Add(hbox1, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 8);

	panel->SetSizer(mainVbox);
	Centre();
}

fMainFrame::~fMainFrame() = default;

// Update frontend information
// Everything that has updatable information from an Observer gets updated here
void fMainFrame::Update()
{
	auto hookedApps = getAppManager()->getHookedApps();

	// Delete all old information
	for (auto hooked_and_shown_app : hookedAndShownApps)
	{
		for (auto and_shown_app : hooked_and_shown_app)
		{
			and_shown_app->Destroy();
		}
	}

	// Don't know how to split frontend into multiple classes
	// NEED TO DO THAT NEXT, will be there in one or two master merges.
	for (auto hooked_apps_line : hookedAppsLines)
	{
		mainVbox->Detach(hooked_apps_line);
		delete hooked_apps_line;
	}

	// Clear those vectors
	hookedAppsLines.clear();
	hookedAndShownApps.clear();

	unsigned int counter = 0;
	for (auto &element : hookedApps)
	{
		element->getApplicationHook()->refreshTerminalPosition();
		auto newHbox = new wxBoxSizer(wxHORIZONTAL);
		hookedAppsLines.push_back(newHbox);

		// Prepare texts and paste them in. 
		auto tempText = new wxStaticText(panel, wxID_ANY, element->getApplicationHook()->getApplicationInformation()->getAppName());
		auto tempDebugXYText{ "X: " + std::to_string(element->getApplicationHook()->getApplicationRect()->left) +
			" Y: " + std::to_string(element->getApplicationHook()->getApplicationRect()->top)};
		
		auto hotkeyDebugText = new wxStaticText(panel, wxID_ANY,"Hotkey: " + getModHotkeyNameByUINT(element->getSelectedHotkeys().at(0)) + " + " + getHotkeyNameByUINT(element->getSelectedHotkeys().at(1)), wxDefaultPosition, wxSize(200, 25));
		auto tempDebugInfoXY = new wxStaticText(panel, wxID_ANY, tempDebugXYText);
		auto tempButton = new wxButton(panel, offsetID + counter, "UNHOOK");
		tempButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &fMainFrame::OnUnhookButtonPressed, this);
		hookedAndShownApps.push_back({ tempText, tempDebugInfoXY, hotkeyDebugText, tempButton });
		
		newHbox->Add(tempText, 1, wxTOP, 10);
		newHbox->Add(hotkeyDebugText, 1, wxTOP, 10);
		newHbox->Add(tempDebugInfoXY, 1, wxTOP, 10);
		newHbox->Add(tempButton, 1, wxTOP, 10);
		
		mainVbox->Add(newHbox, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 2);
		counter++;
	}
	panel->SetSizer(mainVbox);
	panel->Layout();
}

void fMainFrame::OnHookButtonPressed(wxCommandEvent& evt)
{
	// Initialized with values which dont exist in the VirtualKeyMap
	uint32_t tempHotkey = 0xFF;
	uint32_t tempModHotkey = 0xFF;
	const auto getValHotkey = mhotkeyControl->GetValue().ToStdString();
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
	const auto selectedItemId = mComboboxOpenApps->GetSelection();
	const auto selectedClientData = static_cast<HWND>(mComboboxOpenApps->GetClientData(selectedItemId));
	
	getAppManager()->select_application_for_dd(selectedClientData, tempHotkey, tempModHotkey);
}

// Obvious methods do obvious things down here.

// Debug method, used by a friend who has a pretty interesting monitorsetup which screws with my code.
// He tests that.
void fMainFrame::OnRefreshPosButtonPressed(wxCommandEvent& evt)
{
	Update();
}

void fMainFrame::OnUnhookButtonPressed(wxCommandEvent& evt)
{
	auto hookedApps = getAppManager()->getHookedApps();
	const auto getButtonId = evt.GetId();
	const auto getAppNameToClose = hookedApps.at(getButtonId - offsetID)->getApplicationHook()->getApplicationInformation()->getHwnd();
	getAppManager()->deselectTerm(getAppNameToClose);
}

void fMainFrame::OnClose(wxCloseEvent& evt)
{
	getAppManager()->deselectTerms();
	Destroy();
}

void fMainFrame::OnAppComboboxOpen(wxCommandEvent& evt)
{
	mComboboxOpenApps->Clear();
	getAppManager()->refreshRunningApps();
	for (auto elem : *getAppManager()->getOpenApps())
	{
		mComboboxOpenApps->Append(elem->getAppName(), elem->getHwnd());
	}
}