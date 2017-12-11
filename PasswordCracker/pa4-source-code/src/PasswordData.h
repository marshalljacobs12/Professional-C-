#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class PasswordData 
{
public:
	void LoadData(std::string dictFile, std::string passFile);
	std::vector<std::string> GetPasswordsToBruteForce();
	// GetPasswordsDecryptedWithDict just needed for testing purposes
	std::unordered_map<std::string, std::string> GetPasswordsDecryptedWithDict();
	// GetPasswordList just needed for testing purposes
	std::vector<std::string> GetPasswordList();
	void WriteResultsToFile(std::unordered_map<std::string, std::string> bruteForcedPasswords);

private:
	std::vector<std::string> mPasswordList;
	std::unordered_map<std::string, std::string> mDict;
	std::unordered_map<std::string, std::string> mDecryptedWithDict;
	std::vector<std::string> mPasswordsToBruteForce;
};
