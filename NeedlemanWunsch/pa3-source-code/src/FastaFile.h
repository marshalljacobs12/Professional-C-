#pragma once
#include <string>

class FastaFile
{
public:
	FastaFile(std::string filename);
	std::string GetHeader();
	std::string GetSequence();

private:
	std::string mHeader;
	std::string mSequence;
	void ParseFile(std::string filename);
};