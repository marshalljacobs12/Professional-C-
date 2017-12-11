#include "SetPenCommand.h"
#include "Shape.h"
#include "PaintModel.h"

// Probably can get rid of this
SetPenCommand::SetPenCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	:Command(start, shape)
{
	mPrevPen = mShape->GetPen();
}

void SetPenCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mPrevPen = mShape->GetPen();
	mShape->SetPen(model->GetPen());
}

void SetPenCommand::Undo(std::shared_ptr<PaintModel> model)
{
	auto temp = mShape->GetPen();
	mShape->SetPen(mPrevPen);
	mPrevPen = temp;
}

void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
	auto temp = mShape->GetPen();
	mShape->SetPen(mPrevPen);
	mPrevPen = temp;
}