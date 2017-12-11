#pragma once
#include "Singleton.h"
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include "Op.h"

#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;

class Utils : public Singleton<Utils>
{
public:
	bool IsInNoParamOps(std::string op) 
	{ 
		return std::find(mNoParamOps.begin(), mNoParamOps.end(), op) != mNoParamOps.end(); 
	}
	bool IsInOneParamOps(std::string op) 
	{ 
		return std::find(mOneParamOps.begin(), mOneParamOps.end(), op) != mOneParamOps.end(); 
	}
	bool IsValidParameter(std::string op, int param, int numLines)
	{
		bool result;
		if (op == "rotate")
		{
			result = true;
		}
		else if (op == "test_human" || op == "test_zombie")
		{
			if (param == 1 || param == 2)
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
		else if (op == "je" || op == "jne" || op == "goto")
		{
			if (param < 1 || param > numLines)
			{
				result = false;
			}
			else
			{
				result = true;
			}
		}
		else
		{
			result = false;
		}
		
		return result;
	}

private:
	std::vector<std::string> mNoParamOps = {
		"attack",
		"ranged_attack",
		"forward",
		"endturn",
		"test_wall",
		"test_random",
		"test_passable"
	};
	std::vector<std::string> mOneParamOps = {
		"rotate",
		"test_human",
		"test_zombie",
		"je",
		"jne",
		"goto"
	};
};