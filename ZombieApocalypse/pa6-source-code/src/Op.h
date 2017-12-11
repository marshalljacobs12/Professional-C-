// Defines all the classes for different operations
// which can be performed.
#pragma once
#include <vector>
#include <string>

struct MachineState;

// Abstract Base Class
class Op
{
public:
	Op(const wchar_t* OpName) 
		: mParam(-1)
		, mOpName(OpName)
	{ }
	Op(const wchar_t* OpName, int parameter)
		: mParam(parameter)
		, mOpName(OpName)
	{ }

	void DebugOutput(MachineState& state);
	void DebugRand(double x);

	virtual void Execute(MachineState& state) = 0;
protected:
	int mParam;
	const wchar_t* mOpName;
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
	OpRotate(int parameter)
		: Op(L"OpRotate", parameter)
	{ }

	void Execute(MachineState& state) override;
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
	OpGoto(int parameter)
		: Op(L"OpGoto", parameter)
	{ }

	void Execute(MachineState& state) override;
};

struct OpAttack : Op
{
	OpAttack()
		: Op(L"OpAttack")
	{ }

	void Execute(MachineState& state) override;
};

struct OpRangedAttack : Op
{
	OpRangedAttack()
		: Op(L"OpGoto")
	{ }

	void Execute(MachineState& state) override;
};

struct OpForward : Op
{
	OpForward()
		: Op(L"OpForward")
	{ }

	void Execute(MachineState& state) override;
};

struct OpEndTurn : Op
{
	OpEndTurn()
		: Op(L"OpEndTurn")
	{ }

	void Execute(MachineState& state) override;
};

struct OpTestHuman : Op
{
	OpTestHuman(int parameter)
		: Op(L"OpTestHuman", parameter)
	{ }

	void Execute(MachineState& state) override;
};

struct OpTestWall : Op
{
	OpTestWall()
		: Op(L"OpTestWall")
	{ }

	void Execute(MachineState& state) override;
};

struct OpTestZombie : Op
{
	OpTestZombie(int parameter)
		: Op(L"OpTestZombie", parameter)
	{ }

	void Execute(MachineState& state) override;
};

struct OpTestRandom : Op
{
	OpTestRandom()
		: Op(L"OpTestRandom")
	{ }

	void Execute(MachineState& state) override;
};

struct OpTestPassable : Op
{
	OpTestPassable()
		: Op(L"OpTestPassable")
	{ }

	void Execute(MachineState& state) override;
};

struct OpJe : Op
{
	OpJe(int parameter)
		: Op(L"OpJe", parameter)
	{ }

	void Execute(MachineState& state) override;
};

struct OpJne : Op
{
	OpJne(int parameter)
		: Op(L"OpJne", parameter)
	{ }

	void Execute(MachineState& state) override;
};
