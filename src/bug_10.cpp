#include "advent.h"


inline void reverse(std::vector<int>& vect, int from, int length)
{
	int const vsize = vect.size();
	if (length > vsize)
		return;
	for (int i = 0; i < length / 2; ++i)
		std::swap(vect[(from + i) % vsize], vect[(from + length - i - 1) % vsize]);
}

inline std::string knotHash(std::string const& input)
{
	int const numRounds = 64;
	int const seqLength = 256;
	int const xorLength = 16;

	std::vector<int> sequence;
	std::generate_n(std::back_inserter(sequence), seqLength, [&sequence]() {return sequence.size(); });
	
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

	std::string xored(2 * seqLength / xorLength, 0);
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
	return xored;
}

void BugFix<10>::solve1st()
{
	int const seqLength = 256;
	std::vector<int> sequence;
	std::generate_n(std::back_inserter(sequence), seqLength, [&sequence]() {return sequence.size(); });

	std::string word;
	int curInd = 0;
	int skipSize = 0;

	while (std::getline(*mIn, word, ','))
	{
		int const length = std::stoi(word);
		if (length > static_cast<int>(sequence.size()))
			continue;
		reverse(sequence, curInd, length);

		curInd += (length + skipSize) % seqLength;
		++skipSize;
	}

	*mOut << sequence[0] * sequence[1] << std::endl;
}

void BugFix<10>::solve2nd()
{
	std::string input;
	*mIn >> input;

	*mOut << knotHash(input) << std::endl;
}
