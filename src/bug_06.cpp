#include "advent.h"


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

bool equals(std::vector<size_t> const& lhs_blocks, std::vector<size_t> const& rhs_blocks)
{
	if (lhs_blocks.size() != rhs_blocks.size())
		return false;
	for (size_t i = 0; i < lhs_blocks.size(); ++i)
	{
		if (lhs_blocks[i] != rhs_blocks[i])
			return false;
	}
	return true;
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
void BugFix<6>::solve1st()
{
	std::vector<size_t> blocks, blocks_copy;

	int number = 0;
	while (*mIn >> number)
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
	
	*mOut << period << " " << history.size() - repeatInd << std::endl;
}

void BugFix<6>::solve2nd()
{
	std::vector<size_t> blocks, blocks_copy;

	int number = 0;
	while (*mIn >> number)
	{
		blocks.push_back(number);
	}
	blocks_copy = blocks;
	distribute(blocks_copy, findMax(blocks_copy));

	bool stop = false;

	while (!equals(blocks, blocks_copy))
	{
		distribute(blocks, findMax(blocks));
		distribute(blocks_copy, findMax(blocks_copy));
		distribute(blocks_copy, findMax(blocks_copy));
	}

	size_t period = 1;
	distribute(blocks_copy, findMax(blocks_copy));

	while (!equals(blocks, blocks_copy))
	{
		distribute(blocks_copy, findMax(blocks_copy));
		++period;
	}

	*mOut << period << std::endl;
}
