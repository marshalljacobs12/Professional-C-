#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<Location> ParseInputFile(std::string inputFile)
{
	std::ifstream inputFileStream;
	std::string line;
	std::vector<Location> locations;

	inputFileStream.open(inputFile);

	if (!inputFileStream.is_open())
	{
		std::cout << "could not open " << inputFile << std::endl;
		return locations;
	}

	while (static_cast<bool>(std::getline(inputFileStream, line)))
	{
		std::istringstream ss(line);
		std::string name;
		std::string token;
		std::vector<double> coordinates;
		bool didExtractName = false;

		while (static_cast<bool>(std::getline(ss, token, ',')))
		{
			if (!didExtractName)
			{
				name = token;
				didExtractName = true;
			}
			else
			{
				coordinates.push_back(stod(token));
			}
		}
		Location location = Location(name, coordinates[0], coordinates[1]);
		locations.push_back(location);
	}
	return locations;
}

Population GenerateInitialPopulation(std::mt19937& generator, int populationSize, size_t numLocations)
{
	Population population;
	population.mMembers = std::vector<std::vector<int>>(populationSize);

	std::for_each(population.mMembers.begin(), population.mMembers.end(), 
		[&numLocations, &generator](std::vector<int>& a) {
		int i = 0;
		a = std::vector<int>(numLocations);
		std::generate(a.begin(), a.end(), 
			[&i]() {
			return i++;
		});
		std::shuffle(a.begin() + 1, a.end(), generator);
	});
	
	return population;
}

Location ConvertToRadians(const Location& loc)
{
	Location locRadians(loc.mName, loc.mLatitude, loc.mLongitude);
	locRadians.mLatitude = loc.mLatitude * 0.0174533;
	locRadians.mLongitude = loc.mLongitude * 0.0174533;
	return locRadians;
}

double CalculateHaversineDistance(const Location& loc1, const Location& loc2)
{
	Location locA = ConvertToRadians(loc1);
	Location locB = ConvertToRadians(loc2);
	double dLongitude = locB.mLongitude - locA.mLongitude;
	double dLatitude = locB.mLatitude - locA.mLatitude;
	double a = pow(sin(dLatitude / 2.0), 2.0) + cos(locA.mLatitude) * cos(locB.mLatitude) * pow(sin(dLongitude / 2.0), 2.0);
	double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
	double distance = 3961.0 * c;
	return distance;
}

double CalculateMemberFitness(const std::vector<int>& member, const std::vector<Location>& locations)
{
	std::vector<double> haversineDistances;
	std::adjacent_difference(member.begin(), member.end(), std::back_inserter(haversineDistances), 
		[&locations](int a, int b) -> double {
		return CalculateHaversineDistance(locations[a], locations[b]);
	});

	double fitness = std::accumulate(haversineDistances.begin(), haversineDistances.end(), 0.0);

	// Need to add distance from last location in vector to location 0
	fitness += CalculateHaversineDistance(locations[member.back()], locations[member.front()]);
	return fitness;
}

std::vector<std::pair<int, double>> CalculatePopulationFitness(const Population& population, const std::vector<Location>& locations)
{
	std::vector<std::pair<int, double>> fitnessVec;
	int i = 0;
	std::transform(population.mMembers.begin(), population.mMembers.end(), std::back_inserter(fitnessVec),
		[&locations, &fitnessVec, &i](const std::vector<int>& member) {
		double fitness = CalculateMemberFitness(member, locations);
		return std::make_pair(i++, fitness);
	});
	return fitnessVec;
}

std::vector<std::pair<int, double>> SortFitnessVector(const std::vector<std::pair<int, double>>& fitnessVec)
{
	// Need to make a copy of fitnessVec because it is passed by constant reference
	std::vector<std::pair<int, double>> sortedVec = fitnessVec;
	std::sort(sortedVec.begin(), sortedVec.end(), [](std::pair<int, double> a, std::pair<int, double>b)
	{
		return a.second < b.second;
	});
	return sortedVec;
}

std::vector<double> GenerateUniformProbabilityVector(size_t numMembers)
{
	std::vector<double> probabilityVec(numMembers);
	std::generate(probabilityVec.begin(), probabilityVec.end(), 
		[&numMembers]() {
		return 1.0 / numMembers;
	});
	return probabilityVec;
}

std::vector<double> GenerateFitnessProbabilityVector(const std::vector<std::pair<int, double>>& fitnessVec)
{
	auto fitnessProbVec = GenerateUniformProbabilityVector(fitnessVec.size());

	std::for_each(fitnessVec.begin(), fitnessVec.begin() + 2, 
		[&fitnessProbVec](const std::pair<int, double> & a) {
		fitnessProbVec[a.first] *= 6.0;
	});

	std::for_each(fitnessVec.begin() + 2, fitnessVec.begin() + fitnessVec.size()/2,
		[&fitnessProbVec](const std::pair<int, double> &a) {
		fitnessProbVec[a.first] *= 3.0;
	});

	double sum = std::accumulate(fitnessProbVec.begin(), fitnessProbVec.end(), 0.0);
	
	std::transform(fitnessProbVec.begin(), fitnessProbVec.end(), fitnessProbVec.begin(),
		[&sum](double a) {
		return a / sum;
	});

	return fitnessProbVec;
}

int SelectRandomParent(double rand, const std::vector<double>& fitnessProbVec)
{
	double runningTotal = 0.0;
	int selectedIndex = 0;

	auto vec = std::find_if(fitnessProbVec.begin(), fitnessProbVec.end(),
		[&runningTotal, &rand, &selectedIndex](double a) {
		runningTotal += a;
		if (rand <= runningTotal)
		{
			return true;
		}
		selectedIndex++;
		return false;
	});

	return selectedIndex;
}

std::vector<std::pair<int, int>> SelectParents(const std::vector<std::pair<int, double>>& sortedFitnessVec, const std::vector<double>& fitnessProbVec, std::mt19937& generator)
{
	std::vector<std::pair<int, int>> parentsVec(sortedFitnessVec.size());

	std::generate(parentsVec.begin(), parentsVec.end(), 
		[&sortedFitnessVec, &fitnessProbVec, &generator]() {
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		int x = SelectRandomParent(dist(generator), fitnessProbVec);
		int y = SelectRandomParent(dist(generator), fitnessProbVec);
		return std::make_pair(x, y);
	});

	return parentsVec;
}

std::vector<int> CrossoverHelper(const std::vector<int>& firstParent, const std::vector<int>& secondParent, int index)
{
	std::vector<int> result;

	// Copy first n elements of firstParent into result using std::copy_n
	int n = index + 1;
	std::copy_n(firstParent.begin(), n, std::back_inserter(result));

	// Copy over all elements of secondParent not already in result
	std::copy_if(secondParent.begin(), secondParent.end(), std::back_inserter(result),
		[&result](const int& a) {
			auto it = std::find(result.begin(), result.end(), a);
			return it == result.end();
	});

	return result;
}

std::vector<int> MutateChild(std::vector<int>& child, int mutationChance, std::mt19937& generator)
{
	double mutationPercentage = static_cast<double>(mutationChance) / 100.0;
	std::uniform_real_distribution<double> dist1(0.0, 1.0);
	double rand = dist1(generator);

	// If rand <= mutationPercentage, perform mutation by swapping values at two random indices in child
	if (rand <= mutationPercentage)
	{
		std::uniform_int_distribution<int> dist2(1, static_cast<int>(child.size()) - 1);
		int firstIndex = dist2(generator);
		int secondIndex = dist2(generator);
		std::swap(child[firstIndex], child[secondIndex]);
	}
	
	return child;
}

std::vector<int> CrossoverPair(const std::vector<int>& parentA, const std::vector<int>& parentB, int mutationChance, std::mt19937& generator)
{
	std::vector<int> child;

	std::uniform_int_distribution<int> dist1(1, static_cast<int>(parentA.size()) - 2);
	int crossoverIndex = dist1(generator);

	std::uniform_int_distribution<int> dist2(0, 1);
	int crossoverOrder = dist2(generator);

	// If crossoverOrder is 1, parentA goes first
	if (crossoverOrder == 1)
	{
		child = CrossoverHelper(parentA, parentB, crossoverIndex);
	}
	// Else, crossoverOrder is 0, so parentB goes first
	else
	{
		child = CrossoverHelper(parentB, parentA, crossoverIndex);
	}

	MutateChild(child, mutationChance, generator);

	return child;
}

Population GenerateNewPopulation(const Population& population, std::vector<std::pair<int, int>>& parents, int mutationChance, std::mt19937& generator)
{
	Population newPopulation;

	std::transform(parents.begin(), parents.end(), std::back_inserter(newPopulation.mMembers),
		[&population, &mutationChance, &generator](const std::pair<int, int>& a) {
		return CrossoverPair(population.mMembers[a.first], population.mMembers[a.second], mutationChance, generator);
	});

	return newPopulation;
}