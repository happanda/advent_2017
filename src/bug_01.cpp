#include "advent.h"


void BugFix<1>::solve1st()
{
	std::string input;
	std::getline(*mIn, input);

	int repeatsSum = 0;
	for (size_t i = 0; i <= input.size(); ++i)
	{
		if (input[i] == input[(i + 1) % input.size()])
			repeatsSum += input[i] - '0';
	}
	*mOut << repeatsSum << std::endl;
}

void BugFix<1>::solve2nd()
{
	std::string input;
	std::getline(*mIn, input);

	int repeatsSum = 0;
	for (size_t i = 0; i <= input.size(); ++i)
	{
		if (input[i] == input[(i + input.size() / 2) % input.size()])
			repeatsSum += input[i] - '0';
	}
	*mOut << repeatsSum << std::endl;
}
