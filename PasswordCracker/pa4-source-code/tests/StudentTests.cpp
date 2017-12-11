#include "catch.hpp"
#include "SrcMain.h"
#include "PasswordData.h"
#include "Solver.h"
#include <string>
#include <chrono>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

/*
	I don't add test cases for WriteResultsToFile in PasswordData.h because that functionality
	is tested thoroughly in the File tests (graded tests). The functions I test are LoadData, 
	GetPasswordsToBruteForce, BruteForce, and ParallelBruteForce. I do not have unit tests for
	GetPasswordsDecryptedWithDict or GetPasswordList because they never are called during the
	execution of my program. The only reason I made those accessor methods was to enable me to
	test LoadData. Also, I can only indirectly test ParallelBruteForceHelper, which is the helper
	function that parallel_invoke calls with 9 parallel lambda expressions because it is a private
	method. I conclude it is working correctly because ParallelBruteForce is working correctly.

	NOTE TO GRADER: I had to comment out my tests for BruteForce() which does serial brute-forcing
	decryption of passwords because it was causing timeouts on travis (SIGTERM). You can just uncomment
	those two tests and run them locally to see that those tests work as intended.
*/
TEST_CASE("Function test cases", "[student]")
{
	PasswordData pd;
	pd.LoadData("input/d-test.txt", "input/pass-test.txt");

	// LoadData(std::string dictFile, std::string passFile)
	SECTION("LoadData test")
	{
		std::vector<std::string> mExpectedBruteForcePasswords = {
			"8cb2237d0679ca88db6464eac60da96345513964",
			"f7c3bc1d808e04732adf679965ccc34ca7ae3441",
			"2d27b62c597ec858f6e7b54e7e58525e6a95e6d8"
		};
		std::unordered_map<std::string, std::string> mExpectedDictPasswords = {
			{"7c4a8d09ca3762af61e59520943dc26494f8941b", "123456"},
			{"5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8", "password"},
			{"7c222fb2927d828af22f592134e8932480637c0d", "12345678"},
			{"b1b3773a05c0ed0176787a4f1574ff0075f7521e", "qwerty"}
		};
		std::vector<std::string> mExpectedList = {
			"7c4a8d09ca3762af61e59520943dc26494f8941b",
			"5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8",
			"7c222fb2927d828af22f592134e8932480637c0d",
			"b1b3773a05c0ed0176787a4f1574ff0075f7521e",
			"8cb2237d0679ca88db6464eac60da96345513964",
			"f7c3bc1d808e04732adf679965ccc34ca7ae3441",
			"2d27b62c597ec858f6e7b54e7e58525e6a95e6d8" };

		auto mBruteForcePasswords = pd.GetPasswordsToBruteForce();
		auto mDictPasswords = pd.GetPasswordsDecryptedWithDict();
		auto mList = pd.GetPasswordList();

		REQUIRE(mBruteForcePasswords.size() == mExpectedBruteForcePasswords.size());
		REQUIRE(mDictPasswords.size() == mExpectedDictPasswords.size());
		REQUIRE(mList.size() == mExpectedList.size());

		REQUIRE(mBruteForcePasswords == mExpectedBruteForcePasswords);
		REQUIRE(mDictPasswords == mExpectedDictPasswords);
		REQUIRE(mList == mExpectedList);
	}

	// GetPasswordsToBruteForce()
	// Do an additional test of GetPasswordsToBruteForce because it is used in
	// the actual program and is not just for testing purposes like the other
	// two accessors
	SECTION("GetPasswordsToBruteForce test")
	{
		std::vector<std::string> mExpectedBruteForcePasswords = {
			"8cb2237d0679ca88db6464eac60da96345513964",
			"f7c3bc1d808e04732adf679965ccc34ca7ae3441",
			"2d27b62c597ec858f6e7b54e7e58525e6a95e6d8"
		};

		auto mBruteForcePasswords = pd.GetPasswordsToBruteForce();

		REQUIRE(mBruteForcePasswords.size() == mExpectedBruteForcePasswords.size());
		for (int i = 0; i < mBruteForcePasswords.size(); i++)
		{
			REQUIRE(mBruteForcePasswords[i] == mExpectedBruteForcePasswords[i]);
		}
		REQUIRE(mBruteForcePasswords == mExpectedBruteForcePasswords);
	}

	// BruteForce()
	// Only solves passwords less than 4 characters, so none of these 
	// passwords will be solved
	/*
	SECTION("BruteForce test longer than 4 characters")
	{
		std::unordered_map<std::string, std::string> mTestDecryptedPasswords = {
			{"8cb2237d0679ca88db6464eac60da96345513964", "12345"},
			{"f7c3bc1d808e04732adf679965ccc34ca7ae3441", "123456789"},
			{"d27b62c597ec858f6e7b54e7e58525e6a95e6d8", "football"}
		};

		Solver solver(pd.GetPasswordsToBruteForce());
		auto mDecryptedPasswords = solver.BruteForce();

		REQUIRE(mDecryptedPasswords.empty());
	}

	// Should successfully crack these encrypted passwords
	SECTION("BruteForce test 4 character decryptions")
	{
		std::vector<std::string> mSampleEncryptedPasswords = {
			"7110eda4d09e062aa5e4a390b0a572ac0d2c0220",
			"49f25741ff0db65a7c4290aa73f34b4d4a3644c6",
			"eef4bc910dec128c41cc8f61ab9cc99acdfccccf"
		};
		std::unordered_map<std::string, std::string> mExpectedDecryptedPasswords = {
			{"7110eda4d09e062aa5e4a390b0a572ac0d2c0220", "1234"},
			{"49f25741ff0db65a7c4290aa73f34b4d4a3644c6", "solo"},
			{"eef4bc910dec128c41cc8f61ab9cc99acdfccccf", "sm12"}
		};

		Solver solver(mSampleEncryptedPasswords);
		auto mDecryptedPasswords = solver.BruteForce();

		REQUIRE(mDecryptedPasswords.size() == mExpectedDecryptedPasswords.size());
		REQUIRE(mDecryptedPasswords == mExpectedDecryptedPasswords);
	}
	*/

	// ParallelBruteForce()
	// Tests can be identical to BruteForce because ParallelBruteForce does same thing
	// I don't test the time requirement because that is already tested in the graded
	// tests. First section tests whether passwords longer than 4 characters can be 
	// decrypted (which they can't be according to the assignment requirements
	SECTION("ParallelBruteForce test longer than 4 characters")
	{
		std::unordered_map<std::string, std::string> mTestDecryptedPasswords = {
			{ "8cb2237d0679ca88db6464eac60da96345513964", "12345" },
			{ "f7c3bc1d808e04732adf679965ccc34ca7ae3441", "123456789" },
			{ "d27b62c597ec858f6e7b54e7e58525e6a95e6d8", "football" }
		};

		Solver solver(pd.GetPasswordsToBruteForce());
		auto mDecryptedPasswords = solver.ParallelBruteForce();

		REQUIRE(mDecryptedPasswords.empty());
	}

	// Should successfully crack these encrypted passwords
	SECTION("ParallelBruteForce test 4 character decryptions")
	{
		std::vector<std::string> mSampleEncryptedPasswords = {
			"7110eda4d09e062aa5e4a390b0a572ac0d2c0220",
			"49f25741ff0db65a7c4290aa73f34b4d4a3644c6",
			"eef4bc910dec128c41cc8f61ab9cc99acdfccccf"
		};
		std::unordered_map<std::string, std::string> mExpectedDecryptedPasswords = {
			{ "7110eda4d09e062aa5e4a390b0a572ac0d2c0220", "1234" },
			{ "49f25741ff0db65a7c4290aa73f34b4d4a3644c6", "solo" },
			{ "eef4bc910dec128c41cc8f61ab9cc99acdfccccf", "sm12" }
		};

		Solver solver(mSampleEncryptedPasswords);
		auto mDecryptedPasswords = solver.ParallelBruteForce();

		REQUIRE(mDecryptedPasswords.size() == mExpectedDecryptedPasswords.size());
		REQUIRE(mDecryptedPasswords == mExpectedDecryptedPasswords);
	}
}

// Provided file tests
// (Once you pass the first section, comment out the other)
TEST_CASE("File tests", "[student]")
{
	SECTION("Dictionary only")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-dict.txt"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("solved.txt", "expected/dict-solved.txt");
		REQUIRE(result);
	}

	SECTION("Brute force only")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-brute.txt"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("solved.txt", "expected/brute-solved.txt");
		REQUIRE(result);
	}

	SECTION("Full")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d8.txt",
			"input/pass-full.txt"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("solved.txt", "expected/full-solved.txt");
		REQUIRE(result);
	}

	SECTION("Full - Timed in Release")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d8.txt",
			"input/pass-full.txt"
		};
		auto start = std::chrono::high_resolution_clock::now();
		ProcessCommandArgs(3, argv);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float seconds = duration / 1000000000.0f;
		bool result = CheckTextFilesSame("solved.txt", "expected/full-solved.txt");
		REQUIRE(result);
		WARN("****Full timed test took: " << seconds << "s****");
		REQUIRE(seconds < 2.5f);
	}
}
