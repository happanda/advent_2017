#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "advent.h"


void BugFix<13>::solve1st()
{
	std::string line;
	std::vector<int> ranges;

	while (std::getline(*mIn, line))
	{
		std::regex splitRegex(":");
		std::smatch match;
		std::regex_search(line, match, splitRegex);

		size_t const depth = std::stoi(match.prefix());
		int const range = std::stoi(match.suffix());

		if (depth + 1 > ranges.size())
			ranges.resize(depth + 1);
		ranges[depth] = range;
	}

	int severity = 0;
	for (size_t s = 0; s < ranges.size(); ++s) //s = step
	{
		if (ranges[s] > 0)
		{
			if (ranges[s] == 1
				|| s % (ranges[s] * 2 - 2) == 0)
			{
				severity += ranges[s] * s;
			}
		}
	}

	*mOut << severity << std::endl;
}

void BugFix<13>::solve2nd()
{
	std::string line;
	std::vector<int> ranges;

	while (std::getline(*mIn, line))
	{
		std::regex splitRegex(":");
		std::smatch match;
		std::regex_search(line, match, splitRegex);

		size_t const depth = std::stoi(match.prefix());
		int const range = std::stoi(match.suffix());

		if (depth + 1 > ranges.size())
			ranges.resize(depth + 1);
		ranges[depth] = range;
	}

	int delay = 0;
	bool finished = false;

	while (!finished)
	{
		finished = true;

		for (size_t s = 0; s < ranges.size(); ++s) //s = step
		{
			if (ranges[s] > 0)
			{
				if (ranges[s] == 1
					|| (s + delay) % (ranges[s] * 2 - 2) == 0)
				{
					finished = false;
					++delay;
				}
			}
		}
	}

	*mOut << delay << std::endl;
}
