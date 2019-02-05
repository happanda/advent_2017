#include "advent.h"


void BugFix<12>::solve1st()
{
	std::string word;

	std::map<int, std::set<int>> pipes;

	while (std::getline(*mIn, word))
	{
		std::regex splitRegex("<\\->");
		std::smatch match;
		std::regex_search(word, match, splitRegex);

		int const prog = std::stoi(match.prefix());

		std::istringstream issSuffix(match.suffix());
		std::string token;

		while (std::getline(issSuffix, token, ','))
		{
			int const connectedProg = std::stoi(token);
			pipes[prog].insert(connectedProg);
			pipes[connectedProg].insert(prog);
		}
	}

	int count = 0;
	std::set<int> visited;
	std::queue<int> que;
	que.push(0);
	while (!que.empty())
	{
		for (int prog : pipes[que.front()])
		{
			if (visited.find(prog) == visited.end())
			{
				++count;
				que.push(prog);
				visited.insert(prog);
			}
		}
		que.pop();
	}

	*mOut << count << std::endl;
}

void BugFix<12>::solve2nd()
{
	std::string word;

	std::map<int, std::set<int>> pipes;

	while (std::getline(*mIn, word))
	{
		std::regex splitRegex("<\\->");
		std::smatch match;
		std::regex_search(word, match, splitRegex);

		int const prog = std::stoi(match.prefix());
		pipes[prog].insert(prog);

		std::istringstream issSuffix(match.suffix());
		std::string token;

		while (std::getline(issSuffix, token, ','))
		{
			int const connectedProg = std::stoi(token);
			pipes[prog].insert(connectedProg);
			pipes[connectedProg].insert(prog);
		}
	}

	std::set<int> allProgs;
	std::transform(pipes.begin(), pipes.end(), std::inserter(allProgs, allProgs.end()), [](auto const& elem) { return elem.first; });
	int countGroups = 0;

	while (!allProgs.empty())
	{
		++countGroups;
		int const anchorProg = *allProgs.begin();

		std::set<int> visited;
		std::queue<int> que;
		que.push(anchorProg);
		while (!que.empty())
		{
			for (int prog : pipes[que.front()])
			{
				if (visited.find(prog) == visited.end())
				{
					que.push(prog);
					visited.insert(prog);
				}
			}
			que.pop();
		}
		
		for (int prog : visited)
		{
			//*mOut << prog << " ";
			allProgs.erase(prog);
		}
		//*mOut << std::endl;
	}

	*mOut << countGroups << std::endl;
}
