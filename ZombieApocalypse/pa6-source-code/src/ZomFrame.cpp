#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include <stdexcept>
#include "Exceptions.h"
#include "World.h"

enum
{
	ID_SIM_START = 1000,
	ID_TURN_TIMER,
	ID_LOAD_ZOMBIE_FILE,
	ID_LOAD_SURVIVOR_FILE,
	ID_RANDOMIZE,
	ID_RESET,
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame) // NOLINT
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SIM_START, ZomFrame::OnSimStart)
	EVT_MENU(ID_LOAD_ZOMBIE_FILE, ZomFrame::OnLoadZombieFile)
	EVT_MENU(ID_LOAD_SURVIVOR_FILE, ZomFrame::OnLoadSurvivorFile)
	EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
	EVT_MENU(ID_RESET, ZomFrame::OnReset)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(nullptr, wxID_ANY, title, pos, size)
, mPanel(nullptr)
, mSimMenu(nullptr)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SIM_START, "Start/stop\tSpace", "Start or stop the simulation");
	mSimMenu->Append(ID_LOAD_ZOMBIE_FILE, "Load Zombie...");
	mSimMenu->Append(ID_LOAD_SURVIVOR_FILE, "Load Survivor...");
	mSimMenu->Append(ID_RANDOMIZE, "Randomize");
	mSimMenu->Append(ID_RESET, "Reset");
	mSimMenu->Enable(ID_SIM_START, false);
	mSimMenu->Enable(ID_RANDOMIZE, false);
	mSimMenu->Enable(ID_RESET, false);

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	wxFrame::SetMenuBar(menuBar);
	wxFrame::CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	wxFrame::SetSizer(sizer);
	wxFrame::SetAutoLayout(true);
	wxFrame::Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);

	// TEMP CODE: Initialize zombie test machine
	//mZombieMachine.LoadMachine(std::string(""));
	//mZombieMachine.BindState(mZombieTestState);
	// END TEMP CODE
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	// TODO: Add code for File>New
	mZombieMachine.ClearMachine();
	mPanel->mCurrentZombieFile.clear();
	mSurvivorMachine.ClearMachine();
	mPanel->mCurrentSurvivorFile.clear();
	World::Get().GetmZombieMachineStates()->clear();
	World::Get().GetmSurvivorMachineStates()->clear();
	World::Get().ResetMonth();
	mTurnTimer->Stop();
	mSimMenu->Enable(ID_SIM_START, false);
	mSimMenu->Enable(ID_RANDOMIZE, false);
	mSimMenu->Enable(ID_RESET, false);
	mPanel->PaintNow();
	wxLogDebug("Month: %d", World::Get().GetMonth());
}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
	// TEMP CODE: Take turn for zombie machine
	//mZombieMachine.TakeTurn(mZombieTestState);
	//SetTestState();
	auto zombieStates = World::Get().GetmZombieMachineStates();
	//
	//World::Get().GetmZombieMachineStates()->clear();
	if (!(*zombieStates).empty())
	{
		for (auto& i : *zombieStates)
		{
			mZombieMachine.TakeTurn(i);
			//World::Get().GetmZombieMachineStates()->emplace_back(i);
		}
	}
	//mPanel->PaintNow();

	auto survivorStates = World::Get().GetmSurvivorMachineStates();
	if (!(*survivorStates).empty())
	{
		for (auto& i : *survivorStates)
		{
			mSurvivorMachine.TakeTurn(i);
		}
	}
	// END TEMP CODE
	World::Get().IncrementMonth();
	wxLogDebug("Month: %d", World::Get().GetMonth());

	mPanel->PaintNow();

	if (World::Get().GetmSurvivorMachineStates()->empty() && !World::Get().GetmZombieMachineStates()->empty())
	{
		mTurnTimer->Stop();
		mIsActive = false;
		wxMessageBox(wxT("Zombies Win"));
	}
	if (World::Get().GetmZombieMachineStates()->empty() && !World::Get().GetmSurvivorMachineStates()->empty())
	{
		mTurnTimer->Stop();
		mIsActive = false;
		wxMessageBox(wxT("Humans Win"));
	}
}

void ZomFrame::OnLoadZombieFile(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, "Open ZOM file", "./zom", "",
		"ZOM Files|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK)
	{
		auto filename = openFileDialog.GetPath();
		// An invalid file will cause ParseZomFile to throw an InvalidFileException
		// Invalid operations in the .zom file will throw InvalidOpException
		try
		{
			// parse file
			mZombieMachine.LoadMachine(filename.c_str());
			if (mZombieMachine.HasRangedAttack())
			{
				mZombieMachine.ClearMachine();
				throw InvalidFileException("Zombie file has ranged attack");
			}
			mPanel->mCurrentZombieFile = mZombieMachine.GetName();
			if (!mSurvivorMachine.GetName().empty())
			{
				mSimMenu->Enable(ID_SIM_START, true);
				mSimMenu->Enable(ID_RANDOMIZE, true);
				mSimMenu->Enable(ID_RESET, true);
			}
			mPanel->PaintNow();
		}
		catch (InvalidFileException &e)
		{
			wxLogMessage(e.what());
		}
		catch (InvalidOpException &ioe)
		{
			wxLogMessage(ioe.what());
		}
	}
}

void ZomFrame::OnLoadSurvivorFile(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, "Open ZOM file", "./zom", "",
		"ZOM Files|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK)
	{
		auto filename = openFileDialog.GetPath();
		// An invalid file will cause ParseZomFile to throw an InvalidFileException
		// Invalid operations in the .zom file will throw InvalidOpException
		try
		{
			// parse file
			mSurvivorMachine.LoadMachine(filename.c_str());
			mPanel->mCurrentSurvivorFile = mSurvivorMachine.GetName();
			if (!mZombieMachine.GetName().empty())
			{
				mSimMenu->Enable(ID_SIM_START, true);
				mSimMenu->Enable(ID_RANDOMIZE, true);
				mSimMenu->Enable(ID_RESET, true);
			}
			mPanel->PaintNow();
		}
		catch (InvalidFileException &e)
		{
			wxLogMessage(e.what());
		}
		catch (InvalidOpException &ioe)
		{
			wxLogMessage(ioe.what());
		}
	}
}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
	if (!mZombieMachine.GetName().empty())
	{
		int numZombies = World::Get().GetRandomPopulation();
		wxLogDebug("%d", numZombies);
		auto mZombieStates = World::Get().GetmZombieMachineStates();
		for (int i = 0; i < 20; i++)
		{
			//try to place the zombie in random location. If it fails throw exception
			MachineState newZombie;
			int randX = World::Get().GetRandomCoordinate();
			int randY = World::Get().GetRandomCoordinate();
			// don't put new zombie in occupied location
			while (World::Get().IsLocationOccupied(randX, randY))
			{
				randX = World::Get().GetRandomCoordinate();
				randY = World::Get().GetRandomCoordinate();
			}
			newZombie.mX = randX;
			newZombie.mY = randY;
			int randOrientation = World::Get().GetRandomOrientation();
			if (randOrientation == 0)
			{
				newZombie.mFacing = MachineState::UP;
			}
			else if (randOrientation == 1)
			{
				newZombie.mFacing = MachineState::RIGHT;
			}
			else if (randOrientation == 2)
			{
				newZombie.mFacing = MachineState::DOWN;
			}
			else if (randOrientation == 3)
			{
				newZombie.mFacing = MachineState::LEFT;
			}
			mZombieMachine.BindState(newZombie);
			World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
		}
	}

	if (!mSurvivorMachine.GetName().empty())
	{
		int numSurvivors = World::Get().GetRandomPopulation();
		wxLogDebug("%d", numSurvivors);
		for (int i = 0; i < 10; i++)
		{
			// try to place the survivor in random location. If it fails throw exception
			MachineState newSurvivor;
			int randX = World::Get().GetRandomCoordinate();
			int randY = World::Get().GetRandomCoordinate();
			// don't put new survivor in occupied location
			while (World::Get().IsLocationOccupied(randX, randY))
			{
				randX = World::Get().GetRandomCoordinate();
				randY = World::Get().GetRandomCoordinate();
			}
			newSurvivor.mX = randX;
			newSurvivor.mY = randY;
			int randOrientation = World::Get().GetRandomOrientation();
			if (randOrientation == 0)
			{
				newSurvivor.mFacing = MachineState::UP;
			}
			else if (randOrientation == 1)
			{
				newSurvivor.mFacing = MachineState::RIGHT;
			}
			else if (randOrientation == 2)
			{
				newSurvivor.mFacing = MachineState::DOWN;
			}
			else if (randOrientation == 3)
			{
				newSurvivor.mFacing = MachineState::LEFT;
			}
			mSurvivorMachine.BindState(newSurvivor);
			World::Get().GetmSurvivorMachineStates()->emplace_back(newSurvivor);
		}
	}
	mPanel->PaintNow();
}

void ZomFrame::OnReset(wxCommandEvent& event)
{
	World::Get().GetmZombieMachineStates()->clear();
	World::Get().GetmSurvivorMachineStates()->clear();
	World::Get().ResetMonth();
	mTurnTimer->Stop();
	mPanel->PaintNow();
	wxLogDebug("Month: %d", World::Get().GetMonth());
}
