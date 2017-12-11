#pragma once
#include "FastaFile.h"
#include <string>

enum PriorCellDirection : char
{
	topleft,
	left,
	above
};

const short MATCH_SCORE = 1;
const short MISMATCH_SCORE = -1;
const short GAP_SCORE = -1;

class NeedlemanWunsch
{
public:
	NeedlemanWunsch(FastaFile& f1, FastaFile& f2)
		: mF1(f1),
		mF2(f2),
		mResult1(""),
		mResult2(""),
		mSequence1(""),
		mSequence2(""),
		mScore(0)
	{}
	void Calculate();
	void WriteResultsToFile();
	std::string GetSequence1();
	std::string GetSequence2();
	std::string GetResult1();
	std::string GetResult2();
	short GetScore();
private:
	FastaFile mF1;
	FastaFile mF2;
	std::string mSequence1;
	std::string mSequence2;
	std::string mResult1;
	std::string mResult2;
	short mScore;
};
