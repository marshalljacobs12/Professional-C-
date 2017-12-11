#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include "Op.h"
#include "Exceptions.h"
#include "Utils.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include <wx/log.h>

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;
	
	enum Facing { UP, RIGHT, DOWN, LEFT};
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(UP)
		, mTest(false)
		, mX(0)
		, mY(0)
	{
	}

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;
	// (x, y) coordinates of character's location
	int mX;
	int mY;

	int GetActionsPerTurn() const throw() { return mActionsPerTurn; }
	bool GetInfect() const throw() { return mInfectOnAttack; }
private:
	// Data which is set by the traits
	int mActionsPerTurn = 0;
	bool mInfectOnAttack = false;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Clear out the current machine
	void ClearMachine();
	
	// Load in all the ops for this machine from the specified file
	//void LoadMachine(const std::string& filename);
	void LoadMachine(const char* filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	bool HasRangedAttack();

	const std::string& GetName() const { return mName; }
private:
	std::vector<std::unique_ptr<Op>> mOps;
	std::string mName;
	bool mHasRangedAttack = false;
};

template <typename MachineTraits>
void Machine<MachineTraits>::ClearMachine()
{
	mOps.clear();
	mName.clear();
	mHasRangedAttack = false;
}

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const char* filename)
{
	std::ifstream inputFileStream;
	std::string line;
	std::string op;
	std::string paramStr;
	int parameter;
	bool hasParameter;
	int numLines = 0;

	/*
		Potential Exceptions
		1. not a ZOM file (doesn't end in .zom)
		2. File will not open
		3. File is empty
		NOTE: File is guaranteed to exist because openFileDialog checks for that
	*/

	int len = strlen(filename);
	const char *fileExtension = &filename[len - 4];
	if (strcmp(fileExtension, ".zom") != 0)
	{
		inputFileStream.close();
		throw InvalidFileException("InvalidFileException: Wrong file extension");
	}

	inputFileStream.open(filename);

	if (!inputFileStream.is_open())
	{
		inputFileStream.close();
		throw InvalidFileException("InvalidFileException: Could not open file");
	}
	else if (inputFileStream.peek() == std::ifstream::traits_type::eof())
	{
		inputFileStream.close();
		throw InvalidFileException("InvalidFileException: Empty file");
	}

	// Counting number of lines first enables me to discard Ops with invalid jumps
	while (static_cast<bool>(std::getline(inputFileStream, line)))
	{
		if (line.empty())
		{
			continue;
		}
		numLines++;
	}

	// Seek back to beginning of file
	inputFileStream.clear();
	inputFileStream.seekg(0, std::ios::beg);
	mHasRangedAttack = false;

	while (static_cast<bool>(std::getline(inputFileStream, line)))
	{
		// Skip blank lines
		if (line.empty())
		{
			continue;
		}
		// Discard Comments
		auto beginComment = line.find(';');
		if (beginComment != std::string::npos)
		{
			line = line.substr(0, beginComment);
		}

		// Separate op from parameter if present 
		auto paramSeparator = line.find(',');
		if (paramSeparator != std::string::npos)
		{
			op = line.substr(0, paramSeparator);
			paramStr = line.substr(paramSeparator + 1);
			auto compiled = op + paramStr;
			// std::stoi can throw an exception
			try
			{
				parameter = std::stoi(paramStr);
			}
			catch (std::invalid_argument&)
			{
				wxLogMessage("invalid argument");
				inputFileStream.close();
				throw InvalidOpException();
			}
			catch (std::out_of_range&)
			{
				wxLogMessage("out of range");
				inputFileStream.close();
				throw InvalidOpException();
			}
			catch (std::exception&)
			{
				wxLogMessage("exception");
				inputFileStream.close();
				throw InvalidOpException();
			}
			hasParameter = true;
		}
		else
		{
			op = line;
			hasParameter = false;
		}
		// Remove extra whitespace
		// CITE: https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
		op.erase(remove_if(op.begin(), op.end(), isspace), op.end());

		if (Utils::Get().IsInNoParamOps(op))
		{
			// If a parameter free op is passed a parameter throw an exception
			if (hasParameter)
			{
				inputFileStream.close();
				throw InvalidOpException();
			}
			else
			{
				if (op == "attack")
				{
					mOps.push_back(std::make_unique<OpAttack>());
				}
				else if (op == "ranged_attack")
				{
					mOps.push_back(std::make_unique<OpRangedAttack>());
					mHasRangedAttack = true;
				}
				else if (op == "forward")
				{
					mOps.push_back(std::make_unique<OpForward>());
				}
				else if (op == "endturn")
				{
					mOps.push_back(std::make_unique<OpEndTurn>());
				}
				else if (op == "test_wall")
				{
					mOps.push_back(std::make_unique<OpTestWall>());
				}
				else if (op == "test_random")
				{
					mOps.push_back(std::make_unique<OpTestRandom>());
				}
				else if (op == "test_passable")
				{
					mOps.push_back(std::make_unique<OpTestPassable>());
				}
			}
		}
		else if (Utils::Get().IsInOneParamOps(op))
		{
			// If an op that requires a parameter isn't given one, throw an exception
			if (!hasParameter)
			{
				inputFileStream.close();
				throw InvalidOpException();
			}
			// If parameter is invalid integer for the given command, throw an exception
			else if (!Utils::Get().IsValidParameter(op, parameter, numLines))
			{
				inputFileStream.close();
				throw InvalidOpException();
			}
			// Otherwise, add it to the machine
			else
			{
				if (op == "rotate")
				{
					mOps.push_back(std::make_unique<OpRotate>(parameter));
				}
				else if (op == "test_human")
				{
					mOps.push_back(std::make_unique<OpTestHuman>(parameter));
				}
				else if (op == "test_zombie")
				{
					mOps.push_back(std::make_unique<OpTestZombie>(parameter));
				}
				else if (op == "je")
				{
					mOps.push_back(std::make_unique<OpJe>(parameter));
				}
				else if (op == "jne")
				{
					mOps.push_back(std::make_unique<OpJne>(parameter));
				}
				else if (op == "goto")
				{
					mOps.push_back(std::make_unique<OpGoto>(parameter));
				}
			}
		}
		// Not one of the possible Ops
		else
		{
			inputFileStream.close();
			throw InvalidOpException();
		}
	}
	inputFileStream.close();

	// Set mName to name of input file (not the full path)
	std::string path(filename);
	std::size_t pos = path.find_last_of("/\\");
	mName = path.substr(pos + 1);
	wxLogDebug(mName.c_str());
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	wxLogDebug("TAKING TURN");
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
}

template <typename MachineTraits>
bool Machine<MachineTraits>::HasRangedAttack()
{
	return mHasRangedAttack;
}
