#include "advent.h"

struct GridPos
{
	int32_t x;
	int32_t y;
};

bool operator<(GridPos const& lhs, GridPos const& rhs)
{
	return lhs.x < rhs.x ? true :
		(lhs.x == rhs.x ? lhs.y < rhs.y : false);
}

bool operator==(GridPos const& lhs, GridPos const& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

GridPos const& operator+=(GridPos& lhs, GridPos const& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

namespace std
{
	template <>
	struct hash<GridPos>
	{
		size_t operator()(GridPos const& pos) const noexcept
		{
			return std::_Hash_bytes(static_cast<const unsigned char*>(static_cast<const void*>(&pos)), sizeof(pos));
		}
	};
}

void rotateLeft(GridPos& dir)
{
	dir = { -dir.y, dir.x };
}

void rotateRight(GridPos& dir)
{
	dir = { dir.y, -dir.x };
}

void opposite(GridPos& dir)
{
	dir = { -dir.x, -dir.y };
}

void print(std::ostream& is, std::unordered_map<GridPos, bool> const& grid, GridPos const& virus)
{
	GridPos minPos = { std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max() },
		maxPos = { std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min() };
	for (auto const& c : grid)
	{
		if (c.first.x < minPos.x)
			minPos.x = c.first.x;
		if (c.first.x > maxPos.x)
			maxPos.x = c.first.x;

		if (c.first.y < minPos.y)
			minPos.y = c.first.y;
		if (c.first.y > maxPos.y)
			maxPos.y = c.first.y;
	}
	minPos = { -3, -3 };
	maxPos = { 6, 6 };
	for (int32_t y = minPos.y; y < maxPos.y; ++y)
	{
		for (int32_t x = minPos.x; x < maxPos.x; ++x)
		{
			GridPos pos = { x, y };
			if (virus == pos)
				is << "X";
			else if (grid.find(pos) != grid.end())
				is << "#";
			else
				is << ".";
		}
		is << std::endl;
	}
}


void BugFix<22>::solve1st()
{
	std::unordered_map<GridPos, bool> grid;
	std::string line;
	GridPos pos = { 0, 0 };
	while (std::getline(*mIn, line))
	{
		pos.x = 0;
		for (auto const& ch : line)
		{
			if (ch == '#')
				grid[pos] = true;
			++pos.x;
		}
		++pos.y;
	}

	GridPos const startPos = { pos.x / 2, pos.y / 2 };
	GridPos virus = startPos;
	GridPos dir = { 0, -1 };

	uint32_t countInfected = { 0 };
	uint32_t constexpr NumSteps = 10000;
	for (uint32_t step = 0; step < NumSteps; ++step)
	{
		//print(*mOut, grid, virus);
		if (grid.find(virus) != grid.end())
		{
			grid.erase(virus);
			rotateLeft(dir);
			//*mOut << "x " << virus.x << " " << virus.y << std::endl;
		}
		else
		{
			grid[virus] = true;
			++countInfected;
			rotateRight(dir);
			//*mOut << ". " << virus.x << " " << virus.y << std::endl;
		}
		virus += dir;
		//*mOut << std::endl;
	}
	//print(*mOut, grid, virus);
	*mOut << countInfected << std::endl;
}

void BugFix<22>::solve2nd()
{
	enum class NodeState : uint8_t
	{
		Weakened,
		Infected,
		Flagged
	};
	std::unordered_map<GridPos, NodeState> grid;

	auto print = [](std::ostream& is, std::unordered_map<GridPos, NodeState> const& grid, GridPos const& virus)
	{
		GridPos minPos = { std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max() },
			maxPos = { std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min() };
		for (auto const& c : grid)
		{
			if (c.first.x < minPos.x)
				minPos.x = c.first.x;
			if (c.first.x > maxPos.x)
				maxPos.x = c.first.x;

			if (c.first.y < minPos.y)
				minPos.y = c.first.y;
			if (c.first.y > maxPos.y)
				maxPos.y = c.first.y;
		}
		minPos = { -3, -3 };
		maxPos = { 6, 6 };
		for (int32_t y = minPos.y; y < maxPos.y; ++y)
		{
			for (int32_t x = minPos.x; x < maxPos.x; ++x)
			{
				GridPos pos = { x, y };
				if (virus == pos)
					is << "X";
				else if (grid.find(pos) == grid.end())
					is << ".";
				else if (grid.at(pos) == NodeState::Flagged)
					is << "F";
				else if (grid.at(pos) == NodeState::Weakened)
					is << "W";
				else if (grid.at(pos) == NodeState::Infected)
					is << "#";
			}
			is << std::endl;
		}
	};
	std::string line;
	GridPos pos = { 0, 0 };
	while (std::getline(*mIn, line))
	{
		pos.x = 0;
		for (auto const& ch : line)
		{
			if (ch == '#')
				grid[pos] = NodeState::Infected;
			++pos.x;
		}
		++pos.y;
	}

	GridPos const startPos = { pos.x / 2, pos.y / 2 };
	GridPos virus = startPos;
	GridPos dir = { 0, -1 };

	uint32_t countInfected = { 0 };
	uint32_t constexpr NumSteps = 10000000;
	for (uint32_t step = 0; step < NumSteps; ++step)
	{
		//print(*mOut, grid, virus);
		auto const& it = grid.find(virus);
		if (it == grid.end())
		{
			grid[virus] = NodeState::Weakened;
			rotateRight(dir);
			//*mOut << "x " << virus.x << " " << virus.y << std::endl;
		}
		else
		{
			if (it->second == NodeState::Weakened)
			{
				grid[virus] = NodeState::Infected;
				++countInfected;
			}
			else if (it->second == NodeState::Infected)
			{
				grid[virus] = NodeState::Flagged;
				rotateLeft(dir);
			}
			else if (it->second == NodeState::Flagged)
			{
				grid.erase(virus);
				opposite(dir);
			}
			//*mOut << ". " << virus.x << " " << virus.y << std::endl;
		}
		virus += dir;
		//*mOut << std::endl;
	}
	//print(*mOut, grid, virus);
	*mOut << countInfected << std::endl;
}
