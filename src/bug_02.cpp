#include <iostream>
#include "advent.h"


template <>
void runFix<1, 0>(int argc, char* argv[])
{
	const char* input = "5 1 9 5\
		7 5 3\
		2 4 6 8";
	const int length = std::strlen(input);
	int repeatsSum = 0;
	for (int i = 0; i <= length; ++i)
	{
		if (input[i] == input[(i + 1) % length])
			repeatsSum += input[i] - '0';
	}
	std::cout << repeatsSum << std::endl;
}

template <>
void runFix<1, 1>(int argc, char* argv[])
{
}

