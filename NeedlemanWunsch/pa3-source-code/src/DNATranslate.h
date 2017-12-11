#pragma once
#include <map>
#include <string>

class DNATranslate
{
public:
	static const int STATE_MACHINE[24][4];
	static std::map<char, std::string> AMINO_ACIDS;
	static std::map<char, int> CountAminoAcids(std::string sequence);
	static void WriteAminoAcidsCountToFile(std::string header, std::map<char, int> counts);
};

