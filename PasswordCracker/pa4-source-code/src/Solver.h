#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "tbb/queuing_rw_mutex.h"
#include "tbb/concurrent_unordered_set.h"

class Solver
{
public:
	Solver(std::vector<std::string> passwordsToDecrypt);
	std::unordered_map<std::string, std::string> BruteForce();
	std::unordered_map<std::string, std::string> ParallelBruteForce();

	// Getting rid of cppcoreguidelines-special-member-functions warning
	// Need to disallow copy constructor, assignment operator, move copy constructor, and
	// move assignment opereator because of queing_rw_mutex private data member, which is 
	// not copyable
	// CITE: https://stackoverflow.com/questions/24272641/how-to-use-a-stdmutex-in-a-class-context
	Solver(const Solver& other) = delete;
	Solver(Solver&& other) = delete;
	Solver& operator=(const Solver& other) = delete;
	Solver& operator=(Solver&& other) = delete;

	// Need a user-declared destructor to remove cppcoreguidelines-special-member-functions warning
	~Solver() = default;
private:
	void ParallelBruteForceHelper(int start, int end);
	std::unordered_map<int, char> mCountingMachineMap;
	std::unordered_map<std::string, std::string> mDecryptedPasswords;
	tbb::concurrent_unordered_set<std::string> mRemainingPasswordsToDecrypt;
	tbb::queuing_rw_mutex mPasswordDeletionMutex;
};