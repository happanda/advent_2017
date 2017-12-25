#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include "advent.h"


std::string const input_08 = std::string(R"(206,63,255,131,65,80,238,157,254,24,133,2,16,0,1,3)");

void reverse(std::vector<int>& vect, int from, int length)
{
	int const vsize = vect.size();
	if (length > vsize)
		return;
	for (int i = 0; i < length / 2; ++i)
		std::swap(vect[(from + i) % vsize], vect[(from + length - i - 1) % vsize]);
}

template <>
void runFix<10, 0>(int argc, char* argv[])
{
	int const seqLength = 256;
	std::vector<int> sequence;
	std::generate_n(std::back_inserter(sequence), seqLength, [&sequence]() {return sequence.size(); });

	std::istringstream iss(input_08);
	std::string word;
	int curInd = 0;
	int skipSize = 0;

	while (std::getline(iss, word, ','))
	{
		int const length = std::stoi(word);
		if (length > static_cast<int>(sequence.size()))
			continue;
		reverse(sequence, curInd, length);

		curInd += (length + skipSize) % seqLength;
		++skipSize;
	}

	std::cout << sequence[0] * sequence[1] << std::endl;
}

template <>
void runFix<10, 1>(int argc, char* argv[])
{
	int const numRounds = 64;
	int const seqLength = 256;
	int const xorLength = 16;

	std::vector<int> sequence;
	std::generate_n(std::back_inserter(sequence), seqLength, [&sequence]() {return sequence.size(); });

	std::string input(input_08);
	std::vector<int> lengths;
	std::transform(input.begin(), input.end(), std::back_inserter(lengths), [](char symbol) { return static_cast<int>(symbol); });
	for (int n : { 17, 31, 73, 47, 23 })
		lengths.push_back(n);

	int curInd = 0;
	int skipSize = 0;
	
	for (int r = 0; r < numRounds; ++r)
	{
		for (int length : lengths)
		{
			if (length > static_cast<int>(sequence.size()))
				continue;
			reverse(sequence, curInd, length);

			curInd += (length + skipSize) % seqLength;
			skipSize = (skipSize + 1) % seqLength;
		}
	}

	std::string xored(2 * seqLength / xorLength + 1, '\0');
	for (int i = 0; i < seqLength / xorLength; ++i)
	{
		int xorResult = 0;
		for (int j = 0; j < xorLength; ++j)
		{
			xorResult ^= sequence[i * xorLength + j];
		}

		char hex1 = (xorResult / 16) % 16;
		char hex0 = xorResult % 16;
		if (hex1 < 10)
			xored[i * 2] = hex1 + '0';
		else
			xored[i * 2] = hex1 - 10 + 'a';
		if (hex0 < 10)
			xored[i * 2 + 1] = hex0 + '0';
		else
			xored[i * 2 + 1] = hex0 - 10 + 'a';
	}

	std::cout << xored << std::endl;
}