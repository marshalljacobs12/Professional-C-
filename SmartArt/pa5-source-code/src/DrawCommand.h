#pragma once
#include "Command.h"

class DrawCommand : public Command
{
public:
	DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
	void Update(const wxPoint& newPoint) override;
	void Finalize(std::shared_ptr<PaintModel> model) override;
	void Undo(std::shared_ptr<PaintModel> model) override;
	void Redo(std::shared_ptr<PaintModel> model) override;
};