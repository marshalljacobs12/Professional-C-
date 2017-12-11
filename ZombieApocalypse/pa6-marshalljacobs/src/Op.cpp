#include "Op.h"
#include "Machine.h"
#include "World.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include <wx/log.h>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	wxLogDebug("%d:%s,%d", state.mProgramCounter, mOpName, mParam);
	wxLogDebug("(%d,%d)", state.mX, state.mY);
}

// For debugging test_random
void Op::DebugRand(double x)
{
	//wxLogDebug("%f", x);
}

void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT):
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN):
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	case (MachineState::LEFT):
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter = mParam;
}

void OpAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	Machine<ZombieTraits> zombieMachine;
	Machine<HumanTraits> humanMachine;
	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (World::Get().IsLocationOccupied(state.mX, state.mY - 1))
		{
			// zombie attacking
			if (state.GetInfect())
			{
				// zombie infects human
				if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY - 1))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY - 1)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
					MachineState newZombie;
					newZombie.mX = state.mX;
					newZombie.mY = state.mY - 1;
					// need to bind
					zombieMachine.BindState(newZombie);
					World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
				}
				// zombie attacks zomebie so nothing happens
				else
				{

				}
			}
			//human attacking
			else
			{
				// human kills human
				if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY - 1))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY - 1)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
				}
				// human kills zombie
				else
				{
					auto zombieStates = World::Get().GetmZombieMachineStates();
					//for (auto& i : *zombieStates)
					for (std::vector<MachineState>::const_iterator it = zombieStates->begin();
						it != zombieStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY - 1)
						{
							World::Get().GetmZombieMachineStates()->erase(it);
							break;
						}
					}
				}
			}
		}
		break;
	case (MachineState::RIGHT):
		if (World::Get().IsLocationOccupied(state.mX + 1, state.mY))
		{
			// zombie attacking
			if (state.GetInfect())
			{
				// zombie infects human
				if (World::Get().IsLocationOccupiedByHuman(state.mX + 1, state.mY))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX + 1 && (*it).mY == state.mY)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
					MachineState newZombie;
					newZombie.mX = state.mX + 1;
					newZombie.mY = state.mY;
					// need to bind
					zombieMachine.BindState(newZombie);
					World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
				}
				// zombie attacks zombie so nothing happens
				else
				{

				}
			}
			//human attacking
			else
			{
				// human kills human
				if (World::Get().IsLocationOccupiedByHuman(state.mX + 1, state.mY))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX + 1 && (*it).mY == state.mY)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
				}
				// human kills zombie
				else
				{
					auto zombieStates = World::Get().GetmZombieMachineStates();
					//for (auto& i : *zombieStates)
					for (std::vector<MachineState>::const_iterator it = zombieStates->begin();
						it != zombieStates->end();
						++it)
					{
						if ((*it).mX == state.mX + 1 && (*it).mY == state.mY)
						{
							World::Get().GetmZombieMachineStates()->erase(it);
							break;
						}
					}
				}
			}
		}
		break;
	case (MachineState::DOWN):
		if (World::Get().IsLocationOccupied(state.mX, state.mY + 1))
		{
			// zombie attacking
			if (state.GetInfect())
			{
				// zombie infects human
				if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY + 1))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY + 1)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
					MachineState newZombie;
					newZombie.mX = state.mX;
					newZombie.mY = state.mY + 1;
					// need to bind
					zombieMachine.BindState(newZombie);
					World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
				}
				// zombie attacks zombie so nothing happens
				else
				{

				}
			}
			//human attacking
			else
			{
				// human kills human
				if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY + 1))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY + 1)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
				}
				// human kills zombie
				else
				{
					auto zombieStates = World::Get().GetmZombieMachineStates();
					//for (auto& i : *zombieStates)
					for (std::vector<MachineState>::const_iterator it = zombieStates->begin();
						it != zombieStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY + 1)
						{
							World::Get().GetmZombieMachineStates()->erase(it);
							break;
						}
					}
				}
			}
		}
		break;
	case (MachineState::LEFT):
		if (World::Get().IsLocationOccupied(state.mX - 1, state.mY))
		{
			// zombie attacking
			if (state.GetInfect())
			{
				// zombie infects human
				if (World::Get().IsLocationOccupiedByHuman(state.mX - 1, state.mY))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX - 1 && (*it).mY == state.mY)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
					MachineState newZombie;
					newZombie.mX = state.mX - 1;
					newZombie.mY = state.mY;
					// need to bind
					zombieMachine.BindState(newZombie);
					World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
				}
				// zombie attacks zombie so nothing happens
				else
				{

				}
			}
			//human attacking
			else
			{
				// human kills human
				if (World::Get().IsLocationOccupiedByHuman(state.mX - 1, state.mY))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX - 1 && (*it).mY == state.mY)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
				}
				// human kills zombie
				else
				{
					auto zombieStates = World::Get().GetmZombieMachineStates();
					//for (auto& i : *zombieStates)
					for (std::vector<MachineState>::const_iterator it = zombieStates->begin();
						it != zombieStates->end();
						++it)
					{
						if ((*it).mX == state.mX - 1 && (*it).mY == state.mY)
						{
							World::Get().GetmZombieMachineStates()->erase(it);
							break;
						}
					}
				}
			}
		}
		break;
	}
	state.mActionsTaken++;
	state.mProgramCounter++;
}

void OpRangedAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	Machine<ZombieTraits> zombieMachine;
	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (World::Get().IsLocationOccupied(state.mX, state.mY - 2))
		{
			// zombie attacking
			if (state.GetInfect())
			{
				// zombie infects human
				if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY - 2))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY - 2)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
					MachineState newZombie;
					newZombie.mX = state.mX;
					newZombie.mY = state.mY - 2;
					// need to bind
					zombieMachine.BindState(newZombie);
					World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
				}
				// zombie attacks zomebie so nothing happens
				else
				{

				}
			}
			//human attacking
			else
			{
				// human kills human
				if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY - 2))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY - 2)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
				}
				// human kills zombie
				else
				{
					auto zombieStates = World::Get().GetmZombieMachineStates();
					//for (auto& i : *zombieStates)
					for (std::vector<MachineState>::const_iterator it = zombieStates->begin();
						it != zombieStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY - 2)
						{
							World::Get().GetmZombieMachineStates()->erase(it);
							break;
						}
					}
				}
			}
		}
		break;
	case (MachineState::RIGHT):
		if (World::Get().IsLocationOccupied(state.mX + 2, state.mY))
		{
			// zombie attacking
			if (state.GetInfect())
			{
				// zombie infects human
				if (World::Get().IsLocationOccupiedByHuman(state.mX + 2, state.mY))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX + 2 && (*it).mY == state.mY)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
					MachineState newZombie;
					newZombie.mX = state.mX + 2;
					newZombie.mY = state.mY;
					// need to bind
					zombieMachine.BindState(newZombie);
					World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
				}
				// zombie attacks zombie so nothing happens
				else
				{

				}
			}
			//human attacking
			else
			{
				// human kills human
				if (World::Get().IsLocationOccupiedByHuman(state.mX + 2, state.mY))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX + 2 && (*it).mY == state.mY)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
				}
				// human kills zombie
				else
				{
					auto zombieStates = World::Get().GetmZombieMachineStates();
					//for (auto& i : *zombieStates)
					for (std::vector<MachineState>::const_iterator it = zombieStates->begin();
						it != zombieStates->end();
						++it)
					{
						if ((*it).mX == state.mX + 2 && (*it).mY == state.mY)
						{
							World::Get().GetmZombieMachineStates()->erase(it);
							break;
						}
					}
				}
			}
		}
		break;
	case (MachineState::DOWN):
		if (World::Get().IsLocationOccupied(state.mX, state.mY + 2))
		{
			// zombie attacking
			if (state.GetInfect())
			{
				// zombie infects human
				if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY + 2))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY + 2)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
					MachineState newZombie;
					newZombie.mX = state.mX;
					newZombie.mY = state.mY + 2;
					// need to bind
					zombieMachine.BindState(newZombie);
					World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
				}
				// zombie attacks zombie so nothing happens
				else
				{

				}
			}
			//human attacking
			else
			{
				// human kills human
				if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY + 2))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY + 2)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
				}
				// human kills zombie
				else
				{
					auto zombieStates = World::Get().GetmZombieMachineStates();
					//for (auto& i : *zombieStates)
					for (std::vector<MachineState>::const_iterator it = zombieStates->begin();
						it != zombieStates->end();
						++it)
					{
						if ((*it).mX == state.mX && (*it).mY == state.mY + 2)
						{
							World::Get().GetmZombieMachineStates()->erase(it);
							break;
						}
					}
				}
			}
		}
		break;
	case (MachineState::LEFT):
		if (World::Get().IsLocationOccupied(state.mX - 2, state.mY))
		{
			// zombie attacking
			if (state.GetInfect())
			{
				// zombie infects human
				if (World::Get().IsLocationOccupiedByHuman(state.mX - 2, state.mY))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX - 2 && (*it).mY == state.mY)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
					MachineState newZombie;
					newZombie.mX = state.mX - 2;
					newZombie.mY = state.mY;
					// need to bind
					zombieMachine.BindState(newZombie);
					World::Get().GetmZombieMachineStates()->emplace_back(newZombie);
				}
				// zombie attacks zombie so nothing happens
				else
				{

				}
			}
			//human attacking
			else
			{
				// human kills human
				if (World::Get().IsLocationOccupiedByHuman(state.mX - 2, state.mY))
				{
					auto survivorStates = World::Get().GetmSurvivorMachineStates();
					//for (auto& i : *survivorStates)
					for (std::vector<MachineState>::const_iterator it = survivorStates->begin();
						it != survivorStates->end();
						++it)
					{
						if ((*it).mX == state.mX - 2 && (*it).mY == state.mY)
						{
							World::Get().GetmSurvivorMachineStates()->erase(it);
							break;
						}
					}
				}
				// human kills zombie
				else
				{
					auto zombieStates = World::Get().GetmZombieMachineStates();
					//for (auto& i : *zombieStates)
					for (std::vector<MachineState>::const_iterator it = zombieStates->begin();
						it != zombieStates->end();
						++it)
					{
						if ((*it).mX == state.mX - 2 && (*it).mY == state.mY)
						{
							World::Get().GetmZombieMachineStates()->erase(it);
							break;
						}
					}
				}
			}
		}
		break;
	}
	state.mActionsTaken++;
	state.mProgramCounter++;
}

void OpForward::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.mX == 0 && state.mFacing == MachineState::LEFT)
	{
		state.mActionsTaken++;
	}
	else if (state.mY == 0 && state.mFacing == MachineState::UP)
	{
		state.mActionsTaken++;
	}
	else if (state.mX == 19 && state.mFacing == MachineState::RIGHT)
	{
		state.mActionsTaken++;
	}
	else if (state.mY == 19 && state.mFacing == MachineState::DOWN)
	{
		state.mActionsTaken++;
	}
	// NEED CHECKS FOR IF SPOT IS OCCUPIED
	else
	{
		if (state.mFacing == MachineState::LEFT)
		{
			if (!World::Get().IsLocationOccupied(state.mX - 1, state.mY))
			{
				state.mX--;
			}
		}
		else if (state.mFacing == MachineState::UP)
		{
			if (!World::Get().IsLocationOccupied(state.mX, state.mY - 1))
			{
				state.mY--;
			}
		}
		else if (state.mFacing == MachineState::RIGHT)
		{
			if (!World::Get().IsLocationOccupied(state.mX + 1, state.mY))
			{
				state.mX++;
			}
		}
		else if (state.mFacing == MachineState::DOWN)
		{
			if (!World::Get().IsLocationOccupied(state.mX, state.mY + 1))
			{
				state.mY++;
			}
		}
		state.mActionsTaken++;
	}
	state.mProgramCounter++;
}

void OpEndTurn::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mActionsTaken = state.GetActionsPerTurn();
	state.mProgramCounter++;
}

void OpTestHuman::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY - mParam))
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case (MachineState::RIGHT):
		if (World::Get().IsLocationOccupiedByHuman(state.mX + mParam, state.mY))
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case (MachineState::DOWN):
		if (World::Get().IsLocationOccupiedByHuman(state.mX, state.mY + mParam))
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case (MachineState::LEFT):
		if (World::Get().IsLocationOccupiedByHuman(state.mX - mParam, state.mY))
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	}
	state.mProgramCounter++;
}

void OpTestWall::Execute(MachineState& state)
{
	DebugOutput(state);

	if (state.mX == 0 && state.mFacing == MachineState::LEFT)
	{
		state.mTest = true;
	}
	else if (state.mX == 19 && state.mFacing == MachineState::RIGHT)
	{
		state.mTest = true;
	}
	else if (state.mY == 0 && state.mFacing == MachineState::UP)
	{
		state.mTest = true;
	}
	else if (state.mY == 19 && state.mFacing == MachineState::DOWN)
	{
		state.mTest = true;
	}
	else
	{
		state.mTest = false;
	}
	state.mProgramCounter++;
}

void OpTestZombie::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (World::Get().IsLocationOccupiedByZombie(state.mX, state.mY - mParam))
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case (MachineState::RIGHT):
		if (World::Get().IsLocationOccupiedByZombie(state.mX + mParam, state.mY))
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case (MachineState::DOWN):
		if (World::Get().IsLocationOccupiedByZombie(state.mX, state.mY + mParam))
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case (MachineState::LEFT):
		if (World::Get().IsLocationOccupiedByZombie(state.mX - mParam, state.mY))
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	}
	state.mProgramCounter++;
}

void OpTestRandom::Execute(MachineState& state)
{
	DebugOutput(state);
	double x = World::Get().GenRandomDouble();
	DebugRand(x);
	if (x > 0.5)
	{
		state.mTest = true;
	}
	else
	{
		state.mTest = false;
	}
	state.mProgramCounter++;
}

void OpTestPassable::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (state.mY == 0 || World::Get().IsLocationOccupied(state.mX, state.mY - 1))
		{
			state.mTest = false;
		}
		else
		{
			state.mTest = true;
		}
		break;
	case (MachineState::RIGHT):
		if (state.mX == 19 || World::Get().IsLocationOccupied(state.mX + 1, state.mY))
		{
			state.mTest = false;
		}
		else
		{
			state.mTest = true;
		}
		break;
	case (MachineState::DOWN):
		if (state.mY == 19 || World::Get().IsLocationOccupied(state.mX, state.mY + 1))
		{
			state.mTest = false;
		}
		else
		{
			state.mTest = true;
		}
		break;
	case (MachineState::LEFT):
		if (state.mX == 0 || World::Get().IsLocationOccupied(state.mX - 1, state.mY))
		{
			state.mTest = false;
		}
		else
		{
			state.mTest = true;
		}
		break;
	}
	state.mProgramCounter++;
}

void OpJe::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.mTest)
	{
		state.mProgramCounter = mParam;
	}
	else
	{
		state.mProgramCounter++;
	}
}

void OpJne::Execute(MachineState& state)
{
	DebugOutput(state);
	if (!state.mTest)
	{
		state.mProgramCounter = mParam;
	}
	else
	{
		state.mProgramCounter++;
	}
}
