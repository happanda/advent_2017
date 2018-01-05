#include "advent.h"


void BugFix<4>::solve1st()
{
	int numGoodPhrases = 0;

	std::string line;

	while (std::getline(*mIn, line))
	{
		std::istringstream wordsSStr(line);
		std::string word;
		std::unordered_set<std::string> distinct;
		bool hasRepeats = false;

		while (!hasRepeats && wordsSStr >> word)
		{
			if (distinct.find(word) != distinct.end())
				hasRepeats = true;
			else
				distinct.insert(word);
		}

		if (!hasRepeats)
			++numGoodPhrases;
	}

	*mOut << numGoodPhrases << std::endl;
}

void BugFix<4>::solve2nd()
{
	int numGoodPhrases = 0;

	std::string line;

	while (std::getline(*mIn, line))
	{
		std::istringstream wordsSStr(line);
		std::string word;
		std::unordered_set<std::string> distinct;
		bool hasRepeats = false;

		while (!hasRepeats && wordsSStr >> word)
		{
			std::sort(word.begin(), word.end());

			if (distinct.find(word) != distinct.end())
				hasRepeats = true;
			else
				distinct.insert(word);
		}

		if (!hasRepeats)
			++numGoodPhrases;
	}

	*mOut << numGoodPhrases << std::endl;
}
