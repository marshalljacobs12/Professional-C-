#include "FastaFile.h"
#include <iostream>
#include <fstream>

FastaFile::FastaFile(std::string filename)
{
	ParseFile(filename);
}

void FastaFile::ParseFile(std::string filename)
{
	mSequence = "";
	std::ifstream::pos_type size;
	std::string line;
	bool didParseHeader = false;

	std::ifstream file(filename, std::ios::in | std::ios::ate);

	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file 
		mSequence.reserve(size);
		file.seekg(std::ios::beg);

		while (static_cast<bool>(std::getline(file, line)))
		{
			if (!didParseHeader)
			{
				if (line[0] == '>')
				{
					mHeader = line.substr(1);
				}
				didParseHeader = true;
			}
			else
			{
				for (int i = 0; i < line.length(); i++)
				{
					if (line[i] == 'T' || line[i] == 'C' || line[i] == 'A' || line[i] == 'G')
					{
						mSequence += line[i];
					}
				}
			}
		}
		file.close();
	}
}

std::string FastaFile::GetHeader()
{
	return mHeader;
}

std::string FastaFile::GetSequence()
{
	return mSequence;
}