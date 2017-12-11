#include "PencilShape.h"

PencilShape::PencilShape(const wxPoint& start)
	:Shape(start)
{
	mPoints.emplace_back(start);
}

void PencilShape::Update(const wxPoint& newPoint)
{
	Shape::Update(newPoint);
	mPoints.emplace_back(newPoint);
}

void PencilShape::Finalize()
{
	for (auto point : mPoints)
	{
		if (point.x < mTopLeft.x)
		{
			mTopLeft.x = point.x + mOffset.x;
		}
		if (point.y < mTopLeft.y)
		{
			mTopLeft.y = point.y + mOffset.y;
		}
		if (point.x > mBotRight.x)
		{
			mBotRight.x = point.x + mOffset.x;
		}
		if (point.y > mBotRight.y)
		{
			mBotRight.y = point.y + mOffset.y;
		}
	}
}

void PencilShape::Draw(wxDC& dc) const
{
	dc.SetPen(mPen);
	dc.SetBrush(mBrush);

	if (mPoints.size() == 1)
	{
		dc.DrawPoint(mPoints[0] + mOffset);
	}
	else
	{
		// maybe use mPoints.data()?
		dc.DrawLines(static_cast<int>(mPoints.size()), &mPoints[0], mOffset.x, mOffset.y);
	}
}