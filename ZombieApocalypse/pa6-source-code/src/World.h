#pragma once
#include "Singleton.h"
#include "Machine.h"
#include "Traits.h"
#include <memory>
#include <random>
#include <vector>

//#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;

class World : public Singleton<World>
{
public:
	DECLARE_SINGLETON(World);

	std::shared_ptr<std::vector<MachineState>> GetmZombieMachineStates() { return mZombieMachineStates; }
	std::shared_ptr<std::vector<MachineState>> GetmSurvivorMachineStates() { return mSurvivorMachineStates; }
	double GenRandomDouble() { return mDist(mGenerator); }
	int GetRandomPopulation() { return mDist2(mGenerator); }
	int GetRandomCoordinate() { return mDist3(mGenerator); }
	int GetRandomOrientation() { return mDist4(mGenerator); }
	void SetmTestState(MachineState& state) { mTestState = state; }
	MachineState GetmTestState() { return mTestState; }
	bool IsLocationOccupied(int x, int y) 
	{
		for (auto it = mZombieMachineStates->begin(); it != mZombieMachineStates->end(); ++it)
		{
			if ((*it).mX == x && (*it).mY == y)
			{
				return true;
			}
		}
		for (auto it = mSurvivorMachineStates->begin(); it != mSurvivorMachineStates->end(); ++it)
		{
			if ((*it).mX == x && (*it).mY == y)
			{
				return true;
			}
		}
		return false;
	}
	bool IsLocationOccupiedByZombie(int x, int y)
	{
		for (auto it = mZombieMachineStates->begin(); it != mZombieMachineStates->end(); ++it)
		{
			if ((*it).mX == x && (*it).mY == y)
			{
				return true;
			}
		}
		return false;
	}
	bool IsLocationOccupiedByHuman(int x, int y)
	{
		for (auto it = mSurvivorMachineStates->begin(); it != mSurvivorMachineStates->end(); ++it)
		{
			if ((*it).mX == x && (*it).mY == y)
			{
				return true;
			}
		}
		return false;
	}
	int GetMonth() { return mMonth; }
	void IncrementMonth() { mMonth++; }
	void ResetMonth() { mMonth = 0; }
	void InitializeTestStates()
	{
		mZombieMachineStates->clear();
		mSurvivorMachineStates->clear();

		Machine<ZombieTraits> zombieMachine;
		Machine<HumanTraits> humanMachine;

		MachineState test1;
		test1.mX = 1;
		test1.mY = 1;
		zombieMachine.BindState(test1);

		mZombieMachineStates->emplace_back(test1);

		MachineState test2;
		test2.mX = 0;
		test2.mY = 0;
		humanMachine.BindState(test2);

		mSurvivorMachineStates->emplace_back(test2);
	}
private:
	std::shared_ptr<std::vector<MachineState>> mZombieMachineStates = std::make_shared<std::vector<MachineState>>();
	std::shared_ptr<std::vector<MachineState>> mSurvivorMachineStates = std::make_shared<std::vector<MachineState>>();
	MachineState mTestState;
	std::mt19937 mGenerator{ 7410785 };
	std::uniform_real_distribution<double> mDist{ 0.0, 1.0 };
	std::uniform_int_distribution<int> mDist2{ 0, 10 };
	std::uniform_int_distribution<int> mDist3{ 0, 19 };
	std::uniform_int_distribution<int> mDist4{ 0, 3 };
	int mMonth{ 0 };
};