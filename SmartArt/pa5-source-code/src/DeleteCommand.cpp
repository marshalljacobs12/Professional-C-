#include "DeleteCommand.h"
#include "Shape.h"
#include "PaintModel.h"

// Is it better to just have this in header file?
DeleteCommand::DeleteCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	:Command(start, shape)
{

}

void DeleteCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	model->RemoveShape(mShape);
}

void DeleteCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->AddShape(mShape);
}

void DeleteCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->RemoveShape(mShape);
}