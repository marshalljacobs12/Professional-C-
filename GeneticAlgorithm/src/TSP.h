#pragma once
#include <string>
#include <vector>
#include <random>
#include <utility>

struct Location
{
	Location(std::string name, double latitutude, double longitude) 
		: mName(std::move(name)), mLatitude(std::move(latitutude)), mLongitude(std::move(longitude)) {}
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

std::vector<Location> ParseInputFile(std::string inputFile);

Population GenerateInitialPopulation(std::mt19937& generator, int populationSize, size_t numLocations);

Location ConvertToRadians(const Location& loc);

double CalculateHaversineDistance(const Location& loc1, const Location& loc2);

double CalculateMemberFitness(const std::vector<int>& member, const std::vector<Location>& locations);

std::vector<std::pair<int, double>> CalculatePopulationFitness(const Population& population, const std::vector<Location>& locations);

std::vector<std::pair<int, double>> SortFitnessVector(const std::vector<std::pair<int, double>>& fitnessVec);

std::vector<double> GenerateUniformProbabilityVector(size_t numMembers);

std::vector<double> GenerateFitnessProbabilityVector(const std::vector<std::pair<int, double>>& fitnessVec);

int SelectRandomParent(double rand, const std::vector<double>& fitnessProbVec);

std::vector<std::pair<int, int>> SelectParents(const std::vector<std::pair<int, double>>& sortedFitnessVec, const std::vector<double>& fitnessProbVec, std::mt19937& generator);

std::vector<int> CrossoverHelper(const std::vector<int>& firstParent, const std::vector<int>& secondParent, int index);

std::vector<int> MutateChild(std::vector<int>& child, int mutationChance, std::mt19937& generator);

std::vector<int> CrossoverPair(const std::vector<int>& parentA, const std::vector<int>& parentB, int mutationChance, std::mt19937& generator);

Population GenerateNewPopulation(const Population& population, std::vector<std::pair<int, int>>& parents, int mutationChance, std::mt19937& generator);