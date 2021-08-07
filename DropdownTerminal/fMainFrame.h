#pragma once
// Has nothing to do with mainframes.

#include <wx/wx.h>

#include "ApplicationManager.h"
#include "AppManagerObserver.h"

class fMainFrame : public AppManagerObserver, public wxFrame
{
	const int offsetID = 10000;
	
public:
	fMainFrame();
	~fMainFrame();

public:
	wxComboBox* mComboboxOpenApps = nullptr;
	wxStaticText* openAppsText = nullptr;
	wxComboBox* mhotkeyControl = nullptr;
	wxComboBox* mHotkeyModifier = nullptr;
	wxButton* mbutton = nullptr;
	wxButton* mbuttonRefresh = nullptr;
	wxBoxSizer* mainVbox;

	std::vector<wxBoxSizer*> hookedAppsLines;

	std::vector<std::vector<wxWindow*>> hookedAndShownApps;
	
	wxPanel* panel = nullptr;

	void OnHookButtonPressed(wxCommandEvent& evt);
	void OnRefreshPosButtonPressed(wxCommandEvent& evt);
	void OnUnhookButtonPressed(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);
	void Update() override;
	void OnAppComboboxOpen(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

