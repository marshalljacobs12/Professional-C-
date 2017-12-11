#include "SrcMain.h"
#include "PasswordData.h"
#include "Solver.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	PasswordData pd;

	if (argc == 3)
	{
		std::string dictionaryFile = argv[1];
		std::string passwordFile = argv[2];

		pd.LoadData(dictionaryFile, passwordFile);

		auto passwordsToBruteForce = pd.GetPasswordsToBruteForce();

		// deleted assignment operator because of mutex
		//Solver solver = Solver(passwordsToBruteForce);
		Solver solver(passwordsToBruteForce);

		//auto bruteForcedPasswords = solver.BruteForce();
		auto bruteForcedPasswords = solver.ParallelBruteForce();

		// Write all decrypted passwords from both dictionary and brute forcing to file
		pd.WriteResultsToFile(bruteForcedPasswords);
	}
}
