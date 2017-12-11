#pragma once
#include "Shape.h"

class RectShape : public Shape
{
public:
	RectShape(const wxPoint& start);
	void Draw(wxDC& dc) const override;
};