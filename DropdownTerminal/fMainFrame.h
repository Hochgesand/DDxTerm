#pragma once
// Has nothing to do with mainframes.

#include <wx/wx.h>

class fMainFrame : public wxFrame
{
public:
	fMainFrame();
	~fMainFrame();

public:
	wxComboBox* m_combo_box1 = nullptr;
	wxStaticText* openAppsText = nullptr;
	std::vector<wxStaticText*> open_apps_vector;
	wxGridSizer* grid = new wxGridSizer(2, 1, 5, 5);
	
	void OnComboboxSelected(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

