#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include "advent.h"


std::string const input_08 = std::string(R"(206, 63, 255, 131, 65, 80, 238, 157, 254, 24, 133, 2, 16, 0, 1, 3)");

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
