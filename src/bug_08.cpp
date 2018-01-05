#include "advent.h"


void BugFix<8>::solve1st()
{
	std::unordered_map<std::string, bool(*)(int lhs, int rhs)> operators;
	operators.insert({ "==", [](int lhs, int rhs) { return lhs == rhs; } });
	operators.insert({ "!=", [](int lhs, int rhs) { return lhs != rhs; } });
	operators.insert({ "<=", [](int lhs, int rhs) { return lhs <= rhs; } });
	operators.insert({ ">=", [](int lhs, int rhs) { return lhs >= rhs; } });
	operators.insert({ "<", [](int lhs, int rhs) { return lhs < rhs; } });
	operators.insert({ ">", [](int lhs, int rhs) { return lhs > rhs; } });

	std::unordered_map<std::string, void(*)(int& reg, int val)> instructions;
	instructions.insert({ "inc", [](int& reg, int val) { reg += val; } });
	instructions.insert({ "dec", [](int& reg, int val) { reg -= val; } });

	std::unordered_map<std::string, int> values;

	std::string line;
	while (std::getline(*mIn, line))
	{
		std::regex wordsRegex("[^\\s]+");
		auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), wordsRegex);
		auto wordsEnd = std::sregex_iterator();

		std::vector<std::string> words;
		std::transform(wordsBegin, wordsEnd, std::back_inserter(words),
			[](auto const& it) { return it.str(); });

		std::string const& checkReg = words[4];
		std::string const& operation = words[5];
		int const checkVal = std::stoi(words[6]);

		if (operators[operation](values[checkReg], checkVal))
		{
			std::string const& changeReg = words[0];
			std::string const& instruction = words[1];
			int const changeVal = std::stoi(words[2]);
			instructions[instruction](values[changeReg], changeVal);
		}
	}

	auto maximum = std::max_element(values.begin(), values.end(), [](auto const& lhs, auto const& rhs)
	{
		return lhs.second < rhs.second;
	});
	*mOut << maximum->second << std::endl;
}

void BugFix<8>::solve2nd()
{
	std::unordered_map<std::string, bool(*)(int lhs, int rhs)> operators;
	operators.insert({ "==", [](int lhs, int rhs) { return lhs == rhs; } });
	operators.insert({ "!=", [](int lhs, int rhs) { return lhs != rhs; } });
	operators.insert({ "<=", [](int lhs, int rhs) { return lhs <= rhs; } });
	operators.insert({ ">=", [](int lhs, int rhs) { return lhs >= rhs; } });
	operators.insert({ "<", [](int lhs, int rhs) { return lhs < rhs; } });
	operators.insert({ ">", [](int lhs, int rhs) { return lhs > rhs; } });

	std::unordered_map<std::string, void(*)(int& reg, int val)> instructions;
	instructions.insert({ "inc", [](int& reg, int val) { reg += val; } });
	instructions.insert({ "dec", [](int& reg, int val) { reg -= val; } });

	std::unordered_map<std::string, int> values;

	int totalMax = std::numeric_limits<int>::min();
	std::string line;
	while (std::getline(*mIn, line))
	{
		std::regex wordsRegex("[^\\s]+");
		auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), wordsRegex);
		auto wordsEnd = std::sregex_iterator();

		std::vector<std::string> words;
		std::transform(wordsBegin, wordsEnd, std::back_inserter(words),
			[](auto const& it) { return it.str(); });

		std::string const& checkReg = words[4];
		std::string const& operation = words[5];
		int const checkVal = std::stoi(words[6]);

		if (operators[operation](values[checkReg], checkVal))
		{
			std::string const& changeReg = words[0];
			std::string const& instruction = words[1];
			int const changeVal = std::stoi(words[2]);
			instructions[instruction](values[changeReg], changeVal);
			if (values[changeReg] > totalMax)
				totalMax = values[changeReg];
		}
	}

	*mOut << totalMax << std::endl;
}
