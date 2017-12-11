#include "SetBrushCommand.h"
#include "Shape.h"
#include "PaintModel.h"

// Probably can get rid of this
SetBrushCommand::SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	:Command(start, shape)
{
	mPrevBrush = mShape->GetBrush();
}

void SetBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mPrevBrush = mShape->GetBrush();
	mShape->SetBrush(model->GetBrush());
}

void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
	auto temp = mShape->GetBrush();
	mShape->SetBrush(mPrevBrush);
	mPrevBrush = temp;
}

void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
	auto temp = mShape->GetBrush();
	mShape->SetBrush(mPrevBrush);
	mPrevBrush = temp;
}