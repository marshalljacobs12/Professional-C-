#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>

class ZomDrawPanel : public wxPanel
{
 
public:
	ZomDrawPanel(wxFrame* parent);
	void PaintNow();

protected:
	void PaintEvent(wxPaintEvent & evt);
	void Render(wxDC& dc);
	void DrawGrid(wxDC& dc);
	void DrawStatusText(wxDC& dc);

	DECLARE_EVENT_TABLE()
	
public:
	// Add variables here
	std::string mCurrentZombieFile;
	std::string mCurrentSurvivorFile;
};

