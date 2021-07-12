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

	void OnComboboxSelected(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

