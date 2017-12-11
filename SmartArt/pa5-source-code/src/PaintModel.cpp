#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
	mActiveCommand = nullptr;
	mPen = *wxBLACK_PEN;
	mBrush = *wxWHITE_BRUSH;
	mSelectedShape = nullptr;
	mMoveStatus = disallowed;
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
	if (mImportedBitmap.IsOk())
	{
		dc.DrawBitmap(mImportedBitmap, wxPoint());
	}
	for (auto shape : mShapes)
	{
		shape->Draw(dc);
	}
	if (mSelectedShape != nullptr)
	{
		mSelectedShape->DrawSelection(dc);
	}
}

// Clear the current paint model and start fresh
void PaintModel::New()
{
	mShapes.clear();
	mActiveCommand = nullptr;
	while (!mUndoStack.empty())
	{
		mUndoStack.pop();
	}
	while (!mRedoStack.empty())
	{
		mRedoStack.pop();
	}
	mPen = *wxBLACK_PEN;
	mBrush = *wxWHITE_BRUSH;
	mSelectedShape = nullptr;
	mMoveStatus = disallowed;
	mImportedBitmap = wxBitmap();
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
	mShapes.emplace_back(shape);
}

// Remove a shape from the paint model
// Might want to put code here for removing selection box if
// selected objects draw command is undone
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
	auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
	if (iter != mShapes.end())
	{
		if (*iter == mSelectedShape)
		{
			mSelectedShape = nullptr;
		}
		mShapes.erase(iter);
	}
}

bool PaintModel::HasActiveCommand() const
{
	return mActiveCommand != nullptr;
}

void PaintModel::CreateCommand(CommandType type, const wxPoint& start)
{
	mActiveCommand = CommandFactory::Create(shared_from_this(), type, start);
}

void PaintModel::UpdateCommand(const wxPoint& newPoint)
{
	mActiveCommand->Update(newPoint);
}

void PaintModel::FinalizeCommand()
{
	mActiveCommand->Finalize(shared_from_this());
	mUndoStack.emplace(mActiveCommand);
	mActiveCommand = nullptr;
	// Just added a command to the model, so clear the redo stack
	while (!mRedoStack.empty())
	{
		mRedoStack.pop();
	}
}

bool PaintModel::CanUndo() const
{
	return !mUndoStack.empty();
}

bool PaintModel::CanRedo() const
{
	return !mRedoStack.empty();
}

void PaintModel::Undo()
{
	mUndoStack.top()->Undo(shared_from_this());
	mRedoStack.emplace(mUndoStack.top());
	mUndoStack.pop();
}

void PaintModel::Redo()
{
	mRedoStack.top()->Redo(shared_from_this());
	mUndoStack.emplace(mRedoStack.top());
	mRedoStack.pop();
}

int PaintModel::GetPenWidth() const
{
	return mPen.GetWidth();
}

void PaintModel::SetPenWidth(int width)
{
	mPen.SetWidth(width);
}

wxColour PaintModel::GetPenColor() const
{
	return mPen.GetColour();
}

void PaintModel::SetPenColor(const wxColour& color)
{
	mPen.SetColour(color);
}

wxPen PaintModel::GetPen() const
{
	return mPen;
}

wxColour PaintModel::GetBrushColor() const
{
	return mBrush.GetColour();
}

void PaintModel::SetBrushColor(const wxColour& color)
{
	mBrush.SetColour(color);
}

wxBrush PaintModel::GetBrush() const
{
	return mBrush;
}

void PaintModel::SelectShapeAtPoint(const wxPoint& loc)
{
	for (auto it = mShapes.rbegin(); it != mShapes.rend(); ++it)
	{
		if ((*it)->Intersects(loc))
		{
			mSelectedShape = *it;
			return;
		}
	}
	mSelectedShape = nullptr;
}

bool PaintModel::HasSelectedShape() const
{
	return mSelectedShape != nullptr;
}

std::shared_ptr<Shape> PaintModel::GetSelectedShape() const
{
	return mSelectedShape;
}

void PaintModel::Unselect()
{
	mSelectedShape = nullptr;
}

// use 	bool Intersects(const wxPoint& point) const;
// from shape
bool PaintModel::CanMove(const wxPoint& loc) const
{
	if (HasSelectedShape())
	{
		if (GetSelectedShape()->Intersects(loc))
		{
			return true;
		}
	}
	return false;
}

void PaintModel::SetCanMove(bool enabled)
{
	if (enabled)
	{
		mMoveStatus = allowed;
	}
	else
	{
		mMoveStatus = disallowed;
	}
}

bool PaintModel::IsMoving() const
{
	if (mMoveStatus == moving)
	{
		return true;
	}
	return false;
}

void PaintModel::SetIsMoving(bool enabled)
{
	if (enabled)
	{
		mMoveStatus = moving;
	}
	else if (!enabled && (mMoveStatus == moving || mMoveStatus == allowed))
	{
		mMoveStatus = allowed;
	}
	else
	{
		mMoveStatus = disallowed;
	}
}

void PaintModel::ExportDrawing(wxString filename, const wxSize& filesize)
{
	wxBitmap bitmap;

	// Create the bitmap of the specified wxSize 
	bitmap.Create(filesize);
	// Create a memory DC to draw to the bitmap 
	wxMemoryDC dc(bitmap);

	// Clear the background color 
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();

	// Draw all the shapes (make sure not the selection!) 
	DrawShapes(dc);

	// Find the file extension (Default to PNG in invalid extension
	// or no extension given)
	auto extension = filename.AfterLast(wxUniChar('.'));
	wxBitmapType type;

	if (extension.Cmp(wxString("jpg")) == 0 || extension.Cmp(wxString("jpeg")) == 0)
	{
		type = wxBITMAP_TYPE_JPEG;
	}
	else if (extension.Cmp(wxString("bmp")) == 0)
	{
		type = wxBITMAP_TYPE_BMP;
	}
	else if (extension.Cmp(wxString("png")) == 0)
	{
		type = wxBITMAP_TYPE_PNG;
	}
	// Last two checks probably unnecessary
	// No period in pathname so add .png extension
	else if (extension.Length() == filename.Length())
	{
		filename += wxString(".png");
		type = wxBITMAP_TYPE_PNG;
	}
	// invalid file extension so replace with png by default
	else
	{
		filename.Truncate(filename.size() - extension.size());
		filename += wxString("png");
		type = wxBITMAP_TYPE_PNG;
	}
	// Write the bitmap with the specified file name and wxBitmapType 
	bitmap.SaveFile(filename, type);
}

void PaintModel::ImportDrawing(wxString filename)
{
	// Find the file extension (Default to PNG in invalid extension
	// or no extension given)
	auto extension = filename.AfterLast(wxUniChar('.'));
	wxBitmapType type;

	if (extension.Cmp(wxString("jpg")) == 0 || extension.Cmp(wxString("jpeg")) == 0)
	{
		type = wxBITMAP_TYPE_JPEG;
	}
	else if (extension.Cmp(wxString("bmp")) == 0)
	{
		type = wxBITMAP_TYPE_BMP;
	}
	else if (extension.Cmp(wxString("png")) == 0)
	{
		type = wxBITMAP_TYPE_PNG;
	}
	// do I need an else where I just return?

	// clear out PaintModel if a bitmap is currently loaded
	if (mImportedBitmap.IsOk())
	{
		New();
	}

	mImportedBitmap.LoadFile(filename, type);
}

// Methods for unit testing
std::vector<std::shared_ptr<Shape>> PaintModel::GetmShapes() const
{
	return mShapes;
}

void PaintModel::SetmShapes(std::vector<std::shared_ptr<Shape>>& test)
{
	mShapes = test;
}

std::shared_ptr<Command> PaintModel::GetmActiveCommand() const
{
	return mActiveCommand;
}

void PaintModel::SetmActiveCommand(std::shared_ptr<Command>& test)
{
	mActiveCommand = test;
}

std::stack<std::shared_ptr<Command>> PaintModel::GetmUndoStack() const
{
	return mUndoStack;
}

void PaintModel::SetmUndoStack(std::stack<std::shared_ptr<Command>>& test)
{
	mUndoStack = test;
}

std::stack<std::shared_ptr<Command>> PaintModel::GetmRedoStack() const
{
	return mRedoStack;
}

void PaintModel::SetmRedoStack(std::stack<std::shared_ptr<Command>>& test)
{
	mRedoStack = test;
}

MoveStatus PaintModel::GetmMoveStatus() const
{
	return mMoveStatus;
}

void PaintModel::SetmMoveStatus(MoveStatus test)
{
	mMoveStatus = test;
}

wxBitmap PaintModel::GetmImportedBitmap() const
{
	return mImportedBitmap;
}

void PaintModel::SetmImportedBitmap(wxBitmap& test)
{
	mImportedBitmap = test;
}

