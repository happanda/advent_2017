#include "advent.h"


void BugFix<7>::solve1st()
{
	std::unordered_map<std::string, std::string> programTree;

	std::string line;
	while (std::getline(*mIn, line))
	{
		std::regex wordsRegex("[a-z]+");
		auto words_begin = std::sregex_iterator(line.begin(), line.end(), wordsRegex);
		auto words_end = std::sregex_iterator();

		int i = 0;
		std::string base;
		for (auto w = words_begin; w != words_end; ++w)
		{
			std::smatch match = *w;
			if (i == 0)
			{
				base = match.str();
				if (programTree.find(base) == programTree.end())
					programTree[base] = "";
			}
			else
			{
				std::string progName = match.str();
				programTree[progName] = base;
			}
			++i;
		}
	}

	std::string prog = programTree.begin()->first;
	while (!programTree[prog].empty())
	{
		prog = programTree[prog];
	}

	*mOut << prog << std::endl;
}

struct ProgramWeight
{
	size_t ownWeight{ 0 };
	size_t totalWeight{ 0 };
};

typedef std::unordered_multimap<std::string, std::string> MapType;
typedef std::unordered_map<std::string, ProgramWeight> WeightType;

void fillWeights(std::string const& progName, MapType const& tree, WeightType& weights, std::string& faultyProgBase)
{
	size_t weight = weights.at(progName).ownWeight;
	auto const& range = tree.equal_range(progName);
	size_t weightAbove = 0;

	for (auto it = range.first; it != range.second; ++it)
	{
		auto const& progAbove = it->second;
		fillWeights(progAbove, tree, weights, faultyProgBase);
		if (weightAbove > 0 && weightAbove != weights[progAbove].totalWeight)
		{
			if (faultyProgBase.empty())
				faultyProgBase = progName;
		}
		else
			weightAbove = weights[progAbove].totalWeight;
		weight += weights[progAbove].totalWeight;
	}
	weights.at(progName).totalWeight = weight;
}

void BugFix<7>::solve2nd()
{
	MapType programTree;
	WeightType programWeights;

	std::string line;
	while (std::getline(*mIn, line))
	{
		std::regex weightRegex("\\d+");
		std::smatch weightMatch;
		std::regex_search(line, weightMatch, weightRegex);
		size_t const weight = std::stoul(weightMatch.str());

		std::regex wordsRegex("[a-z]+");
		auto words_begin = std::sregex_iterator(line.begin(), line.end(), wordsRegex);
		auto words_end = std::sregex_iterator();

		int i = 0;
		std::string base;
		for (auto w = words_begin; w != words_end; ++w)
		{
			std::smatch match = *w;

			if (i == 0)
			{
				base = match.str();
				programWeights[base].ownWeight = weight;
				programWeights[base].totalWeight = 0;
			}
			else
			{
				programTree.insert(MapType::value_type(base, match.str()));
			}
			++i;
		}
	}

	// find root
	std::string root;
	for (auto const& prog : programWeights)
	{
		bool found = false;
		for (auto const& pp : programTree)
		{
			if (pp.second == prog.first)
			{
				found = true;
				break;
			}
		}
		if (found == false)
		{
			root = prog.first;
			break;
		}
	}

	std::string faultyProgBase;
	fillWeights(root, programTree, programWeights, faultyProgBase);

	//print out all from root
	/*for (auto const& prog : programWeights)
	{
		std::string const& progName = prog.first;
		*mOut << progName << " (" << prog.second.totalWeight << ") -> ";

		size_t curWeight = 0;
		auto const& range = programTree.equal_range(progName);
		for (auto it = range.first; it != range.second; ++it)
		{
			std::string const& progAbove = it->second;
			if (curWeight > 0 && curWeight != programWeights[progAbove].totalWeight)
				*mOut << "### ";
			curWeight = programWeights[progAbove].totalWeight;
			*mOut << progAbove << " (" << curWeight << "), ";
		}
		*mOut << std::endl;
	}*/
	
	int weightDiff = 0;
	auto const& range = programTree.equal_range(faultyProgBase);
	for (auto it = range.first; it != range.second; ++it)
	{
		auto const& progOne = it->second;
		bool okay = false;

		for (auto jt = range.first; !okay && jt != range.second; ++jt)
		{
			if (jt == it)
				continue;

			auto const& progTwo = jt->second;
			if (programWeights[progOne].totalWeight == programWeights[progTwo].totalWeight)
			{
				okay = true;
			}
			else
				weightDiff = (int)programWeights[progTwo].totalWeight - programWeights[progOne].totalWeight;
		}

		if (!okay)
		{
			*mOut << programWeights[progOne].ownWeight + weightDiff << std::endl;
			break;
		}
	}
}
