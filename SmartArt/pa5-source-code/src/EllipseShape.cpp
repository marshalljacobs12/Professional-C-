#include "EllipseShape.h"

EllipseShape::EllipseShape(const wxPoint& start)
	:Shape(start)
{

}

void EllipseShape::Draw(wxDC& dc) const
{
	dc.SetPen(mPen);
	dc.SetBrush(mBrush);
	dc.DrawEllipse(wxRect(mTopLeft + mOffset, mBotRight + mOffset));
}