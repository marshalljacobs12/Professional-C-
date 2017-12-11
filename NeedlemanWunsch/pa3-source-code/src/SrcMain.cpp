#include "SrcMain.h"
#include <iostream>
#include <map>
#include <string>
#include "FastaFile.h"
#include "DNATranslate.h"
#include "NeedlemanWunsch.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	if (argc == 2)
	{
		std::string filename = argv[1];

		FastaFile mFile(filename);

		std::string sequence = mFile.GetSequence();
		std::string header = mFile.GetHeader();

		std::map<char, int> counts = DNATranslate::CountAminoAcids(sequence);
		DNATranslate::WriteAminoAcidsCountToFile(header, counts);
	}
	else if (argc == 3)
	{
		std::string filename1 = argv[1];
		std::string filename2 = argv[2];

		FastaFile mFile1(filename1);
		FastaFile mFile2(filename2);

		NeedlemanWunsch nw(mFile1, mFile2);
		nw.Calculate();
		nw.WriteResultsToFile();
	}
}
