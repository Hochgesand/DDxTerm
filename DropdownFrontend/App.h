#pragma once

#include <wx/wx.h>

#include "altMain.h"

class _declspec(dllexport) App : public wxApp
{
public:
	App();
	~App();

private:
	altMain* m_frame1 = nullptr;

public:
	virtual bool OnInit();
};

