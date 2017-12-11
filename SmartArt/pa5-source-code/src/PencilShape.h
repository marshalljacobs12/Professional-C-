#pragma once
#include "Shape.h"
#include <vector>

class PencilShape : public Shape
{
public:
	PencilShape(const wxPoint& start);
	void Update(const wxPoint& newPoint) override;
	void Finalize() override;
	void Draw(wxDC& dc) const override;

private:
	std::vector<wxPoint> mPoints;
};