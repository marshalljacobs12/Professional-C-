#include "Shape.h"
#include <algorithm>

Shape::Shape(const wxPoint& start)
	:mStartPoint(start)
	,mEndPoint(start)
	,mTopLeft(start)
	,mBotRight(start)
{
	mOffset.x = 0;
	mOffset.y = 0;
}

// Tests whether the provided point intersects
// with this shape
bool Shape::Intersects(const wxPoint& point) const
{
	bool retVal = false;
	wxPoint topleft;
	wxPoint botright;
	GetBounds(topleft, botright);
	if (point.x >= topleft.x && point.x <= botright.x &&
		point.y >= topleft.y && point.y <= botright.y)
	{
		retVal = true;
	}
	return retVal;
}

// Update shape with new provided point
void Shape::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;

	// For most shapes, we only have two points - start and end
	// So we can figure out the top left/bottom right bounds
	// based on this.
	mTopLeft.x = std::min(mStartPoint.x, mEndPoint.x);
	mTopLeft.y = std::min(mStartPoint.y, mEndPoint.y);
	mBotRight.x = std::max(mStartPoint.x, mEndPoint.x);
	mBotRight.y = std::max(mStartPoint.y, mEndPoint.y);
}

void Shape::Finalize()
{
	// Default finalize doesn't do anything
}

void Shape::GetBounds(wxPoint& topLeft, wxPoint& botRight) const
{
	topLeft = mTopLeft + mOffset;
	botRight = mBotRight + mOffset;
}

wxPen Shape::GetPen() const
{
	return mPen;
}

void Shape::SetPen(const wxPen& pen)
{
	mPen = pen;
}

wxBrush Shape::GetBrush() const
{
	return mBrush;
}

void Shape::SetBrush(const wxBrush& brush)
{
	mBrush = brush;
}

void Shape::DrawSelection(wxDC& dc) const
{
	dc.SetPen(*wxBLACK_DASHED_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	
	wxPoint dottedTopLeft(mTopLeft.x + mOffset.x - 3, mTopLeft.y + mOffset.y - 3);
	wxPoint dottedBotRight(mBotRight.x + mOffset.x + 3, mBotRight.y + mOffset.y + 3);
	dc.DrawRectangle(wxRect(dottedTopLeft, dottedBotRight));
}

wxPoint Shape::GetOffset() const
{
	return mOffset;
}

void Shape::SetOffset(const wxPoint& loc)
{
	mOffset = loc;
}

// Need to get the new starting position for each move command
wxPoint Shape::GetStart() const
{
	return mStartPoint;
}

