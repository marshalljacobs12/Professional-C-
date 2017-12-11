#include "catch.hpp"
#include "SrcMain.h"
#include <string>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

TEST_CASE("Function test cases", "[student]")
{

}

TEST_CASE("Student Unoptimized Tests", "[student]")
{
	SECTION("Basic")
	{
		const char* argv[] = {
			"tests/tests",
			"input/basic.zc"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/basic.out.zom");
		REQUIRE(result);
	}
	
	SECTION("Two Statements")
	{
		const char* argv[] = {
			"tests/tests",
			"input/two_statements.zc"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/two_statements.out.zom");
		REQUIRE(result);
	}
	
	SECTION("Stationary")
	{
		const char* argv[] = {
			"tests/tests",
			"input/stationary.zc"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/stationary.out.zom");
		REQUIRE(result);
	}
	
	SECTION("Search")
	{
		const char* argv[] = {
			"tests/tests",
			"input/search.zc"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/search.out.zom");
		REQUIRE(result);
	}

	SECTION("Michonne")
	{
		const char* argv[] = {
			"tests/tests",
			"input/michonne.zc"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/michonne.out.zom");
		REQUIRE(result);
	}
	
	SECTION("The Governor")
	{
		const char* argv[] = {
			"tests/tests",
			"input/the_governor.zc"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/the_governor.out.zom");
		REQUIRE(result);
	}
}

TEST_CASE("Student Optimized Tests", "[student]")
{
	SECTION("Two Statements (optimized)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/two_statements.zc",
			"-o"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/two_statements.optimized.zom");
		REQUIRE(result);
	}

	SECTION("Stationary (optimized)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/stationary.zc",
			"-o"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/stationary.optimized.zom");
		REQUIRE(result);
	}

	SECTION("Search (optimized)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/search.zc",
			"-o"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/search.optimized.zom");
		REQUIRE(result);
	}

	SECTION("Michonne (optimized)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/michonne.zc",
			"-o"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/michonne.optimized.zom");
		REQUIRE(result);
	}

	SECTION("The Governor (optimized)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/the_governor.zc",
			"-o"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("out.zom", "expected/the_governor.optimized.zom");
		REQUIRE(result);
	}
}
