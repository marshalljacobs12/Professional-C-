#include "catch.hpp"
#include <string>
#include <chrono>
#include "Machine.h"
#include "Op.h"
#include "Traits.h"
#include "World.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
TEST_CASE("Function test cases", "[student]")
{
	SECTION("Test Attack Op")
	{
		World::Get().InitializeTestStates();
		auto testZombieVec = World::Get().GetmZombieMachineStates();
		REQUIRE(testZombieVec->size() == 1);

		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		testHumanVec->at(0).mX = 1;
		testHumanVec->at(0).mFacing = MachineState::DOWN;

		OpAttack testOp;
		testOp.Execute(testHumanVec->at(0));

		testZombieVec = World::Get().GetmZombieMachineStates();

		REQUIRE(testHumanVec->size() == 1);
		REQUIRE(testZombieVec->empty());
	}

	SECTION("Test Ranged_Attack Op")
	{
		World::Get().InitializeTestStates();
		auto testZombieVec = World::Get().GetmZombieMachineStates();
		REQUIRE(testZombieVec->size() == 1);

		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		testHumanVec->at(0).mX = 3;
		testHumanVec->at(0).mY = 1;
		testHumanVec->at(0).mFacing = MachineState::LEFT;

		OpRangedAttack testOp;
		testOp.Execute(testHumanVec->at(0));

		testZombieVec = World::Get().GetmZombieMachineStates();

		REQUIRE(testHumanVec->size() == 1);
		REQUIRE(testZombieVec->empty());
	}

	SECTION("Test Rotate Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		REQUIRE(testHumanVec->at(0).mFacing == MachineState::UP);

		OpRotate testOp(1);
		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mFacing == MachineState::LEFT);

		OpRotate testOp2(0);
		testOp2.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mFacing == MachineState::UP);
	}

	SECTION("Test Forward Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		testHumanVec->at(0).mX = 1;

		OpForward testOp;
		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mX == 1);
		REQUIRE(testHumanVec->at(0).mY == 0);

		testHumanVec->at(0).mFacing = MachineState::DOWN;

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mX == 1);
		REQUIRE(testHumanVec->at(0).mY == 0);

		testHumanVec->at(0).mFacing = MachineState::RIGHT;

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mX == 2);
		REQUIRE(testHumanVec->at(0).mY == 0);
	}

	SECTION("Test Endturn Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();

		OpEndTurn testOp;

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mActionsTaken == 2);
	}

	SECTION("Test Test_Human Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		testHumanVec->at(0).mX = 1;

		auto testZombieVec = World::Get().GetmZombieMachineStates();

		OpTestHuman testOp(1);

		testOp.Execute(testZombieVec->at(0));

		REQUIRE(testZombieVec->at(0).mTest);
	}

	SECTION("Test Test_Wall Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		
		OpTestWall testOp;

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mTest);
	}

	SECTION("Test Test_Zombie Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		testHumanVec->at(0).mX = 1;
		testHumanVec->at(0).mFacing = MachineState::DOWN;

		OpTestZombie testOp(1);

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mTest);
	}

	SECTION("Test Test_Random Op")
	{
		/* 
			First random double between 0 and 1 generated with seed 7410785 is 0.476421,
			so mTest flag should be set to false
		*/
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		
		OpTestRandom testOp;

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(!testHumanVec->at(0).mTest);
	}

	SECTION("Test Test_Passable Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();

		OpTestPassable testOp;

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(!testHumanVec->at(0).mTest);

		testHumanVec->at(0).mFacing = MachineState::DOWN;

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mTest);

		testHumanVec->at(0).mX = 1;
		testHumanVec->at(0).mFacing = MachineState::DOWN;

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(!testHumanVec->at(0).mTest);
	}

	SECTION("Test Je Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();
		testHumanVec->at(0).mTest = true;

		OpJe testOp(5);

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mProgramCounter == 5);
	}

	SECTION("Test Jne Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();

		OpJne testOp(5);

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mProgramCounter == 5);
	}

	SECTION("Test Goto Op")
	{
		World::Get().InitializeTestStates();
		auto testHumanVec = World::Get().GetmSurvivorMachineStates();

		OpGoto testOp(5);

		testOp.Execute(testHumanVec->at(0));

		REQUIRE(testHumanVec->at(0).mProgramCounter == 5);
	}
}
