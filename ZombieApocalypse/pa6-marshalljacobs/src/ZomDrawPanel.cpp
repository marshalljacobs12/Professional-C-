#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"
#include "World.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel) // NOLINT
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);

	// Draw the status information
	DrawStatusText(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
	
	// TODO: Draw rest of grid/zombies/etc
	// Grid
	for (int i = 10; i < 610; i += 30)
	{
		for (int j = 10; j < 610; j += 30)
		{
			dc.DrawLine(i, j, i + 30, j);
			dc.DrawLine(i, j, i, j + 30);
			dc.DrawLine(i, j + 30, i + 30, j + 30);
			dc.DrawLine(i + 30, j, i + 30, j + 30);
		}
	}
	// Humans and Zombies
	//auto mStates = World::Get().GetmStates();
	//if (!mStates.empty())
	dc.SetBrush(wxBrush(wxColor("red"), wxBRUSHSTYLE_SOLID));
	/*
	if (true)
	{
		//auto mDemo = mStates[0];
		//auto mDemo = std::make_shared<MachineState>();
		auto mDemo = World::Get().GetmTestState();
		auto mX = mDemo.mX;
		auto mY = mDemo.mY;
		auto x1 = mX * 30 + 25;
		auto y1 = mY * 30 + 40;
		auto p1 = wxPoint(x1, y1);
		auto x2 = x1 - 15;
		auto y2 = y1 - 15;
		auto p2 = wxPoint(x2, y2);
		auto x3 = x2 + 30;
		auto y3 = y2;
		auto p3 = wxPoint(x3, y3);
		const wxPoint arr[] = { p1, p2, p3 };
		wxLogDebug("wxPoint mX (%d,%d)", mX, mY);

		dc.DrawPolygon(3, arr, 0, 0, wxODDEVEN_RULE);
	}
	*/
	auto mZombieStates = World::Get().GetmZombieMachineStates();
	if (!mZombieStates->empty())
	{
		int mX, mY;
		int x1, y1, x2, y2, x3, y3;
		wxPoint p1, p2, p3;

		for (auto i : *mZombieStates)
		{
			mX = i.mX;
			mY = i.mY;
			switch (i.mFacing)
			{
			case (MachineState::UP):
				x1 = mX * 30 + 25;
				y1 = mY * 30 + 10;
				p1 = wxPoint(x1, y1);
				x2 = x1 - 15;
				y2 = y1 + 15;
				p2 = wxPoint(x2, y2);
				x3 = x2 + 30;
				y3 = y1 + 15;
				p3 = wxPoint(x3, y3);
				break;
			case (MachineState::RIGHT):
				x1 = mX * 30 + 40;
				y1 = mY * 30 + 25;
				p1 = wxPoint(x1, y1);
				x2 = x1 - 15;
				y2 = y1 - 15;
				p2 = wxPoint(x2, y2);
				x3 = x2;
				y3 = y2 + 30;
				p3 = wxPoint(x3, y3);
				break;
			case (MachineState::DOWN):
				x1 = mX * 30 + 25;
				y1 = mY * 30 + 40;
				p1 = wxPoint(x1, y1);
				x2 = x1 - 15;
				y2 = y1 - 15;
				p2 = wxPoint(x2, y2);
				x3 = x2 + 30;
				y3 = y2;
				p3 = wxPoint(x3, y3);
				break;
			case (MachineState::LEFT):
				x1 = mX * 30 + 15;
				y1 = mY * 30 + 25;
				p1 = wxPoint(x1, y1);
				x2 = x1 + 15;
				y2 = y1 - 15;
				p2 = wxPoint(x2, y2);
				x3 = x2;
				y3 = y1 + 15;
				p3 = wxPoint(x3, y3);
				break;
			}
			const wxPoint arr[] = { p1, p2, p3 };
			dc.DrawPolygon(3, arr, 0, 0, wxODDEVEN_RULE);
		}
	}
	else
	{
		wxLogDebug("mZombieStates empty");
	}

	dc.SetBrush(wxBrush(wxColor("green"), wxBRUSHSTYLE_SOLID));
	auto mSurvivorStates = World::Get().GetmSurvivorMachineStates();
	if (!mSurvivorStates->empty())
	{
		int mX, mY;
		int x1, y1, x2, y2, x3, y3;
		wxPoint p1, p2, p3;

		for (auto i : *mSurvivorStates)
		{
			mX = i.mX;
			mY = i.mY;
			switch (i.mFacing)
			{
			case (MachineState::UP):
				x1 = mX * 30 + 25;
				y1 = mY * 30 + 10;
				p1 = wxPoint(x1, y1);
				x2 = x1 - 15;
				y2 = y1 + 15;
				p2 = wxPoint(x2, y2);
				x3 = x2 + 30;
				y3 = y1 + 15;
				p3 = wxPoint(x3, y3);
				break;
			case (MachineState::RIGHT):
				x1 = mX * 30 + 40;
				y1 = mY * 30 + 25;
				p1 = wxPoint(x1, y1);
				x2 = x1 - 15;
				y2 = y1 - 15;
				p2 = wxPoint(x2, y2);
				x3 = x2;
				y3 = y2 + 30;
				p3 = wxPoint(x3, y3);
				break;
			case (MachineState::DOWN):
				x1 = mX * 30 + 25;
				y1 = mY * 30 + 40;
				p1 = wxPoint(x1, y1);
				x2 = x1 - 15;
				y2 = y1 - 15;
				p2 = wxPoint(x2, y2);
				x3 = x2 + 30;
				y3 = y2;
				p3 = wxPoint(x3, y3);
				break;
			case (MachineState::LEFT):
				x1 = mX * 30 + 15;
				y1 = mY * 30 + 25;
				p1 = wxPoint(x1, y1);
				x2 = x1 + 15;
				y2 = y1 - 15;
				p2 = wxPoint(x2, y2);
				x3 = x2;
				y3 = y1 + 15;
				p3 = wxPoint(x3, y3);
				break;
			}
			const wxPoint arr[] = { p1, p2, p3 };
			dc.DrawPolygon(3, arr, 0, 0, wxODDEVEN_RULE);
		}
	}
	else
	{
		wxLogDebug("mSurvivorStates empty");
	}
}

void ZomDrawPanel::DrawStatusText(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	auto numZombies = World::Get().GetmZombieMachineStates()->size();
	std::string aliveZombiesText = "Alive: " + std::to_string(static_cast<int>(numZombies));
	std::string zombieFileText = "Current Zombie file: " + mCurrentZombieFile;

	dc.DrawText(wxString("Zombies"), wxCoord(640), wxCoord(20));
	dc.DrawText(wxString::FromUTF8(aliveZombiesText.c_str()), wxCoord(640), wxCoord(45));
	dc.DrawText(wxString::FromUTF8(zombieFileText.c_str()), wxCoord(640), wxCoord(70));

	auto numSurvivors = World::Get().GetmSurvivorMachineStates()->size();
	std::string aliveSurvivorText = "Alive: " + std::to_string(static_cast<int>(numSurvivors));
	std::string survivorFileText = "Current Survivor file: " + mCurrentSurvivorFile;

	dc.DrawText(wxString("Humans"), wxCoord(640), wxCoord(170));
	dc.DrawText(wxString::FromUTF8(aliveSurvivorText.c_str()), wxCoord(640), wxCoord(195));
	dc.DrawText(wxString::FromUTF8(survivorFileText.c_str()), wxCoord(640), wxCoord(220));
	
	auto month = World::Get().GetMonth();
	std::string monthText = "Month: " + std::to_string(month);

	dc.DrawText(wxString::FromUTF8(monthText.c_str()), wxCoord(640), wxCoord(270));
}
