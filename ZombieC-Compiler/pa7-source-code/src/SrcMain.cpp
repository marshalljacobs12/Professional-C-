#include "SrcMain.h"
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "Node.h"
#include <fstream>

extern int gLineNumber;
extern NBlock* gMainBlock;

extern int zompilerparse(); // NOLINT
struct yy_buffer_state; // NOLINT
extern void zompiler_flush_buffer(yy_buffer_state* b); // NOLINT
extern FILE* zompilerin; // NOLINT

void ProcessCommandArgs(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << "You must pass the input file as a command line parameter." << std::endl;
		return;
	}

	// Read input from the first command line parameter
	zompilerin = fopen(argv[1], "r");
	if (zompilerin == nullptr)
	{
		std::cout << "File not found: " << argv[1] << std::endl;
		return;
	}
	zompiler_flush_buffer(nullptr);

	// Start the parse
	zompilerparse();

	CodeContext codeContext;
	if (gMainBlock != nullptr)
	{
		gMainBlock->CodeGen(codeContext);
	}
	
	// only optimize gotos if -o flag passed
	if (argc == 3)
	{
		for (auto i : codeContext.mGotos)
		{
			int k = i.first;
			int temp = i.second;
			int v = 0;
			while (codeContext.mGotos.find(temp) != codeContext.mGotos.end())
			{
				v = codeContext.mGotos[temp];
				temp = v;
			}
			if (v != 0)
			{
				codeContext.mOps[k - 1] = "goto," + std::to_string(v);
			}
		}
	}

	// Close the file stream
	fclose(zompilerin);

	std::ofstream ofs("out.zom");
	for (auto i : codeContext.mOps)
	{
		ofs << i << std::endl;
	}
	ofs.close();
}

void zompilererror(const char* s) // NOLINT
{
	std::cout << s << " on line " << gLineNumber << std::endl;
}
