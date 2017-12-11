#pragma once
#include <wx/dc.h>

// Abstract base class for all Shapes
class Shape
{
public:
	Shape(const wxPoint& start);
	// Tests whether the provided point intersects
	// with this shape
	bool Intersects(const wxPoint& point) const;
	// Update shape with new provided point
	virtual void Update(const wxPoint& newPoint);
	// Finalize the shape -- when the user has finished drawing the shape
	virtual void Finalize();
	// Returns the top left/bottom right points of the shape
	void GetBounds(wxPoint& topLeft, wxPoint& botRight) const;
	// Draw the shape
	virtual void Draw(wxDC& dc) const = 0;

	// Getters / Setters for mPen and mBrush
	wxPen GetPen() const;
	void SetPen(const wxPen& pen);
	wxBrush GetBrush() const;
	void SetBrush(const wxBrush& brush);

	// Selection
	void DrawSelection(wxDC& dc) const;

	// Movement
	wxPoint GetOffset() const;
	void SetOffset(const wxPoint& loc);
	wxPoint GetStart() const;

protected:
	// Starting point of shape
	wxPoint mStartPoint;
	// Ending point of shape
	wxPoint mEndPoint;
	// Top left point of shape
	wxPoint mTopLeft;
	// Bottom right point of shape
	wxPoint mBotRight;
	wxPen mPen;
	wxBrush mBrush;
	// offset for move command
	wxPoint mOffset;
};
