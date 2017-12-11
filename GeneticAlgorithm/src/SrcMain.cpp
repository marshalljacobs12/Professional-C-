#include "SrcMain.h"
#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>

void ProcessCommandArgs(int argc, const char* argv[])
{
	std::string inputFile = argv[1];
	int popSize = atoi(argv[2]);
	int numGenerations = atoi(argv[3]);
	int mutationChance = atoi(argv[4]); // Need to divide by 100 to get a probability
	int seed = atoi(argv[5]);
	
	std::mt19937 mGenerator(seed);

	auto locations = ParseInputFile(inputFile);

	Population population = GenerateInitialPopulation(mGenerator, popSize, locations.size());

	std::ofstream ofs("log.txt");

	ofs << "INITIAL POPULATION:" << std::endl;
	for (size_t j = 0; j <= numGenerations; j++)
	{
		if (j != 0)
		{
			ofs << "GENERATION: " << j << std::endl;
		}
		for (size_t i = 0; i < population.mMembers.size(); i++)
		{
			for (size_t j = 0; j < population.mMembers[i].size(); j++)
			{
				ofs << population.mMembers[i][j];
				// No comma following the last number on a line
				if (j != population.mMembers[i].size() - 1)
				{
					ofs << ",";
				}
			}
			ofs << std::endl;
		}

		auto fitnessVec = CalculatePopulationFitness(population, locations);
		ofs << "FITNESS:" << std::endl;
		for (auto i : fitnessVec)
		{
			ofs << i.first << ":" << i.second << std::endl;
		}

		auto sortedVec = SortFitnessVector(fitnessVec);

		auto probVec = GenerateFitnessProbabilityVector(sortedVec);

		if (j != numGenerations)
		{
			ofs << "SELECTED PAIRS:" << std::endl;
			auto parents = SelectParents(sortedVec, probVec, mGenerator);
			for (auto m : parents)
			{
				ofs << "(" << m.first << "," << m.second << ")" << std::endl;
			}

			Population newGeneration = GenerateNewPopulation(population, parents, mutationChance, mGenerator);

			population = newGeneration;
		}
		else
		{
			ofs << "SOLUTION:" << std::endl;
			std::vector<int> solution = population.mMembers[sortedVec[0].first];
			for (auto a : solution)
			{
				ofs << locations[a].mName << std::endl;
			}
			ofs << locations[0].mName << std::endl;
			ofs << "DISTANCE: " << sortedVec[0].second << " miles" << std::endl;
		}
	}
}
