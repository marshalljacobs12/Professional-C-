#include "DNATranslate.h"
#include <fstream>
#include <iostream>

/*
	(A) : Alanine
	(C) : Cysteine
	(D) : Aspartic acid
	(E) : Glutamic acid
	(F) : Phenylalanine
	(G) : Glycine 
	(H) : Histidine 
	(I) : Isoleucine
	(K) : Lysine
	(L) : Leucine
	(M) : Methionine
	(N) : Asparagine
	(P) : Proline
	(Q) : Glutamine
	(R) : Arginine 
	(S) : Serine
	(T) : Threonine
	(V) : Valine 
	(W) : Tryptophan
	(Y) : Tyrosine
*/

const int DNATranslate::STATE_MACHINE[24][4] = {
	{ 0, 0, 1, 0 },
	{ 2, 0, 1, 0 },
	{ 0, 0, 1, 3 }, //also produce 'M'
	{ 4, 9, 14, 19 },
	{ 5, 6, 7, 8 },
	{ 'F', 'F', 'L', 'L' },
	{ 'S', 'S', 'S', 'S' },
	{ 'Y', 'Y', '*', '*' }, // '*' for stop codon
	{ 'C', 'C', '*', 'W' }, // '*' for stop codon
	{ 10, 11, 12, 13 },
	{ 'L', 'L', 'L', 'L' },
	{ 'P', 'P', 'P', 'P' },
	{ 'H', 'H', 'Q', 'Q' },
	{ 'R', 'R', 'R', 'R' },
	{ 15, 16, 17, 18 },
	{ 'I', 'I', 'I', 'M' },
	{ 'T', 'T', 'T', 'T' },
	{ 'N', 'N', 'K', 'K' },
	{ 'S', 'S', 'R', 'R' },
	{ 20, 21, 22, 23 },
	{ 'V', 'V', 'V', 'V' },
	{ 'A', 'A', 'A', 'A' },
	{ 'D', 'D', 'E', 'E' },
	{ 'G', 'G', 'G', 'G' },
};

std::map<char, std::string> DNATranslate::AMINO_ACIDS = {
	{0, "Total amino acids produced"},
	{'A', "Alanine"},
	{'C', "Cysteine"},
	{'D', "Aspartic acid"},
	{'E', "Glutamic acid"},
	{'F', "Phenylalanine"},
	{'G', "Glycine"},
	{'H', "Histidine"},
	{'I', "Isoleucine"},
	{'K', "Lysine"},
	{'L', "Leucine"},
	{'M', "Methionine"},
	{'N', "Asparagine"},
	{'P', "Proline"},
	{'Q', "Glutamine"},
	{'R', "Arginine"},
	{'S', "Serine"},
	{'T', "Threonine"},
	{'V', "Valine"},
	{'W', "Tryptophan"},
	{'Y', "Tyrosine"},
};

std::map<char, int> DNATranslate::CountAminoAcids(std::string sequence)
{
	std::map<char, int> counts;
	int state = 0;

	for (int i = 0; i < sequence.length(); i++)
	{
		char nucleotide = sequence[i];
		int nucleotideIndex;

		switch (nucleotide)
		{
		case 'T':
			nucleotideIndex = 0;
			break;
		case 'C':
			nucleotideIndex = 1;
			break;
		case 'A':
			nucleotideIndex = 2;
			break;
		case 'G':
			nucleotideIndex = 3;
			break;
		}

		int val = DNATranslate::STATE_MACHINE[state][nucleotideIndex];

		if (val == '*')
		{
			state = 0;
		}
		else if (val < 'A')
		{
			if (state == 2 && nucleotideIndex == 3)
			{
				counts['M']++;
				counts[0]++;
				state = 3;
			}
			else
			{
				state = val;
			}
		}
		else
		{
			counts[0]++;
			counts[val]++;
			state = 3;
		}
	}

	return counts;
}

void DNATranslate::WriteAminoAcidsCountToFile(std::string header, std::map<char, int> counts)
{
	std::ofstream file("amino.txt");
	file << header << std::endl;

	for (auto i = counts.begin(); i != counts.end(); ++i)
	{
		auto index = i->first;

		if (i->first == 0)
		{
			file << DNATranslate::AMINO_ACIDS[index] << ": " << i->second << std::endl;
		}
		else
		{

			file << "(" << i->first << ") ";
			file << DNATranslate::AMINO_ACIDS[index]  << ": " << i->second << std::endl;
		}
	}

	file.close();
}