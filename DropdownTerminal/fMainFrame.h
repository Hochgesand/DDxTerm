#pragma once
// Has nothing to do with mainframes.

#include <wx/wx.h>

#include "AppManagerObserver.h"

class fMainFrame : public AppManagerObserver, public wxFrame
{
	const int offsetID = 10000;
	
public:
	fMainFrame();
	~fMainFrame();

public:
	wxComboBox* m_combo_box1 = nullptr;
	wxStaticText* openAppsText = nullptr;
	wxComboBox* m_hotkey_control = nullptr;
	wxButton* m_button = nullptr;
	wxBoxSizer* main_vbox;
	std::shared_ptr<std::map<HWND, std::string>> openAppsPtr;

	std::vector<wxBoxSizer*> hookedAppsLines;
	
	std::map<wxStaticText*, wxButton*> hookedAndShownApps;
	
	wxPanel* panel = nullptr;

	void OnHookButtonPressed(wxCommandEvent& evt);
	void OnUnhookButtonPressed(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);
	void Update() override;
	void OnAppComboboxOpen(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

