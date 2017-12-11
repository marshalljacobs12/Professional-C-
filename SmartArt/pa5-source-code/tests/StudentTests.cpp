#include "catch.hpp"
#include <string>
#include <chrono>
#include <memory>
#include <stack>
#include <vector>
#include "Shape.h"
#include "Command.h"
#include "RectShape.h"
#include "EllipseShape.h"
#include "LineShape.h"
#include "PencilShape.h"
#include "DeleteCommand.h"
#include <wx/bitmap.h>
#include <wx/gdicmn.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include "PaintModel.h"

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
TEST_CASE("Function test cases", "[student]")
{
	wxPoint testPoint(1, 1);

	std::vector<std::shared_ptr<Shape>> testShapes {
		std::make_shared<RectShape>(testPoint),
		std::make_shared<EllipseShape>(testPoint),
		std::make_shared<LineShape>(testPoint),
		std::make_shared<PencilShape>(testPoint),
	};

	std::shared_ptr<PaintModel> testModel = std::make_shared<PaintModel>();

	auto testCommand = CommandFactory::Create(testModel, CM_DrawEllipse, testPoint);
	//auto testCommand2 = CommandFactory::Create(testModel, CM_DrawRect, testPoint);

	std::stack<std::shared_ptr<Command>> testUndoStack;
	testUndoStack.emplace(testCommand);

	std::stack<std::shared_ptr<Command>> testRedoStack;
	testRedoStack.emplace(testCommand);

	SECTION("test AddShapes")
	{
		std::vector<std::shared_ptr<Shape>> testVec;
		testModel->SetmShapes(testVec);
		
		testModel->AddShape(testShapes[0]);
		testModel->AddShape(testShapes[1]);
		testModel->AddShape(testShapes[2]);
		testModel->AddShape(testShapes[3]);

		auto result = testModel->GetmShapes();

		REQUIRE(result.size() == testShapes.size());
		REQUIRE(result == testShapes);
	}

	SECTION("test RemoveShapes")
	{
		std::vector<std::shared_ptr<Shape>> testVec;
		testModel->SetmShapes(testShapes);

		testModel->RemoveShape(testShapes[0]);
		testModel->RemoveShape(testShapes[1]);
		testModel->RemoveShape(testShapes[2]);

		auto result = testModel->GetmShapes();
		REQUIRE(result.size() == 1);
		REQUIRE(result[0] == testShapes[3]);
	}

	SECTION("test HasActiveCommand false at beginning")
	{
		bool result = testModel->HasActiveCommand();
		REQUIRE(!result);
	}

	SECTION("test HasActiveCommand true when assigned a command")
	{
		testModel->SetmActiveCommand(testCommand);
		bool result = testModel->HasActiveCommand();
		REQUIRE(result);
	}

	SECTION("test CreateCommand")
	{
		testModel->SetmActiveCommand(testCommand);
		testModel->SetmUndoStack(testUndoStack);
		testModel->SetmRedoStack(testRedoStack);
		testModel->FinalizeCommand();

		bool result = testModel->HasActiveCommand();
		REQUIRE(!result);

		REQUIRE(testModel->GetmUndoStack().size() == 2);
		REQUIRE(testModel->GetmRedoStack().empty());
	}

	SECTION("test CanUndo")
	{
		testModel->SetmUndoStack(testUndoStack);
		bool result = testModel->CanUndo();
		REQUIRE(result);
	}

	SECTION("test CanRedo")
	{
		testRedoStack.pop();
		testModel->SetmRedoStack(testRedoStack);
		bool result = testModel->CanRedo();
		REQUIRE(!result);
	}

	SECTION("test Undo")
	{
		testModel->SetmUndoStack(testUndoStack);
		testModel->SetmRedoStack(testRedoStack);

		testModel->Undo();

		REQUIRE(testModel->GetmUndoStack().empty());
		REQUIRE(testModel->GetmRedoStack().size() == 2);
	}

	SECTION("test Redo")
	{
		testModel->SetmUndoStack(testUndoStack);
		testModel->SetmRedoStack(testRedoStack);

		testModel->Redo();

		REQUIRE(testModel->GetmUndoStack().size() == 2);
		REQUIRE(testModel->GetmRedoStack().empty());
	}

	SECTION("test GetPen")
	{
		auto result = testModel->GetPen();
		REQUIRE(result == *wxBLACK_PEN);
	}

	SECTION("test GetBrush")
	{
		auto result = testModel->GetBrush();
		REQUIRE(result == *wxWHITE_BRUSH);
	}
	
	SECTION("test SelectShapeAtPoint")
	{
		testModel->SetmShapes(testShapes);
		testModel->SelectShapeAtPoint(wxPoint(1, 1));
		auto result = testModel->GetSelectedShape();
		REQUIRE(result == testModel->GetmShapes()[3]);
	}

	SECTION("test Unselect")
	{
		testModel->SetmShapes(testShapes);
		testModel->SelectShapeAtPoint(wxPoint(1, 1));
		auto selected = testModel->GetSelectedShape();
		testModel->Unselect();
		auto result = testModel->GetSelectedShape();
		REQUIRE(result == nullptr);
	}

	SECTION("test CanMove")
	{
		testModel->SetmShapes(testShapes);
		testModel->SelectShapeAtPoint(wxPoint(1, 1));
		bool result = testModel->CanMove(wxPoint(1, 1));
		REQUIRE(result);
	}

	SECTION("test SetCanMove")
	{
		testModel->SetCanMove(true);
		auto result = testModel->GetmMoveStatus();
		REQUIRE(result == allowed);
	}

	SECTION("test IsMoving")
	{
		testModel->SetmMoveStatus(allowed);
		bool result = testModel->IsMoving();
		REQUIRE(!result);
	}

	SECTION("test SetIsMoving")
	{
		testModel->SetIsMoving(true);
		auto result = testModel->GetmMoveStatus();
		REQUIRE(result == moving);
	}
}
