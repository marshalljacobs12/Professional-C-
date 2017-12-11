#include "MoveCommand.h"
#include "Shape.h"
#include "PaintModel.h"

// Is it better to just have this in header file?
MoveCommand::MoveCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	:Command(start, shape)
{
	mPrevOffset = shape->GetOffset();
	mStartPoint = start;
}

void MoveCommand::Update(const wxPoint& newPoint)
{
	mShape->SetOffset(newPoint - mStartPoint);
}

void MoveCommand::Finalize(std::shared_ptr<PaintModel> model)
{
}

void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
	auto temp = mShape->GetOffset();
	mShape->SetOffset(mPrevOffset);
	mPrevOffset = temp;
}

void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{
	auto temp = mShape->GetOffset();
	mShape->SetOffset(mPrevOffset);
	mPrevOffset = temp;
}