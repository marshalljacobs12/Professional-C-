#pragma once
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include "Shape.h"
#include "Command.h"
#include <wx/bitmap.h>

enum MoveStatus
{
	disallowed,
	allowed,
	moving
};

class PaintModel : public std::enable_shared_from_this<PaintModel>
{
public:
	PaintModel();
	// Draws any shapes in the model to the provided DC (draw context)
	void DrawShapes(wxDC& dc, bool showSelection = true);

	// Clear the current paint model and start fresh
	void New();

	// Add a shape to the paint model
	void AddShape(std::shared_ptr<Shape> shape);
	// Remove a shape from the paint model
	void RemoveShape(std::shared_ptr<Shape> shape);

	// Manage PaintModel updates
	bool HasActiveCommand() const;
	void CreateCommand(CommandType type, const wxPoint& start);
	void UpdateCommand(const wxPoint& newPoint);
	void FinalizeCommand();

	// Manage undo / redo
	bool CanUndo() const;
	bool CanRedo() const;
	void Undo();
	void Redo();

	// Getters / Setters for pen and brush
	int GetPenWidth() const;
	void SetPenWidth(int width);
	wxColour GetPenColor() const;
	void SetPenColor(const wxColour& color);
	wxPen GetPen() const;
	wxColour GetBrushColor() const;
	void SetBrushColor(const wxColour& color);
	wxBrush GetBrush() const;

	// Selection methods
	void SelectShapeAtPoint(const wxPoint& loc);
	bool HasSelectedShape() const;
	std::shared_ptr<Shape> GetSelectedShape() const;
	void Unselect();

	// Move methods
	bool CanMove(const wxPoint& loc) const;
	void SetCanMove(bool enabled);
	bool IsMoving() const;
	void SetIsMoving(bool enabled);

	// Import / Export methods
	void ExportDrawing(wxString filename, const wxSize& filesize);
	void ImportDrawing(wxString filename);

	// Methods for unit tests
	std::vector<std::shared_ptr<Shape>> GetmShapes() const;
	void SetmShapes(std::vector<std::shared_ptr<Shape>>& test);
	std::shared_ptr<Command> GetmActiveCommand() const;
	void SetmActiveCommand(std::shared_ptr<Command>& test);
	std::stack<std::shared_ptr<Command>> GetmUndoStack() const;
	void SetmUndoStack(std::stack<std::shared_ptr<Command>>& test);
	std::stack<std::shared_ptr<Command>> GetmRedoStack() const;
	void SetmRedoStack(std::stack<std::shared_ptr<Command>>& test);
	MoveStatus GetmMoveStatus() const;
	void SetmMoveStatus(MoveStatus test);
	wxBitmap GetmImportedBitmap() const;
	void SetmImportedBitmap(wxBitmap& test);

private:
	// Vector of all the shapes in the model
	std::vector<std::shared_ptr<Shape>> mShapes;
	std::shared_ptr<Command> mActiveCommand;
	std::stack<std::shared_ptr<Command>> mUndoStack;
	std::stack<std::shared_ptr<Command>> mRedoStack;
	wxPen mPen;
	wxBrush mBrush;
	std::shared_ptr<Shape> mSelectedShape;
	MoveStatus mMoveStatus;
	wxBitmap mImportedBitmap;
};
