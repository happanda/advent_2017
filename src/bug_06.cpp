#include <iostream>
#include <sstream>
#include <vector>
#include "advent.h"


const char* input_06 = "14	0	15	12	11	11	3	5	1	6	8	4	9	1	8	4";

void distribute(std::vector<size_t>& blocks, int ind)
{
	size_t num = blocks[ind];
	size_t const toEvery = num / blocks.size();
	num = num % blocks.size();
	blocks[ind] = 0;

	if (toEvery > 0)
	{
		for (size_t& block : blocks)
			block += toEvery;
	}
	for (size_t i = ind + 1; num > 0; ++i)
	{
		blocks[i % blocks.size()] += 1;
		--num;
	}
}

size_t findMax(std::vector<size_t> const& blocks)
{
	size_t maximum = 0;
	size_t maxInd = 0;
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		if (maximum < blocks[i])
		{
			maximum = blocks[i];
			maxInd = i;
		}
	}
	return maxInd;
}

std::string toString(std::vector<size_t> const& blocks)
{
	std::ostringstream oss;
	for (size_t const& block : blocks)
	{
		oss << block << " ";
	}
	return oss.str();
}


// Solution for both 6.0 and 6.1, but a very bad one.
// I know how to detect a loop without big history arrays,
// but couldn't come up with how to count loop size and entering step number.
// At least not during 30 minutes I gave this problem.
template <>
void runFix<6, 0>(int argc, char* argv[])
{
	std::vector<size_t> blocks, blocks_copy;

	std::istringstream iss(input_06);
	int number = 0;
	while (iss >> number)
	{
		blocks.push_back(number);
	}

	bool stop = false;
	size_t period = 0;
	size_t repeatInd = 0;
	std::vector<std::string> history;
	history.push_back(toString(blocks));

	while (!stop)
	{
		distribute(blocks, findMax(blocks));
		++period;
		auto const toStr = toString(blocks);

		for (size_t i = 0; i < history.size(); ++i)
		{
			if (toStr == history[i])
			{
				repeatInd = i;
				stop = true;
				break;
			}
		}
		if (!stop)
			history.push_back(toStr);
	}
	
	std::cout << period << " " << history.size() - repeatInd << std::endl;
}
