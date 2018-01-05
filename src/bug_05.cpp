#include "advent.h"


void BugFix<5>::solve1st()
{
	int numJumps = 0;
	std::vector<int> instructions;

	int number = 0;
	while (*mIn >> number)
	{
		instructions.push_back(number);
	}

	int instr = 0;
	while (instr >= 0 && instr < static_cast<int>(instructions.size()))
	{
		instr = instr + instructions[instr]++;
		++numJumps;
	}

	*mOut << numJumps << std::endl;
}

void BugFix<5>::solve2nd()
{
	int numJumps = 0;
	std::vector<int> instructions;

	int number = 0;
	while (*mIn >> number)
	{
		instructions.push_back(number);
	}

	int instr = 0;
	while (instr >= 0 && instr < static_cast<int>(instructions.size()))
	{
		if (instructions[instr] >= 3)
			instr = instr + instructions[instr]--;
		else
			instr = instr + instructions[instr]++;
		++numJumps;
	}

	*mOut << numJumps << std::endl;
}
