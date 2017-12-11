#pragma once

#include <vector>
#include <string>
#include <map>

struct CodeContext
{
	std::vector<std::string> mOps;
	std::map<int, int> mGotos;
};

class Node
{
public:
	virtual void CodeGen(CodeContext& context) const = 0;
};

class NNumeric : public Node
{
public:
	NNumeric(std::string& value);
	int mValue;
	void CodeGen(CodeContext& context) const override { }
};

class NStatement : public Node
{
};

class NRotate : public NStatement
{
public:
	NRotate(NNumeric* dir);
	void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mDir;
};

class NForward : public NStatement
{
public:
	NForward() = default;
	void CodeGen(CodeContext& context) const override;
};

class NAttack : public NStatement
{
public:
	NAttack() = default;
	void CodeGen(CodeContext& context) const override;
};

class NRanged : public NStatement
{
public:
	NRanged() = default;
	void CodeGen(CodeContext& context) const override;
};

class NBoolean : public Node
{
};

class NIsHuman : public NBoolean
{
public:
	NIsHuman(NNumeric* param);
	void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mParam;
};

class NIsZombie : public NBoolean
{
public:
	NIsZombie(NNumeric* param);
	void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mParam;
};

class NIsPassable : public NBoolean
{
public:
	NIsPassable() = default;
	void CodeGen(CodeContext& context) const override;
};

class NIsRandom : public NBoolean
{
public:
	NIsRandom() = default;
	void CodeGen(CodeContext& context) const override;
};

class NIsWall : public NBoolean
{
public:
	NIsWall() = default;
	void CodeGen(CodeContext& context) const override;
};

class NBlock : public Node
{
public:
	NBlock() { mMainBlock = false; }
	void CodeGen(CodeContext& context) const override;
	void AddStatement(NStatement* statement);
	void SetMainBlock() { mMainBlock = true; }
	void Optimize() { mOptimize = true; }
private:
	std::vector<NStatement*> mStatements;
	bool mMainBlock;
	bool mOptimize = false;
};

class NIfElse : public NBlock
{
public:
	NIfElse(NBoolean* nBool, NBlock* nIfBlock, NBlock* nElseBlock);
	void CodeGen(CodeContext& context) const override;
private:
	NBoolean* mBool;
	NBlock* mIfBlock;
	NBlock* mElseBlock;
};
