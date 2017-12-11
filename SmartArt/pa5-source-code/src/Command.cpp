#include "Command.h"
#include "Shape.h"
#include "PaintModel.h"
#include "RectShape.h"
#include "EllipseShape.h"
#include "LineShape.h"
#include "PencilShape.h"
#include "DrawCommand.h"
#include "SetPenCommand.h"
#include "SetBrushCommand.h"
#include "DeleteCommand.h"
#include "MoveCommand.h"

Command::Command(const wxPoint& start, std::shared_ptr<Shape> shape)
	:mStartPoint(start)
	,mEndPoint(start)
	,mShape(shape)
{

}

// Called when the command is still updating (such as in the process of drawing)
void Command::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;
}

std::shared_ptr<Command> CommandFactory::Create(std::shared_ptr<PaintModel> model,
	CommandType type, const wxPoint& start)
{
	std::shared_ptr<Command> retVal;
	std::shared_ptr<Shape> shape;
	// TODO: Actually create a command based on the type passed in
	switch (type)
	{
	case CM_DrawRect:
		shape = std::make_shared<RectShape>(start);
		break;
	case CM_DrawEllipse:
		shape = std::make_shared<EllipseShape>(start);
		break;
	case CM_DrawLine:
		shape = std::make_shared<LineShape>(start);
		break;
	case CM_DrawPencil:
		shape = std::make_shared<PencilShape>(start);
		break;
	case CM_SetPen:
		if (model->HasSelectedShape())
		{
			shape = model->GetSelectedShape();
			retVal = std::make_shared<SetPenCommand>(start, shape);
			return retVal;
		}
		break;
	case CM_SetBrush:
		if (model->HasSelectedShape())
		{
			shape = model->GetSelectedShape();
			retVal = std::make_shared<SetBrushCommand>(start, shape);
			return retVal;
		}
		break;
	case CM_Delete:
		if (model->HasSelectedShape())
		{
			shape = model->GetSelectedShape();
			retVal = std::make_shared<DeleteCommand>(start, shape);
			return retVal;
		}
		break;
	case CM_Move:
		if (model->HasSelectedShape())
		{
			shape = model->GetSelectedShape();
			retVal = std::make_shared<MoveCommand>(shape->GetStart(), shape);
			//retVal = std::make_shared<MoveCommand>(start, shape);
			return retVal;
		}
		break;
	}

	shape->SetPen(model->GetPen());
	shape->SetBrush(model->GetBrush());
	retVal = std::make_shared<DrawCommand>(start, shape);
	model->AddShape(shape);

	return retVal;
}
