#pragma once
// Has nothing to do with mainframes.

#include <wx/wx.h>

#include "AppManagerObserver.h"

class fMainFrame : public AppManagerObserver, public wxFrame
{
	
public:
	fMainFrame();
	~fMainFrame();
	void Update() override;

public:
	wxComboBox* m_combo_box1 = nullptr;
	wxStaticText* openAppsText = nullptr;
	wxTextCtrl* m_text_ctrl = nullptr;
	wxButton* m_button = nullptr;
	std::vector<wxStaticText*> open_apps_vector;

	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* panel = nullptr;

	void OnHookButtonPressed(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

