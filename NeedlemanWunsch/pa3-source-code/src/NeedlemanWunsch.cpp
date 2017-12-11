#include "NeedlemanWunsch.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

void NeedlemanWunsch::Calculate()
{
	mSequence1 = mF1.GetSequence();
	mSequence2 = mF2.GetSequence();

	int width = static_cast<int>(mF1.GetSequence().size()) + 1;
	int height = static_cast<int>(mF2.GetSequence().size()) + 1;

	std::vector<std::vector<PriorCellDirection>> mDirectionVector(height, std::vector<PriorCellDirection>(width));
	std::vector<std::vector<short>> mScoreVector(height, std::vector<short>(width));

	for (int i = 0; i < width; i++)
	{
		mScoreVector[0][i] = -static_cast<short>(i);
		mDirectionVector[0][i] = left;
	}

	for (int j = 0; j < height; j++)
	{
		mDirectionVector[j][0] = above;
		mScoreVector[j][0] = -static_cast<short>(j);
	}

	for (int m = 1; m < height; m++)
	{
		for (int n = 1; n < width; n++)
		{
			short score;
			char a = mSequence1[n - 1];
			char b = mSequence2[m - 1];

			if (a == b)
			{
				score = MATCH_SCORE;
			}
			else
			{
				score = MISMATCH_SCORE;
			}

			short scoreAboveLeft = mScoreVector[m - 1][n - 1] + score;
			short scoreLeft = mScoreVector[m][n - 1] + GAP_SCORE;
			short scoreUp = mScoreVector[m - 1][n] + GAP_SCORE;

			if (scoreAboveLeft >= scoreLeft && scoreAboveLeft >= scoreUp)
			{
				mDirectionVector[m][n] = topleft;
				mScoreVector[m][n] = scoreAboveLeft;
			}
			else if (scoreLeft >= scoreUp)
			{
				mDirectionVector[m][n] = left;
				mScoreVector[m][n] = scoreLeft;
			}
			else
			{
				mDirectionVector[m][n] = above;
				mScoreVector[m][n] = scoreUp;
			}
		}
	}

	int x = height - 1;
	int y = width - 1;

	mResult1.reserve(mSequence1.size());
	mResult2.reserve(mSequence2.size());

	while (x != 0 || y != 0)
	{
		switch (mDirectionVector[x][y])
		{
		case topleft:
			mResult1 += mSequence1[y - 1];
			mResult2 += mSequence2[x - 1];
			x--;
			y--;
			break;
		case left:
			mResult1 += mSequence1[y - 1];
			mResult2 += "_";
			y--;
			break;
		case above:
			mResult1 += "_";
			mResult2 += mSequence2[x - 1];
			x--;
			break;
		}
	}

	std::reverse(mResult1.begin(), mResult1.end());
	std::reverse(mResult2.begin(), mResult2.end());

	mScore = mScoreVector[height - 1][width - 1];

	//std::cout << "mScore: " << mScore << std::endl;
	//std::cout << "mResult1: " << mResult1 << std::endl;
	//std::cout << "mResult2: " << mResult2 << std::endl;
}

void NeedlemanWunsch::WriteResultsToFile()
{
	std::ofstream file("match.result");

	file << "A: " << mF1.GetHeader() << std::endl;
	file << "B: " << mF2.GetHeader() << std::endl;
	file << "Score: " << mScore << std::endl;
	file << std::endl;
	file.flush();

	short index = 0;
	std::string line1;
	std::string line2;
	std::string middle;
	auto resultLength = std::max(mResult1.size(), mResult2.size());

	for (auto i = 0; i < resultLength; i++)
	{
		// Don't include null characters terminating strings
		if (i < mResult1.size())
		{
			if (mResult1[i] != '\0')
			{
				line1 += mResult1[i];
			}
		}
		if (i < mResult2.size())
		{
			if (mResult2[i] != '\0')
			{
				line2 += mResult2[i];
			}
		}
		if (i < mResult1.size() && i < mResult2.size() && mResult1[i] == mResult2[i])
		{
			if (mResult1[i] != '\0')
			{
				middle += '|';
			}
		}
		else
		{
			middle += ' ';
		}
		if (middle.size() % 70 == 0 && i != 0)
		{
			std::string matchLine = line1 + "\n" + middle + "\n" + line2 + "\n \n";
			file << matchLine;
			line1 = "";
			line2 = "";
			middle = "";
		}
	}

	std::string matchLine = line1 + "\n" + middle + "\n" + line2; // +"\n \n";
	file << matchLine << std::endl << std::endl;

	file.flush();

	file.close();
}

// Getters needed for unit testing
std::string NeedlemanWunsch::GetSequence1()
{
	return mSequence1;
}
std::string NeedlemanWunsch::GetSequence2()
{
	return mSequence2;
}
std::string NeedlemanWunsch::GetResult1()
{
	return mResult1;
}
std::string NeedlemanWunsch::GetResult2()
{
	return mResult2;
}
short NeedlemanWunsch::GetScore()
{
	return mScore;
}