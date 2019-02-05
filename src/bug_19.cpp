#include "advent.h"

using std::uint32_t;

enum class Direction
{
	Down,
	Right,
	Up,
	Left
};

struct Vec
{
	int32_t x{ 0 };
	int32_t y{ 0 };
};

namespace std
{
	template <>
	struct hash<Vec>
	{
		size_t operator()(const Vec& vec) const noexcept
		{
			return _Hash_bytes(static_cast<const unsigned char*>(static_cast<const void*>(&vec)), sizeof(vec));
		}
	};
}

bool operator==(const Vec& lhs, const Vec& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

typedef std::unordered_map<Vec, char> Field;
Field sField;

Vec findDirection(const Field& field, const Vec& pos, const Vec& shift)
{
	if (shift.x == 0)
	{
		if (field.find(Vec{ pos.x - 1, pos.y }) != field.end())
			return Vec{ -1, 0 };
		else if (field.find(Vec{ pos.x + 1, pos.y }) != field.end())
			return Vec{ 1, 0 };
	}
	else
	{
		if (field.find(Vec{ pos.x, pos.y - 1 }) != field.end())
			return Vec{ 0, -1 };
		else if (field.find(Vec{ pos.x, pos.y + 1 }) != field.end())
			return Vec{ 0, 1 };
	}
	return Vec{ 0, 0 };
}

bool move(const Field& field, Vec& pos, Vec& shift)
{
	if (field.at(pos) == '+')
	{
		shift = findDirection(field, pos, shift);
	}
	if (shift.x != 0 || shift.y != 0)
	{
		pos.x += shift.x;
		pos.y += shift.y;
		return true;
	}
	return false;
}


void BugFix<19>::solve1st()
{
	Vec position{ -1, 0 };
	Vec direction{ 0, 1 };
	
	int32_t y = 0;
	std::string line;
	while (std::getline(*mIn, line))
	{
		for (int32_t x = 0; x < static_cast<int32_t>(line.size()); ++x)
		{
			if (line[x] != ' ')
				sField[Vec{ x, y }] = line[x];
			if (y == 0 && line[x] == '|')
				position.x = x;
		}
		++y;
	}

	std::string letters;;
	while (move(sField, position, direction) && sField.find(position) != sField.end())
	{
		if (sField.at(position) >= 'A' && sField.at(position) <= 'Z')
			letters.append(1, sField.at(position));
	}
	*mOut << letters << std::endl;
}

void BugFix<19>::solve2nd()
{
	Vec position{ -1, 0 };
	Vec direction{ 0, 1 };

	int32_t y = 0;
	std::string line;
	while (std::getline(*mIn, line))
	{
		for (int32_t x = 0; x < static_cast<int32_t>(line.size()); ++x)
		{
			if (line[x] != ' ')
				sField[Vec{ x, y }] = line[x];
			if (y == 0 && line[x] == '|')
				position.x = x;
		}
		++y;
	}

	std::string letters;
	int32_t numSteps{ 1 };
	while (move(sField, position, direction) && sField.find(position) != sField.end())
	{
		++numSteps;
		if (sField.at(position) >= 'A' && sField.at(position) <= 'Z')
			letters.append(1, sField.at(position));
	}
	*mOut << numSteps << std::endl;
}
