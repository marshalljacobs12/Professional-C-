#include "catch.hpp"
#include "SrcMain.h"
#include "TSP.h"
#include <string>
#include <iostream>
#include <random>

// Don't change this
#ifdef _MSC_VER
const std::string SamplePath("sampleoutput/win/");
#elif __APPLE__
const std::string SamplePath("sampleoutput/mac/");
#else
const std::string SamplePath("sampleoutput/linux/");
#endif
// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// My unit tests for all my functions
// Sanjay said it was okay to have them all as different sections within
// one test case so I opted to leave them as is
TEST_CASE("Function test cases", "[student]")
{
	std::mt19937 testGenerator(1);
	std::mt19937 testGenerator2(1);
	std::vector<Location> testLocs = ParseInputFile("input/locations2.txt");
	Population testPop = GenerateInitialPopulation(testGenerator, 5, 10);
	Population testPop2 = GenerateInitialPopulation(testGenerator2, 5, 10);
	SECTION("ParseInputFile with locations2.txt")
	{
		std::vector<Location> expected;
		Location loc = Location("JFK Airport", 40.642523, -73.778216);
		expected.push_back(loc);
		loc = Location("Empire State Building", 40.748522, -73.985505);
		expected.push_back(loc);
		loc = Location("Statue of Liberty", 40.689282, -74.044533);
		expected.push_back(loc);
		loc = Location("Times Square", 40.758871, -73.985056);
		expected.push_back(loc);
		loc = Location("9/11 Memorial", 40.711573, -74.013283);
		expected.push_back(loc);
		loc = Location("Yankee Stadium", 40.829610, -73.926164);
		expected.push_back(loc);
		loc = Location("Long Beach Boardwalk", 40.583162, -73.660001);
		expected.push_back(loc);
		loc = Location("Madison Square Garden", 40.750610, -73.993492);
		expected.push_back(loc);
		loc = Location("Central Park", 40.771222, -73.973430);
		expected.push_back(loc);
		loc = Location("Columbia University", 40.806814, -73.962496);
		expected.push_back(loc);

		bool result = true;

		for (size_t i = 0; i < testLocs.size(); i++)
		{
			if (testLocs[i].mName.compare(expected[i].mName) != 0)
			{
				std::cout << testLocs[i].mName << std::endl;
				result = false;
			}
			else if (testLocs[i].mLatitude != Approx(expected[i].mLatitude))
			{
				std::cout << "Latitude: " << testLocs[i].mLatitude << std::endl;
				result = false;
			}
			else if (testLocs[i].mLongitude != Approx(expected[i].mLongitude))
			{
				std::cout << "Longitude: " << testLocs[i].mLongitude << std::endl;
				result = false;
			}
		}

		REQUIRE(testLocs.size() == 10);
		REQUIRE(result);
	}

	SECTION("GenerateInitialPopulation")
	{		
		Population expected;

		const int tour1[] = { 0, 4, 6, 3, 8, 1, 9, 7, 5, 2 };
		std::vector<int> expectedTour1(tour1, tour1 + sizeof(tour1) / sizeof(int));
		expected.mMembers.push_back(expectedTour1);

		const int tour2[] = { 0, 9, 2, 1, 3, 6, 8, 5, 7, 4 };
		std::vector<int> expectedTour2(tour2, tour2 + sizeof(tour2) / sizeof(int));
		expected.mMembers.push_back(expectedTour2);

		const int tour3[] = { 0, 6, 5, 8, 2, 1, 3, 4, 9, 7 };
		std::vector<int> expectedTour3(tour3, tour3 + sizeof(tour3) / sizeof(int));
		expected.mMembers.push_back(expectedTour3);

		const int tour4[] = { 0, 3, 2, 9, 1, 4, 7, 6, 5, 8 };
		std::vector<int> expectedTour4(tour4, tour4 + sizeof(tour4) / sizeof(int));
		expected.mMembers.push_back(expectedTour4);

		const int tour5[] = { 0, 8, 7, 9, 5, 2, 6, 4, 3, 1 };
		std::vector<int> expectedTour5(tour5, tour5 + sizeof(tour5) / sizeof(int));
		expected.mMembers.push_back(expectedTour5);
	
		REQUIRE(testPop.mMembers == expected.mMembers);
		REQUIRE(testPop.mMembers.size() == 5);
		for (auto i : testPop.mMembers)
		{
			REQUIRE(i.size() == 10);
		}
	}
	
	SECTION("ConvertToRadians")
	{
		Location test = Location("test", 40.0, 50.0);

		Location expected = Location("expected", 0.698132, 0.872665);

		Location result = ConvertToRadians(test);

		REQUIRE(result.mLatitude == Approx(expected.mLatitude));
		REQUIRE(result.mLongitude == Approx(expected.mLongitude));
	}
	
	SECTION("CalculateHaversineDistance")
	{
		Location test1 = Location("test1", 40.0, 50.0);
		Location test2 = Location("test2", 20.0, 30.0);

		double expected = 1820.024046;

		double result = CalculateHaversineDistance(test1, test2);

		REQUIRE(result == Approx(expected));
	}
	
	SECTION("CalculateMemberFitness")
	{
		const int tourArr[] = { 0, 1, 3, 5, 6, 2 };
		std::vector<int> testTour(tourArr, tourArr + sizeof(tourArr) / sizeof(int));

		double d1 = CalculateHaversineDistance(testLocs[0], testLocs[1]);
		double d2 = CalculateHaversineDistance(testLocs[1], testLocs[3]);
		double d3 = CalculateHaversineDistance(testLocs[3], testLocs[5]);
		double d4 = CalculateHaversineDistance(testLocs[5], testLocs[6]);
		double d5 = CalculateHaversineDistance(testLocs[6], testLocs[2]);
		double d6 = CalculateHaversineDistance(testLocs[2], testLocs[0]);

		double expected = d1 + d2 + d3 + d4 + d5 + d6; // = 77.4213

		double result = CalculateMemberFitness(testTour, testLocs);

		REQUIRE(result == Approx(expected));
	}
	
	SECTION("CalculatePopulationFitness")
	{
		std::vector<std::pair<int, double>> expected;

		double f0 = CalculateMemberFitness(testPop.mMembers[0], testLocs);
		std::pair<int, double> p0 = std::make_pair(0, f0);
		expected.push_back(p0);

		double f1 = CalculateMemberFitness(testPop.mMembers[1], testLocs);
		std::pair<int, double> p1 = std::make_pair(1, f1);
		expected.push_back(p1);

		double f2 = CalculateMemberFitness(testPop.mMembers[2], testLocs);
		std::pair<int, double> p2 = std::make_pair(2, f2);
		expected.push_back(p2);

		double f3 = CalculateMemberFitness(testPop.mMembers[3], testLocs);
		std::pair<int, double> p3 = std::make_pair(3, f3);
		expected.push_back(p3);

		double f4 = CalculateMemberFitness(testPop.mMembers[4], testLocs);
		std::pair<int, double> p4 = std::make_pair(4, f4);
		expected.push_back(p4);

		auto test = CalculatePopulationFitness(testPop, testLocs);

		REQUIRE(test.size() == expected.size());
		REQUIRE(test == expected);
	}

	SECTION("SortFitnessVector")
	{
		std::vector<std::pair<int, double>> test;
		test.push_back(std::make_pair(0, 15.0));
		test.push_back(std::make_pair(1, 5.0));
		test.push_back(std::make_pair(2, 20.0));
		test.push_back(std::make_pair(3, 12.0));
		test.push_back(std::make_pair(4, 10.0));
		test.push_back(std::make_pair(5, 7.0));

		std::vector<std::pair<int, double>> expected;
		expected.push_back(std::make_pair(1, 5.0));
		expected.push_back(std::make_pair(5, 7.0));
		expected.push_back(std::make_pair(4, 10.0));
		expected.push_back(std::make_pair(3, 12.0));
		expected.push_back(std::make_pair(0, 15.0));
		expected.push_back(std::make_pair(2, 20.0));

		std::vector<std::pair<int, double>> result = SortFitnessVector(test);

		REQUIRE(result == expected);
	}

	SECTION("GenerateUniformProbabilityVector")
	{
		std::vector<double> test;
		std::vector<double> expected(5, 0.2);

		test = GenerateUniformProbabilityVector(5);

		REQUIRE(test.size() == expected.size());
		REQUIRE(test == expected);
	}
	
	SECTION("GenerateFitnessProbabilityVector")
	{
		std::vector<std::pair<int, double>> test;
		for (int i = 0; i < 6; i++)
		{
			test.push_back(std::make_pair(i, 1.0 / 6.0));
		}

		const double expectedArr[] = { (1.0/3.0), (1.0/3.0), (1.0/6.0), (1.0/18.0), (1.0/18.0), (1.0/18.0) };
		std::vector<double> expected(expectedArr, expectedArr + sizeof(expectedArr) / sizeof(double));

		std::vector<double> result = GenerateFitnessProbabilityVector(test);

		REQUIRE(result.size() == expected.size());
		for (int j = 0; j < result.size(); j++)
		{
			REQUIRE(result[j] == Approx(expected[j]));
		}
	}
	
	SECTION("SelectRandomParent")
	{
		std::vector<double> testFitnessProbVec;
		testFitnessProbVec.push_back(1.0 / 18.0);
		testFitnessProbVec.push_back(1.0 / 6.0);
		testFitnessProbVec.push_back(1.0 / 18.0);
		testFitnessProbVec.push_back(1.0 / 3.0);
		testFitnessProbVec.push_back(1.0 / 18.0);
		testFitnessProbVec.push_back(1.0 / 3.0);

		// I don't actually need to actually generate a random number to test the functionality of 
		// SelectRandomParent. Instead, I just need to pick an arbitrary value between 0.0 and 1.0 
		// to ensure the running total is calculated correctly and that the right index is returned
		double testRand = 0.512; 

		int expected = 3;

		int result = SelectRandomParent(testRand, testFitnessProbVec);

		REQUIRE(result == expected);
	}
	
	SECTION("SelectParents")
	{
		std::vector<std::pair<int, double>> testSortedFitnessVec;
		testSortedFitnessVec.push_back(std::make_pair(1, 5.0));
		testSortedFitnessVec.push_back(std::make_pair(5, 7.0));
		testSortedFitnessVec.push_back(std::make_pair(4, 10.0));
		testSortedFitnessVec.push_back(std::make_pair(3, 12.0));
		testSortedFitnessVec.push_back(std::make_pair(0, 15.0));
		testSortedFitnessVec.push_back(std::make_pair(2, 20.0));


		std::vector<double> testFitnessProbVec;
		testFitnessProbVec.push_back(1.0/18.0);
		testFitnessProbVec.push_back(1.0/6.0);
		testFitnessProbVec.push_back(1.0/18.0);
		testFitnessProbVec.push_back(1.0/3.0);
		testFitnessProbVec.push_back(1.0/18.0);
		testFitnessProbVec.push_back(1.0/3.0);

		/*
			The 12 random numbers generated in SelectParents using testGenerator
			0.715971
			0.802758
			0.0928008
			0.518153
			0.86502
			0.829147
			0.829603
			0.27305
			0.0592432
			0.670528
			0.593066
			0.671654
		*/

		std::vector<std::pair<int, int>> expected;

		int parent1 = SelectRandomParent(0.715971, testFitnessProbVec);
		int parent2 = SelectRandomParent(0.802758, testFitnessProbVec);
		expected.push_back(std::make_pair(parent1, parent2));

		parent1 = SelectRandomParent(0.0928008, testFitnessProbVec);
		parent2 = SelectRandomParent(0.518153, testFitnessProbVec);
		expected.push_back(std::make_pair(parent1, parent2));

		parent1 = SelectRandomParent(0.86502, testFitnessProbVec);
		parent2 = SelectRandomParent(0.829147, testFitnessProbVec);
		expected.push_back(std::make_pair(parent1, parent2));

		parent1 = SelectRandomParent(0.829603, testFitnessProbVec);
		parent2 = SelectRandomParent(0.27305, testFitnessProbVec);
		expected.push_back(std::make_pair(parent1, parent2));

		parent1 = SelectRandomParent(0.0592432, testFitnessProbVec);
		parent2 = SelectRandomParent(0.670528, testFitnessProbVec);
		expected.push_back(std::make_pair(parent1, parent2));

		parent1 = SelectRandomParent(0.593066, testFitnessProbVec);
		parent2 = SelectRandomParent(0.671654, testFitnessProbVec);
		expected.push_back(std::make_pair(parent1, parent2));

		auto result = SelectParents(testSortedFitnessVec, testFitnessProbVec, testGenerator);

		REQUIRE(result == expected);
	}
	
	SECTION("CrossoverHelper")
	{
		std::vector<int> testFirstParent;
		testFirstParent.push_back(0);
		testFirstParent.push_back(3);
		testFirstParent.push_back(6);
		testFirstParent.push_back(2);
		testFirstParent.push_back(9);
		testFirstParent.push_back(1);
		testFirstParent.push_back(4);
		testFirstParent.push_back(8);
		testFirstParent.push_back(7);
		testFirstParent.push_back(5);

		std::vector<int> testSecondParent;
		testSecondParent.push_back(0);
		testSecondParent.push_back(1);
		testSecondParent.push_back(7);
		testSecondParent.push_back(3);
		testSecondParent.push_back(4);
		testSecondParent.push_back(6);
		testSecondParent.push_back(2);
		testSecondParent.push_back(9);
		testSecondParent.push_back(8);
		testSecondParent.push_back(5);

		int testCrossoverIndex = 3;

		std::vector<int> expected;

		// testSecondParent's contribution
		expected.push_back(0);
		expected.push_back(1);
		expected.push_back(7);
		expected.push_back(3);
		// testFirstParent's contribution
		expected.push_back(6);
		expected.push_back(2);
		expected.push_back(9);
		expected.push_back(4);
		expected.push_back(8);
		expected.push_back(5);

		// testSecondParent will be the parent that goes 1st. This function doesn't
		// test the random generation of a 1 or 0 to determine order so I can 
		// arbitrarily choose a parent to go first
		std::vector<int> result = CrossoverHelper(testSecondParent, testFirstParent, testCrossoverIndex);

		REQUIRE(result.size() == expected.size());
		REQUIRE(result == expected);
	}

	SECTION("MutateChild no mutation")
	{
		std::vector<int> testChild;
		testChild.push_back(0);
		testChild.push_back(3);
		testChild.push_back(6);
		testChild.push_back(2);
		testChild.push_back(9);
		testChild.push_back(1);
		testChild.push_back(4);
		testChild.push_back(7);
		testChild.push_back(8);
		testChild.push_back(5);

		std::vector<int> expected;
		expected.push_back(0);
		expected.push_back(3);
		expected.push_back(6);
		expected.push_back(2);
		expected.push_back(9);
		expected.push_back(1);
		expected.push_back(4);
		expected.push_back(7);
		expected.push_back(8);
		expected.push_back(5);

		// random value generated in MutateChild is 0.715971 so testChild should not swap any values
		int testMutationChance = 10;

		std::vector<int> result = MutateChild(testChild, testMutationChance, testGenerator);

		REQUIRE(result.size() == expected.size());
		REQUIRE(result == expected);
	}

	SECTION("MutateChild with mutation")
	{
		std::vector<int> testChild;
		testChild.push_back(0);
		testChild.push_back(3);
		testChild.push_back(6);
		testChild.push_back(2);
		testChild.push_back(9);
		testChild.push_back(1);
		testChild.push_back(4);
		testChild.push_back(7);
		testChild.push_back(8);
		testChild.push_back(5);
		testChild.push_back(11);
		testChild.push_back(10);

		std::vector<int> expected;
		expected.push_back(0);
		expected.push_back(3);
		expected.push_back(6);
		expected.push_back(1);
		expected.push_back(9);
		expected.push_back(2);
		expected.push_back(4);
		expected.push_back(7);
		expected.push_back(8);
		expected.push_back(5);
		expected.push_back(11);
		expected.push_back(10);

		// random value generated in MutateChild is 0.715971 so testChild should swap any values
		int testMutationChance = 80;

		// testGenerator will generate indices 3 and 5 as the indices to be swapped
		std::vector<int> result = MutateChild(testChild, testMutationChance, testGenerator);

		REQUIRE(result.size() == expected.size());
		REQUIRE(result == expected);
	}
	
	SECTION("CrossoverPair")
	{
		std::vector<int> testFirstParent;
		testFirstParent.push_back(0);
		testFirstParent.push_back(3);
		testFirstParent.push_back(6);
		testFirstParent.push_back(2);
		testFirstParent.push_back(9);
		testFirstParent.push_back(1);
		testFirstParent.push_back(4);
		testFirstParent.push_back(8);
		testFirstParent.push_back(7);
		testFirstParent.push_back(5);

		std::vector<int> testSecondParent;
		testSecondParent.push_back(0);
		testSecondParent.push_back(1);
		testSecondParent.push_back(7);
		testSecondParent.push_back(3);
		testSecondParent.push_back(4);
		testSecondParent.push_back(6);
		testSecondParent.push_back(2);
		testSecondParent.push_back(9);
		testSecondParent.push_back(8);
		testSecondParent.push_back(5);

		// testGenerator generates 6 as the crossover index and 1 as crossoverOrder so testFirstParent will
		// go first during the crossover. testGenerator generates 0.802758 as the cutoff for mutation so 
		// this test case will not mutate (testMutationChance = 10 so prob(mutate) = 0.1)
		std::vector<int> expected;
		// testFirstParent's contribution
		expected.push_back(0);
		expected.push_back(3);
		expected.push_back(6);
		expected.push_back(2);
		expected.push_back(9);
		expected.push_back(1);
		expected.push_back(4);
		// testSecondParent's contribution
		expected.push_back(7);
		expected.push_back(8);
		expected.push_back(5);

		int testMutationChance = 10;

		std::vector<int> result = CrossoverPair(testFirstParent, testSecondParent, testMutationChance, testGenerator);

		REQUIRE(result.size() == expected.size());
		REQUIRE(result == expected);
	}
	
	SECTION("GenerateNewPopulation")
	{
		// This is like the emblematic test case of my overall question about testing. Can I call previously
		// tested functions to avoid the headache of doing everything by hand. I'm making the assumption that
		// that is OK and how the assignment was intended to be done.

		auto testFitnessVec = CalculatePopulationFitness(testPop, testLocs); 
		auto testSortedFitnessVec = SortFitnessVector(testFitnessVec); 
		auto testFitnessProbVec = GenerateFitnessProbabilityVector(testFitnessVec); 
		auto testParentsVec = SelectParents(testSortedFitnessVec, testFitnessProbVec, testGenerator);

		auto testFitnessVec2 = CalculatePopulationFitness(testPop2, testLocs);
		auto testSortedFitnessVec2 = SortFitnessVector(testFitnessVec2);
		auto testFitnessProbVec2 = GenerateFitnessProbabilityVector(testFitnessVec2); 
		auto testParentsVec2 = SelectParents(testSortedFitnessVec2, testFitnessProbVec2, testGenerator2);

		int testMutationChance = 10;

		Population expected;
		for (int i = 0; i < testParentsVec.size(); i++)
		{
			auto child = CrossoverPair(testPop.mMembers[testParentsVec[i].first], testPop.mMembers[testParentsVec[i].second], testMutationChance, testGenerator);
			expected.mMembers.push_back(child);
		}

		// Need to pass parameters generated using testGenerator2, not testGenerator, to generate the same 
		// sequence of pseudorandom numbers that were used to create expected
		auto result = GenerateNewPopulation(testPop2, testParentsVec2, testMutationChance, testGenerator2);

		REQUIRE(result.mMembers.size() == expected.mMembers.size());
		for (int i = 0; i < result.mMembers.size(); i++)
		{
			REQUIRE(result.mMembers[i].size() == expected.mMembers[i].size());
		}
		REQUIRE(result.mMembers == expected.mMembers);
	}
	
}

// Provided Complete file test cases
// (Once you pass the first section, comment out the other)
TEST_CASE("Full file test cases", "Marshall Jacobs")
{
	
	SECTION("input/locations.txt 8 5 10 1337")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"8",
			"5",
			"10",
			"1337"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_8_5_10_1337.txt");
		REQUIRE(result);
	}
	SECTION("input/locations.txt 32 20 5 5741328")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"32",
			"20",
			"5",
			"5741328"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_32_20_5_5741328.txt");
		REQUIRE(result);
	}
	SECTION("input/locations.txt 64 500 30 12165465")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"64",
			"500",
			"30",
			"12165465"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_64_500_30_12165465.txt");
		REQUIRE(result);
	}
	SECTION("input/locations.txt 128 200 10 331988960")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"128",
			"200",
			"10",
			"331988960"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_128_200_10_331988960.txt");
		REQUIRE(result);
	}
	SECTION("input/locations.txt 256 200 20 7410785")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"256",
			"200",
			"20",
			"7410785"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_256_200_20_7410785.txt");
		REQUIRE(result);
	}
	SECTION("input/locations2.txt 64 200 25 49328573")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations2.txt",
			"64",
			"200",
			"25",
			"49328573"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_64_200_25_49328573.txt");
		REQUIRE(result);
	}
	
}
