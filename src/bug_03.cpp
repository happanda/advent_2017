#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include "advent.h"


int input_03 = 368078;

std::pair<int, int> runFix2_toCoordinates(int N)
{
	const int maxTier = static_cast<int>((-1.0 + std::sqrt(N)) / 2.0);
	const int tailLen = N - 1 - 4 * (1 + maxTier) * maxTier;

	std::pair<int, int> coords(maxTier, - maxTier);
	coords.first = coords.first + (tailLen > 0 ? 1 : 0);
	coords.second = coords.second + (tailLen > 0 ? tailLen % (2 * (maxTier + 1)) - 1 : 0);

	const int quadrant = tailLen / (2 * (maxTier + 1));
	switch (quadrant)
	{
	case 1:
		std::swap(coords.first, coords.second);
		coords.first = -coords.first;
		break;
	case 2:
		coords.first = -coords.first;
		coords.second = -coords.second;
		break;
	case 3:
		std::swap(coords.first, coords.second);
		coords.second = -coords.second;
		break;
	}
	return coords;
}

template <>
void runFix<2, 0>(int argc, char* argv[])
{
	//for (input_03 = 2; input_03 <= 25; ++input_03)
	{
		int manhattanDist = 0;
		if (input_03 == 1)
		{
			std::cout << "0" << std::endl;
			return;
		}

		std::pair<int, int> coords = runFix2_toCoordinates(input_03);

		//std::cout << input_03 << " : " << coords.first << ", " << coords.second << std::endl;
		std::cout << input_03 << " : " << std::abs(coords.first) + std::abs(coords.second) << std::endl;
	}
}

int runFix2_toSpiralNumber(const std::pair<int, int>& coords)
{
	int maxTier = std::max(std::abs(coords.first), std::abs(coords.second)) - 1;
	
	int N = 4 * maxTier * (maxTier + 1);
	if (coords.second > -coords.first && coords.second <= coords.first)
		N += coords.second + maxTier + 1;
	else if (coords.first >= -coords.second && coords.first < coords.second)
		N += 2 * (maxTier + 1) + maxTier + 1 - coords.first;
	else if (coords.second < -coords.first && coords.second >= coords.first)
		N += 2 * 2 * (maxTier + 1) + maxTier + 1 - coords.second;
	else if (coords.first > coords.second && coords.first <= -coords.second)
		N += 3 * 2 * (maxTier + 1) + coords.first + maxTier + 1;
	return N + 1;
}

template <>
void runFix<2, 1>(int argc, char* argv[])
{
	const std::pair<int, int> shifts[] = { { 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, -1 },{ 1, -1 } };

	std::vector<int> spiral{ 1, 1 };

	while (*spiral.rbegin() < input_03)
	{
		int number = spiral.size() + 1;
		const std::pair<int, int> coords = runFix2_toCoordinates(number);
		int sum = 0;
		
		for (int c = 0; c < 8; ++c)
		{
			std::pair<int, int> shifted = coords;
			shifted.first += shifts[c].first;
			shifted.second += shifts[c].second;
			int neighborNum = runFix2_toSpiralNumber(shifted);
			if (neighborNum < number)
				sum += spiral[neighborNum - 1];
		}
		spiral.push_back(sum);
	}

	std::cout << *spiral.rbegin() << std::endl;
	/*for (int x = -2; x <= 2; ++x)
	{
		for (int y = -2; y <= 2; ++y)
		{
			std::cout << x << ", " << y << " : " << runFix2_toSpiralNumber(std::make_pair(x, y)) << std::endl;
		}
	}*/
}
