#include "Node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.emplace_back(statement);
}

void NBlock::CodeGen(CodeContext& context) const
{
	for (auto i : mStatements)
	{
		i->CodeGen(context);
	}
	if (mMainBlock)
	{
		context.mOps.emplace_back("goto,1");
		// add goto to mGotos for optimization part
		context.mGotos.insert({ static_cast<int>(context.mOps.size()), 1 });
	}
}

NNumeric::NNumeric(std::string& value)
{
	mValue = std::stoi(value);
}

NRotate::NRotate(NNumeric* dir)
	: mDir(dir)
{
}

void NRotate::CodeGen(CodeContext& context) const
{
	if (mDir->mValue == 0)
	{
		context.mOps.emplace_back("rotate,0");
	}
	else if (mDir->mValue == 1)
	{
		context.mOps.emplace_back("rotate,1");
	}
}

void NForward::CodeGen(CodeContext& context) const
{
	context.mOps.emplace_back("forward");
}

NIsHuman::NIsHuman(NNumeric* param)
	: mParam(param)
{
}

void NIsHuman::CodeGen(CodeContext& context) const
{
	context.mOps.emplace_back("test_human," + std::to_string(mParam->mValue));
}

NIsZombie::NIsZombie(NNumeric* param)
	: mParam(param)
{
}

void NIsZombie::CodeGen(CodeContext& context) const
{
	context.mOps.emplace_back("test_zombie," + std::to_string(mParam->mValue));
}

void NIsPassable::CodeGen(CodeContext& context) const
{
	context.mOps.emplace_back("test_passable");
}

void NIsRandom::CodeGen(CodeContext& context) const
{
	context.mOps.emplace_back("test_random");
}

void NIsWall::CodeGen(CodeContext& context) const
{
	context.mOps.emplace_back("test_wall");
}

void NAttack::CodeGen(CodeContext& context) const
{
	context.mOps.emplace_back("attack");
}

void NRanged::CodeGen(CodeContext& context) const
{
	context.mOps.emplace_back("ranged_attack");
}

NIfElse::NIfElse(NBoolean* nBool, NBlock* nIfBlock, NBlock* nElseBlock)
	: mBool(nBool),
	mIfBlock(nIfBlock),
	mElseBlock(nElseBlock)
{
}

void NIfElse::CodeGen(CodeContext& context) const
{
	// step 1
	mBool->CodeGen(context);
	// step 2
	int jeLoc = context.mOps.size();
	// 1 is just a placeholder value
	context.mOps.emplace_back("je,1");
	// step 3
	mElseBlock->CodeGen(context);
	// step 4
	int gotoLoc = context.mOps.size();
	// 1 is just a placeholder value
	context.mOps.emplace_back("goto,1");
	// step 5
	mIfBlock->CodeGen(context);

	// fix up jump locations
	context.mOps[jeLoc] = "je," + std::to_string(gotoLoc + 2);
	context.mOps[gotoLoc] = "goto," + std::to_string(context.mOps.size() + 1);

	// add goto to mGotos for optimization part
	context.mGotos.insert({ gotoLoc + 1, static_cast<int>(context.mOps.size() + 1) });
}
