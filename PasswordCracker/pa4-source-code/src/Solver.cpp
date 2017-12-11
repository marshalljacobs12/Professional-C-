#include "Solver.h"
#include "sha1.h"
#include <tbb/parallel_invoke.h>

Solver::Solver(std::vector<std::string> passwordsToDecrypt)
{
	mCountingMachineMap = {
		{0, 'a'},
		{1, 'b'},
		{2, 'c'},
		{3, 'd'},
		{4, 'e'},
		{5, 'f'},
		{6, 'g'},
		{7, 'h'},
		{8, 'i'},
		{9, 'j'},
		{10, 'k'},
		{11, 'l'},
		{12, 'm'},
		{13, 'n'},
		{14, 'o'},
		{15, 'p'},
		{16, 'q'},
		{17, 'r'},
		{18, 's'},
		{19, 't'},
		{20, 'u'},
		{21, 'v'},
		{22, 'w'},
		{23, 'x'},
		{24, 'y'},
		{25, 'z'},
		{26, '0'},
		{27, '1'},
		{28, '2'},
		{29, '3'},
		{30, '4'},
		{31, '5'},
		{32, '6'},
		{33, '7'},
		{34, '8'},
		{35, '9'},
	};
	for (auto i : passwordsToDecrypt)
	{
		mRemainingPasswordsToDecrypt.insert(i);
	}
}


std::unordered_map<std::string, std::string> Solver::BruteForce()
{
	std::string test;

	for (auto a : mRemainingPasswordsToDecrypt)
	{
		for (int i = 0; i < 36; i++)
		{
			for (int j = 0; j < 36; j++)
			{
				for (int k = 0; k < 36; k++)
				{
					for (int l = 0; l < 36; l++)
					{
						for (int m = 0; m < 4; m++)
						{
							test.clear();
							test += mCountingMachineMap[l];
							if (m >= 1)
							{
								test += mCountingMachineMap[k];
							}
							if (m >= 2)
							{
								test += mCountingMachineMap[j];
							}
							if (m >= 3)
							{
								test += mCountingMachineMap[i];
							}
							unsigned char hash[20];
							sha1::Calc(test.c_str(), test.length(), hash);
							char hexStr[41];
							sha1::ToHexString(hash, hexStr);
							if (a == hexStr)
							{
								mDecryptedPasswords.emplace(a, test);
							}
						}
					}
				}
			}
		}
	}

	return mDecryptedPasswords;
}

std::unordered_map<std::string, std::string> Solver::ParallelBruteForce()
{
	tbb::parallel_invoke(
		[this]() { ParallelBruteForceHelper(0, 3); },
		[this]() { ParallelBruteForceHelper(4, 7); },
		[this]() { ParallelBruteForceHelper(8, 11); },
		[this]() { ParallelBruteForceHelper(12, 15); },
		[this]() { ParallelBruteForceHelper(16, 19); },
		[this]() { ParallelBruteForceHelper(20, 23); },
		[this]() { ParallelBruteForceHelper(24, 27); },
		[this]() { ParallelBruteForceHelper(28, 31); },
		[this]() { ParallelBruteForceHelper(32, 35); }
	);

	return mDecryptedPasswords;
}

/* 
	removals from tbb::concurrent_unordered_set aren't threadsafe.
	fixed thread-safety concerns by using a reader-writer lock
*/
void Solver::ParallelBruteForceHelper(int start, int end)
{
	bool didCheckLength1 = false;
	bool didCheckLength2 = false;
	bool shouldCheckLength3 = true;

	std::string decrypted;

	for (int i = start; i <= end; i++)
	{
		// only check 3 character combinations once
		if (i != 0)
		{
			shouldCheckLength3 = false;
		}
		for (int j = 0; j < 36; j++)
		{
			// only check 2 character combinations once
			if (j == 1)
			{
				didCheckLength2 = true;
			}
			for (int k = 0; k < 36; k++)
			{
				// only check 1 character combinations once
				if (k == 1)
				{
					didCheckLength1 = true;
				}
				for (int l = 0; l < 36; l++)
				{
					decrypted.clear();

					if (!didCheckLength1 && shouldCheckLength3)
					{
						decrypted.clear();
						decrypted += mCountingMachineMap[l];
						unsigned char hash[20];
						sha1::Calc(decrypted.c_str(), decrypted.length(), hash);
						char hexStr[41];
						sha1::ToHexString(hash, hexStr);
						tbb::queuing_rw_mutex::scoped_lock readerLock;
						readerLock.acquire(mPasswordDeletionMutex, false);
						auto it = mRemainingPasswordsToDecrypt.find(hexStr);
						if (it != mRemainingPasswordsToDecrypt.end())
						{
							readerLock.upgrade_to_writer();
							mDecryptedPasswords.emplace(hexStr, decrypted);
							mRemainingPasswordsToDecrypt.unsafe_erase(hexStr);
							readerLock.downgrade_to_reader();
						}
						readerLock.release();
					}

					if (!didCheckLength2 && shouldCheckLength3)
					{
						decrypted.clear();
						decrypted += mCountingMachineMap[l];
						decrypted += mCountingMachineMap[k];
						unsigned char hash[20];
						sha1::Calc(decrypted.c_str(), decrypted.length(), hash);
						char hexStr[41];
						sha1::ToHexString(hash, hexStr);
						tbb::queuing_rw_mutex::scoped_lock readerLock;
						readerLock.acquire(mPasswordDeletionMutex, false);
						auto it = mRemainingPasswordsToDecrypt.find(hexStr);
						if (it != mRemainingPasswordsToDecrypt.end())
						{
							readerLock.upgrade_to_writer();
							mDecryptedPasswords.emplace(hexStr, decrypted);
							mRemainingPasswordsToDecrypt.unsafe_erase(hexStr);
							readerLock.downgrade_to_reader();
						}
						readerLock.release();
					}

					if (shouldCheckLength3)
					{
						decrypted.clear();
						decrypted += mCountingMachineMap[l];
						decrypted += mCountingMachineMap[k];
						decrypted += mCountingMachineMap[j];
						unsigned char hash[20];
						sha1::Calc(decrypted.c_str(), decrypted.length(), hash);
						char hexStr[41];
						sha1::ToHexString(hash, hexStr);
						tbb::queuing_rw_mutex::scoped_lock readerLock;
						readerLock.acquire(mPasswordDeletionMutex, false);
						auto it = mRemainingPasswordsToDecrypt.find(hexStr);
						if (it != mRemainingPasswordsToDecrypt.end())
						{
							readerLock.upgrade_to_writer();
							mDecryptedPasswords.emplace(hexStr, decrypted);
							mRemainingPasswordsToDecrypt.unsafe_erase(hexStr);
							readerLock.downgrade_to_reader();
						}
						readerLock.release();
					}
				
					decrypted.clear();
					decrypted += mCountingMachineMap[l];
					decrypted += mCountingMachineMap[k];
					decrypted += mCountingMachineMap[j];
					decrypted += mCountingMachineMap[i];
					unsigned char hash[20];
					sha1::Calc(decrypted.c_str(), decrypted.length(), hash);
					char hexStr[41];
					sha1::ToHexString(hash, hexStr);
					tbb::queuing_rw_mutex::scoped_lock readerLock;
					readerLock.acquire(mPasswordDeletionMutex, false);
					auto it = mRemainingPasswordsToDecrypt.find(hexStr);
					if (it != mRemainingPasswordsToDecrypt.end())
					{
						readerLock.upgrade_to_writer();
						mDecryptedPasswords.emplace(hexStr, decrypted);
						mRemainingPasswordsToDecrypt.unsafe_erase(hexStr);
						readerLock.downgrade_to_reader();
					}
					readerLock.release();
				}
			}
		}
	}
}