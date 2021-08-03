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
	wxTextCtrl* m_text_ctrl = nullptr;
	wxButton* m_button = nullptr;
	std::vector<wxStaticText*> open_apps_vector;
	std::map<wxStaticText, wxButton, wxBoxSizer> hookedAppsButtonsAndText;

	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* panel = nullptr;

	void OnHookButtonPressed(wxCommandEvent& evt);
	void OnUnhookButtonPressed(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);
	void Update() override;

	wxDECLARE_EVENT_TABLE();
};

