#include "advent.h"


void BugFix<2>::solve1st()
{
	int totalSum = 0;

	std::string line;
	while (std::getline(*mIn, line, '\n'))
	{
		std::istringstream numTokens(line);
		int number = 0,
			min = std::numeric_limits<int>::max(),
			max = 0;
		while (numTokens >> number)
		{
			max = std::max(max, number);
			min = std::min(min, number);
		}
		totalSum += max - min;
	}

	*mOut << totalSum << std::endl;
}

void BugFix<2>::solve2nd()
{
	int totalSum = 0;

	std::string line;

	while (std::getline(*mIn, line, '\n'))
	{
		std::istringstream numTokens(line);

		int number = 0;
		std::vector<int> numbers;

		while (numTokens >> number)
			numbers.push_back(number);

		[&] { // using anonymous lambda for break from inner loop
			for (size_t i = 0; i < numbers.size() - 1; ++i)
			{
				for (size_t j = i + 1; j < numbers.size(); ++j)
				{
					const int min = std::min(numbers[i], numbers[j]);
					const int max = std::max(numbers[i], numbers[j]);

					if (max % min == 0)
					{
						totalSum += max / min;
						return; // break out of both loops
					}
				}
			}
		}();
	}

	*mOut << totalSum << std::endl;
}

