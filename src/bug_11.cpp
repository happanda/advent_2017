#include "advent.h"


enum class Dir
{
	NE, N, NW, SW, S, SE, count
};

Dir convert(std::string const& str)
{
	static struct { std::string str; Dir dir; } const map[] =
	{
		{ "ne", Dir::NE },
		{ "n", Dir::N },
		{ "nw", Dir::NW },
		{ "sw", Dir::SW },
		{ "s", Dir::S },
		{ "se", Dir::SE }
	};
	for (int i = 0; i < sizeof(map) / sizeof(map[0]); ++i)
		if (str == map[i].str)
			return map[i].dir;
	return Dir::count;
}

struct Vect { int x; int y; int z; };

Vect operator+(Vect const& lhs, Vect const& rhs)
{
	return Vect { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

int distance(Vect const& pos)
{
	return std::max(std::max(std::abs(pos.x), std::abs(pos.y)), std::abs(pos.z));
}

void BugFix<11>::solve1st()
{
	std::string word;

	Vect pos{ 0, 0, 0 };
	Vect const moves[] =
	{
		{1, 0, -1},
		{0, 1, -1},
		{-1, 1, 0},
		{-1, 0, 1},
		{0, -1, 1},
		{1, -1, 0}
	};

	int maxDist = 0;
	while (std::getline(*mIn, word, ','))
	{
		Dir dir = convert(word);
		pos = pos + moves[static_cast<int>(dir)];
		maxDist = std::max(maxDist, distance(pos));
	}

	*mOut << distance(pos) << " " << maxDist << std::endl;
}

void BugFix<11>::solve2nd()
{
	solve1st();
}