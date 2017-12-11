#include "catch.hpp"
#include "SrcMain.h"
#include "DNATranslate.h"
#include "FastaFile.h"
#include "NeedlemanWunsch.h"
#include <map>
#include <string>
#include <chrono>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
// Don't include unit tests for writing to file because those are tested
// in the graded test cases
TEST_CASE("Function test cases", "[student]")
{ 
	FastaFile f1("input/Small_Test1.fasta");
	FastaFile f2("input/Small_Test2.fasta");
	FastaFile f3("input/A1C1_Homo_sapiens.fasta");
	NeedlemanWunsch nw(f1, f2);
	nw.Calculate();
	// GetHeader
	SECTION("GetHeader")
	{
		std::string expected = "Small test 1";
		std::string test = f1.GetHeader();
		REQUIRE(expected == test);
	}
	// GetSequence
	SECTION("GetSequence")
	{
		std::string expected = "CGTGAATTCAT";
		std::string test = f1.GetSequence();
		REQUIRE(expected == test);
	}
	SECTION("CountAminoAcids")
	{
		std::map<char, int> expected = {
			{0, 14441},
			{'A', 523},
			{'C', 486},
			{'D', 315},
			{'E', 492},
			{'F', 945},
			{'G', 638},
			{'H', 472},
			{'I', 1048},
			{'K', 933},
			{'L', 1599},
			{'M', 1094},
			{'N', 669},
			{'P', 629},
			{'Q', 540},
			{'R', 610},
			{'S', 1264},
			{'T', 755},
			{'V', 687},
			{'W', 198},
			{'Y', 544}
		};

		std::map<char, int> test = DNATranslate::CountAminoAcids(f3.GetSequence());
		REQUIRE(test == expected);
	}
	// These getters all check that nw.Calculate() finds best sequence alignment correctly
	SECTION("GetSequence1")
	{
		std::string expected = "CGTGAATTCAT";
		std::string test = nw.GetSequence1();
		REQUIRE(expected == test);
	}
	SECTION("GetSequence2")
	{
		std::string expected = "GACTTAC";
		std::string test = nw.GetSequence2();
		REQUIRE(expected == test);
	}
	SECTION("GetResult1")
	{
		std::string expected = "CGTGAATTCAT";
		std::string test = nw.GetResult1();
		REQUIRE(expected == test);
	}
	SECTION("GetResult2")
	{
		std::string expected = "___GACTT_AC";
		std::string test = nw.GetResult2();
		REQUIRE(expected == test);
	}
	SECTION("GetScore")
	{
		short expected = -1;
		short test = nw.GetScore();
		REQUIRE(expected == test);
	}
}

// Provided amino acid tests
// (Once you pass the first section, comment out the other)
TEST_CASE("Amino Acids Tests", "[student]")
{
	SECTION("TAS2R16")
	{
		const char* argv[] = {
			"tests/tests",
			"input/TAS2R16_Homo_sapiens.fasta"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/TAS2R16_amino.txt");
		REQUIRE(result);
	}
	SECTION("A1C1")
	{
		const char* argv[] = {
			"tests/tests",
			"input/A1C1_Homo_sapiens.fasta"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/A1C1_amino.txt");
		REQUIRE(result);
	}
	SECTION("Ebola")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Zaire_ebolavirus.fasta"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/Zaire_amino.txt");
		REQUIRE(result);
	}
}

// Provided pairwise match tests
// (Once you pass the first section, comment out the other)
TEST_CASE("Pairwise Match Tests", "[student]")
{
	SECTION("Small Comparison")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Small_Test1.fasta",
			"input/Small_Test2.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.result", "sampleoutput/Small_Test_Comparison.result");
		REQUIRE(result);
	}
	SECTION("TAS2R16 Comparison")
	{
		const char* argv[] = {
			"tests/tests",
			"input/TAS2R16_Homo_sapiens.fasta",
			"input/TAS2R16_Pan_troglodytes.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.result", "sampleoutput/TAS2R16_Comparison.result");
		REQUIRE(result);
	}
	SECTION("Ebola Comparison (run this in release!!)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Zaire_ebolavirus.fasta",
			"input/Reston_ebolavirus.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.result", "sampleoutput/Ebola_Zaire_vs_Reston_Comparison.result");
		REQUIRE(result);
	}
	SECTION("Ebola Comparison - Timed (run this in release!!)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Zaire_ebolavirus.fasta",
			"input/Reston_ebolavirus.fasta"
		};
		auto start = std::chrono::high_resolution_clock::now();
		ProcessCommandArgs(3, argv);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float seconds = duration / 1000000000.0f;
	
		bool result = CheckTextFilesSame("match.result", "sampleoutput/Ebola_Zaire_vs_Reston_Comparison.result");
		REQUIRE(result);
		WARN("****Zaire timed test took: " << seconds << "s****");
		REQUIRE(seconds < 7.0f);
	}
}
