#include "PasswordData.h"
#include "sha1.h"
#include <fstream>

void PasswordData::LoadData(std::string dictFile, std::string passFile)
{
	std::ifstream dictionaryFile(dictFile);

	if (dictionaryFile.is_open())
	{
		std::string line;

		while (static_cast<bool>(std::getline(dictionaryFile, line)))
		{
			unsigned char hash[20];
			sha1::Calc(line.c_str(), line.length(), hash);
			char hexStr[41];
			sha1::ToHexString(hash, hexStr);	
			mDict.emplace(hexStr, line);
		}
	}

	dictionaryFile.close();

	std::ifstream passwordFile(passFile);

	if (passwordFile.is_open())
	{
		std::string line;

		while (static_cast<bool>(std::getline(passwordFile, line)))
		{
			mPasswordList.push_back(line);
			auto it = mDict.find(line);
			if (it != mDict.end())
			{
				mDecryptedWithDict.emplace(line, mDict[line]);
			}
			else
			{
				mPasswordsToBruteForce.push_back(line);
			}
		}
	}

	passwordFile.close();
}

std::vector<std::string> PasswordData::GetPasswordsToBruteForce()
{
	return mPasswordsToBruteForce;
}

std::unordered_map<std::string, std::string> PasswordData::GetPasswordsDecryptedWithDict()
{
	return mDecryptedWithDict;
}

std::vector<std::string> PasswordData::GetPasswordList()
{
	return mPasswordList;
}

void PasswordData::WriteResultsToFile(std::unordered_map<std::string, std::string> bruteForcedPasswords)
{
	std::ofstream outfile("solved.txt");

	for (auto i : mPasswordList)
	{
		auto it = mDecryptedWithDict.find(i);
		auto it2 = bruteForcedPasswords.find(i);
		if (it != mDecryptedWithDict.end())
		{
			outfile << i << "," << mDecryptedWithDict[i] << std::endl;
		}
		else if (it2 != bruteForcedPasswords.end())
		{
			outfile << i << "," << bruteForcedPasswords[i] << std::endl;
		}
		else
		{
			outfile << i << ",??" << std::endl;
		}
	}

	outfile.close();
}