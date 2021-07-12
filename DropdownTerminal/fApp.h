#pragma once

#include <wx/wx.h>
#include "fMainFrame.h"

class fApp : public wxApp
{
public:
	fApp();
	~fApp();

private:
	fMainFrame* m_frame1 = nullptr;

public:
	virtual bool OnInit();
};

