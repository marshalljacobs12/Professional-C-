#include "PaintFrame.h"
#include <string>
#include <stdexcept>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/toolbar.h>
#include <wx/image.h>
#include <wx/colordlg.h>
#include <wx/textdlg.h>
#include <wx/filedlg.h>
#include "PaintDrawPanel.h"
#include "PaintModel.h"

wxBEGIN_EVENT_TABLE(PaintFrame, wxFrame) // NOLINT
	EVT_MENU(wxID_EXIT, PaintFrame::OnExit)
	EVT_MENU(wxID_NEW, PaintFrame::OnNew)
	EVT_MENU(ID_Import, PaintFrame::OnImport)
	EVT_TOOL(ID_Import, PaintFrame::OnImport)
	EVT_MENU(ID_Export, PaintFrame::OnExport)
	EVT_TOOL(ID_Export, PaintFrame::OnExport)
	EVT_MENU(wxID_UNDO, PaintFrame::OnUndo)
	EVT_TOOL(wxID_UNDO, PaintFrame::OnUndo)
	EVT_MENU(wxID_REDO, PaintFrame::OnRedo)
	EVT_TOOL(wxID_REDO, PaintFrame::OnRedo)
	EVT_MENU(ID_Unselect, PaintFrame::OnUnselect)
	EVT_MENU(ID_Delete, PaintFrame::OnDelete)
	EVT_MENU(ID_SetPenColor, PaintFrame::OnSetPenColor)
	EVT_MENU(ID_SetPenWidth, PaintFrame::OnSetPenWidth)
	EVT_MENU(ID_SetBrushColor, PaintFrame::OnSetBrushColor)
	// The different draw modes
	EVT_TOOL(ID_Selector, PaintFrame::OnSelectTool)
	EVT_TOOL(ID_DrawLine, PaintFrame::OnSelectTool)
	EVT_TOOL(ID_DrawEllipse, PaintFrame::OnSelectTool)
	EVT_TOOL(ID_DrawRect, PaintFrame::OnSelectTool)
	EVT_TOOL(ID_DrawPencil, PaintFrame::OnSelectTool)
wxEND_EVENT_TABLE()	

PaintFrame::PaintFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	// Initialize image handlers to support BMP, PNG, JPEG
	wxImage::AddHandler(new wxPNGHandler()); // NOLINT
	wxImage::AddHandler(new wxJPEGHandler()); // NOLINT

	SetupMenu(); // NOLINT
	
	SetupToolbar();

	SetupModelAndView();

	wxFrame::Show(true);
	
	wxFrame::SetMinSize(GetSize());
	wxFrame::SetMaxSize(GetSize());
}

void PaintFrame::SetupMenu()
{
	// File menu
	mFileMenu = new wxMenu();
	mFileMenu->Append(wxID_NEW);
	mFileMenu->Append(ID_Export, "Export...",
		"Export current drawing to image file.");
	mFileMenu->AppendSeparator();
	mFileMenu->Append(ID_Import, "Import...",
		"Import image into file.");
	mFileMenu->Append(wxID_EXIT);

	// Edit menu
	mEditMenu = new wxMenu();
	mEditMenu->Append(wxID_UNDO);
	mEditMenu->Append(wxID_REDO);
	mEditMenu->AppendSeparator();
	mEditMenu->Append(ID_Unselect, "Unselect",
		"Unselect the current selection");
	mEditMenu->AppendSeparator();
	mEditMenu->Append(ID_Delete, "Delete\tDel",
		"Delete the current selection");
	
	mEditMenu->Enable(wxID_UNDO, false);
	mEditMenu->Enable(wxID_REDO, false);
	mEditMenu->Enable(ID_Unselect, false);
	mEditMenu->Enable(ID_Delete, false);

	// Colors menu
	mColorMenu = new wxMenu();
	mColorMenu->Append(ID_SetPenColor, "Pen Color...", "Set the pen color.");
	mColorMenu->Append(ID_SetPenWidth, "Pen Width...", "Set the pen width.");
	mColorMenu->AppendSeparator();
	mColorMenu->Append(ID_SetBrushColor, "Brush Color...", "Set brush color");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(mFileMenu, "&File");
	menuBar->Append(mEditMenu, "&Edit");
	menuBar->Append(mColorMenu, "&Colors");
	SetMenuBar(menuBar);
	CreateStatusBar();
}

void PaintFrame::SetupToolbar()
{
	// Create the toolbar
	mToolbar = CreateToolBar();
	mToolbar->AddTool(wxID_NEW, "New",
		wxBitmap("Icons/New.png", wxBITMAP_TYPE_PNG),
		"New");
	mToolbar->AddTool(ID_Export, "Export",
		wxBitmap("Icons/Save.png", wxBITMAP_TYPE_PNG),
		"Export");
	mToolbar->AddTool(ID_Import, "Import",
		wxBitmap("Icons/Import.png", wxBITMAP_TYPE_PNG),
		"Import");
	mToolbar->AddSeparator();

	mToolbar->AddTool(wxID_UNDO, "Undo",
		wxBitmap("Icons/Undo.png", wxBITMAP_TYPE_PNG),
		"Undo");
	mToolbar->AddTool(wxID_REDO, "Redo",
		wxBitmap("Icons/Redo.png", wxBITMAP_TYPE_PNG),
		"Redo");
	mToolbar->AddSeparator();

	mToolbar->AddTool(ID_Selector, "Selector",
		wxBitmap("Icons/Cursor.png", wxBITMAP_TYPE_PNG),
		"Selector", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawLine, "Draw Line",
		wxBitmap("Icons/Line.png", wxBITMAP_TYPE_PNG),
		"Draw Line", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawEllipse, "Draw Ellipse",
		wxBitmap("Icons/Ellipse.png", wxBITMAP_TYPE_PNG),
		"Draw Ellipse", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawRect, "Draw Rectangle",
		wxBitmap("Icons/Rectangle.png", wxBITMAP_TYPE_PNG),
		"Draw Rectangle", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawPencil, "Pencil",
		wxBitmap("Icons/Pencil.png", wxBITMAP_TYPE_PNG),
		"Pencil", wxITEM_CHECK);

	mToolbar->Realize();

	ToggleTool(ID_Selector);

	// Both undo and redo are disabled initially
	mToolbar->EnableTool(wxID_UNDO, false);
	mToolbar->EnableTool(wxID_REDO, false);
}

void PaintFrame::SetupModelAndView()
{
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new PaintDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);

	// Programatically bind the mouse events on the draw panel to us
	mPanel->Bind(wxEVT_LEFT_DOWN, &PaintFrame::OnMouseButton, this);
	mPanel->Bind(wxEVT_LEFT_UP, &PaintFrame::OnMouseButton, this);
	mPanel->Bind(wxEVT_MOTION, &PaintFrame::OnMouseMove, this);

	// Create the model
	mModel = std::make_shared<PaintModel>();
	mPanel->SetModel(mModel);
	SetSizer(sizer);

	SetAutoLayout(true);
}

void PaintFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void PaintFrame::OnNew(wxCommandEvent& event)
{
	mModel->New();
	UpdateUndoRedoButtons();
	mPanel->PaintNow();
}

void PaintFrame::OnExport(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, "Save drawing to file", "", "", 
		"JPG files (*.jpg)|*.jpg|BMP files (*.bmp)|*.bmp|PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		auto name = saveFileDialog.GetPath();
		auto size = mPanel->GetSize();
		mModel->ExportDrawing(name, size);
	}
}

void PaintFrame::OnImport(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, "Open drawing from file", "", "",
		"JPG files (*.jpg)|*.jpg|BMP files (*.bmp)|*.bmp|PNG files (*.png)|*.png", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK)
	{
		auto name = openFileDialog.GetPath();
		mModel->ImportDrawing(name);
		mPanel->PaintNow();
	}
}

void PaintFrame::OnUndo(wxCommandEvent& event)
{
	mModel->Undo();
	UpdateUndoRedoButtons();
	mPanel->PaintNow();
}

void PaintFrame::OnRedo(wxCommandEvent& event)
{
	mModel->Redo();
	UpdateUndoRedoButtons();
	mPanel->PaintNow();
}

void PaintFrame::OnUnselect(wxCommandEvent& event)
{
	mModel->Unselect();
	UpdateSelectMenuOptions();
	UpdateUndoRedoButtons();
	mPanel->PaintNow();
}

void PaintFrame::OnDelete(wxCommandEvent& event)
{
	mModel->CreateCommand(CM_Delete, wxPoint());
	mModel->FinalizeCommand();
	mModel->Unselect();
	UpdateUndoRedoButtons();
	UpdateSelectMenuOptions();
	mPanel->PaintNow();
}

void PaintFrame::OnSetPenColor(wxCommandEvent& event)
{
	wxColourData data; 
	data.SetColour(mModel->GetPenColor());

	// display modal for user to select a new pen color
	wxColourDialog dialog(this, &data); 

	// user selected a color, so update mode
	if (dialog.ShowModal() == wxID_OK) 
	{
		auto newColor = dialog.GetColourData().GetColour();
		mModel->SetPenColor(newColor);
		if (mModel->HasSelectedShape())
		{
			mModel->CreateCommand(CM_SetPen, wxPoint());
			mModel->FinalizeCommand();
		}
	}

	UpdateUndoRedoButtons();
	mPanel->PaintNow();
}

void PaintFrame::OnSetPenWidth(wxCommandEvent& event)
{	
	wxTextEntryDialog dialog(this, "Must be an integer value between 1-10", "Emter new pen width");
	
	if (dialog.ShowModal() == wxID_OK)
	{
		std::string newWidthStr = dialog.GetValue().ToStdString();

		if (!newWidthStr.empty())
		{
			try
			{
				int newWidth = std::stoi(newWidthStr);
				if (newWidth >= 1 && newWidth <= 10)
				{
					mModel->SetPenWidth(newWidth);
					if (mModel->HasSelectedShape())
					{
						mModel->CreateCommand(CM_SetPen, wxPoint());
						mModel->FinalizeCommand();
					}
				}
			}
			// stoi either throws an invalid_argument or out_of range exception
			catch (std::invalid_argument)
			{

			}
			catch (std::out_of_range)
			{

			}
		}
	}

	UpdateUndoRedoButtons();
	mPanel->PaintNow();
}

void PaintFrame::OnSetBrushColor(wxCommandEvent& event)
{
	wxColourData data;
	data.SetColour(mModel->GetBrushColor());

	// display modal for user to select a new pen color
	wxColourDialog dialog(this, &data);

	// user selected a color, so update mode
	if (dialog.ShowModal() == wxID_OK)
	{
		auto newColor = dialog.GetColourData().GetColour();
		mModel->SetBrushColor(newColor);
		if (mModel->HasSelectedShape())
		{
			mModel->CreateCommand(CM_SetBrush, wxPoint());
			mModel->FinalizeCommand();
		}
	}

	UpdateUndoRedoButtons();
	mPanel->PaintNow();
}

void PaintFrame::OnMouseButton(wxMouseEvent& event)
{
	if (event.LeftDown())
	{
		// TODO: This is when the left mouse button is pressed
		// If the mouse press wasn't over a selected shape
		if (!mModel->CanMove(event.GetPosition()))
		{
			switch (mCurrentTool)
			{
			case ID_DrawRect:
				mModel->CreateCommand(CM_DrawRect, event.GetPosition());
				break;
			case ID_DrawEllipse:
				mModel->CreateCommand(CM_DrawEllipse, event.GetPosition());
				break;
			case ID_DrawLine:
				mModel->CreateCommand(CM_DrawLine, event.GetPosition());
				break;
			case ID_DrawPencil:
				mModel->CreateCommand(CM_DrawPencil, event.GetPosition());
				break;
			case ID_Selector:
				mModel->SelectShapeAtPoint(event.GetPosition());
				break;
			}
		}
		// mouse press was over a selected shape
		else
		{
			// if no move command has been initiated create one (i.e. no movement yet)
			if (!mModel->IsMoving())
			{
				mModel->CreateCommand(CM_Move, event.GetPosition());
				mModel->SetIsMoving(true);
			}
			else if (mModel->HasActiveCommand())
			{
				mModel->UpdateCommand(event.GetPosition());
			}
		}
	}
	// mouse press released so movement via dragging must be disabled until 
	// next mouse click over selected shape
	else if (event.LeftUp())
	{
		// TODO: This is when the left mouse button is released
		if (mModel->HasActiveCommand())
		{
			mModel->UpdateCommand(event.GetPosition());
			mModel->FinalizeCommand();
		}
		mModel->SetIsMoving(false);
	}

	UpdateUndoRedoButtons();
	UpdateSelectMenuOptions();
	mPanel->PaintNow();
}

void PaintFrame::OnMouseMove(wxMouseEvent& event)
{
	// TODO: This is when the mouse is moved inside the drawable area
	if (mModel->HasActiveCommand())
	{
		mModel->UpdateCommand(event.GetPosition());
	}
	if (mModel->CanMove(event.GetPosition()))
	{
		mModel->SetCanMove(true);
		SetCursorType(CU_Move);
	}
	else
	{
		mModel->SetCanMove(false);
		SetCursorType(CU_Default);
	}
	mPanel->PaintNow();
}

void PaintFrame::ToggleTool(EventID toolID)
{
	// Deselect everything
	for (int i = ID_Selector; i <= ID_DrawPencil; i++)
	{
		mToolbar->ToggleTool(i, false);
	}

	// Select the new tool
	mToolbar->ToggleTool(toolID, true);

	mCurrentTool = toolID;
}

void PaintFrame::SetCursorType(CursorType type)
{
	wxCursor* cursor = mCursors.GetCursor(type);
	if (cursor != nullptr)
	{
		mPanel->SetCursor(*cursor);
	}
}

void PaintFrame::OnSelectTool(wxCommandEvent& event)
{
	EventID id = static_cast<EventID>(event.GetId());
	ToggleTool(id);

	// Select appropriate cursor
	switch (id)
	{
	case ID_DrawLine:
	case ID_DrawEllipse:
	case ID_DrawRect:
		SetCursorType(CU_Cross);
		break;
	case ID_DrawPencil:
		SetCursorType(CU_Pencil);
		break;
	default:
		SetCursorType(CU_Default);
		break;
	}
}

void PaintFrame::UpdateUndoRedoButtons()
{
	mEditMenu->Enable(wxID_UNDO, mModel->CanUndo());
	mToolbar->EnableTool(wxID_UNDO, mModel->CanUndo());

	mEditMenu->Enable(wxID_REDO, mModel->CanRedo());
	mToolbar->EnableTool(wxID_REDO, mModel->CanRedo());
}

void PaintFrame::UpdateSelectMenuOptions()
{
	mEditMenu->Enable(ID_Unselect, mModel->HasSelectedShape());
	mEditMenu->Enable(ID_Delete, mModel->HasSelectedShape());
}
