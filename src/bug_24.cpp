#include "advent.h"


typedef uint16_t NodeID;
typedef std::map<NodeID, std::map<NodeID, bool>> Graph;


void recursivePaths(Graph& graph, NodeID node, uint32_t sum, uint32_t& maxSum, std::ostream& os)
{
	//os << node << " ";
	if (sum > maxSum)
		maxSum = sum;
	if (graph.find(node) != graph.end())
	{
		for (auto& child : graph.at(node))
		{
			if (!child.second)
			{
				child.second = true;
				graph[child.first][node] = true;
				recursivePaths(graph, child.first, sum + node + child.first, maxSum, os);
				//os << std::endl;
				graph[child.first][node] = false;
				child.second = false;
			}
		}
	}
}

void recursivePathsWithLength(Graph& graph, NodeID node, uint32_t sum, uint32_t& maxSum, uint16_t length, uint16_t& maxLen, std::ostream& os)
{
	//os << node << " ";
	if (length >= maxLen)
	{
		maxLen = length;
		if (sum > maxSum)
			maxSum = sum;
	}
	if (graph.find(node) != graph.end())
	{
		for (auto& child : graph.at(node))
		{
			if (!child.second)
			{
				child.second = true;
				graph[child.first][node] = true;
				recursivePathsWithLength(graph, child.first, sum + node + child.first, maxSum, length + 1, maxLen, os);
				//os << std::endl;
				graph[child.first][node] = false;
				child.second = false;
			}
		}
	}
}


void BugFix<24>::solve1st()
{
	Graph graph;

	std::string line;
	while (std::getline(*mIn, line))
	{
		auto delimPos = line.find('/');
		uint16_t const str1st = std::atoi(line.substr(0, delimPos).c_str());
		uint16_t const str2nd = std::atoi(line.substr(delimPos + 1).c_str());
		assert(graph[str1st].find(str2nd) == graph[str1st].end() && "Double edge");
		assert(graph[str2nd].find(str1st) == graph[str2nd].end() && "Double edge");
		graph[str1st][str2nd] = false;
		graph[str2nd][str1st] = false;
	}

	uint32_t maxSum = 0;
	recursivePaths(graph, 0, 0, maxSum, *mOut);

	*mOut << maxSum << std::endl;
}

void BugFix<24>::solve2nd()
{
	Graph graph;

	std::string line;
	while (std::getline(*mIn, line))
	{
		auto delimPos = line.find('/');
		uint16_t const str1st = std::atoi(line.substr(0, delimPos).c_str());
		uint16_t const str2nd = std::atoi(line.substr(delimPos + 1).c_str());
		assert(graph[str1st].find(str2nd) == graph[str1st].end() && "Double edge");
		assert(graph[str2nd].find(str1st) == graph[str2nd].end() && "Double edge");
		graph[str1st][str2nd] = false;
		graph[str2nd][str1st] = false;
	}

	uint32_t maxSum = 0;
	uint16_t maxLen = 0;
	recursivePathsWithLength(graph, 0, 0, maxSum, 0, maxLen, *mOut);

	*mOut << maxSum << std::endl;
}
