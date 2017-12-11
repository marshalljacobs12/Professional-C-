#include "DrawCommand.h"
#include "Shape.h"
#include "PaintModel.h"

// Is it better to just have this in header file?
DrawCommand::DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	:Command(start, shape)
{

}

void DrawCommand::Update(const wxPoint& newPoint)
{
	Command::Update(newPoint);
	mShape->Update(newPoint);
}

void DrawCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mShape->Finalize();
}

void DrawCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->RemoveShape(mShape);
}

void DrawCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->AddShape(mShape);
}