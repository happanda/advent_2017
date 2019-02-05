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

void BugFix<14>::solve1st()
{
	int const length = 128;

	std::string line;
	*mIn >> line;

	int numWritten = 0;
	for (int i = 0; i < length; ++i)
	{
		std::string const str = line + "-" + std::to_string(i);
		std::string const hash = knotHash(str);

		//*mOut << str << '\t' << hash << std::endl;		

		for (char ch : hash)
		{
			char binary = 0;
			if (ch >= '0' && ch <= '9')
				binary = ch - '0';
			else
				binary = ch - 'a' + 10;

			int bit = 1;
			while (bit < (1 << 4))
			{
				if (bit & binary)
					++numWritten;
				bit = bit << 1;
			}
		}
	}

	*mOut << numWritten << std::endl;
}

void traverseRegion(std::vector<std::vector<bool>>& blocks, int i, int j)
{
	if (!blocks[i][j])
		return;

	static std::pair<int, int> constexpr shift[] = { { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 } };
	static int constexpr shiftLen = sizeof(shift) / sizeof(shift[0]);

	blocks[i][j] = false;
	for (int k = 0; k < shiftLen; ++k)
	{
		int const xs = i + shift[k].first;
		int const ys = j + shift[k].second;
		if (xs >= 0 && xs < blocks.size() && ys >= 0 && ys < blocks.size())
		{
			if (blocks[xs][ys])
				traverseRegion(blocks, xs, ys);
		}
	}
}

void BugFix<14>::solve2nd()
{
	int const length = 128;

	std::string line;
	*mIn >> line;

	std::vector<std::vector<bool>> blocks(length);

	int numWritten = 0;
	for (int i = 0; i < length; ++i)
	{
		std::string const str = line + "-" + std::to_string(i);
		std::string const hash = knotHash(str);

		//*mOut << str << '\t' << hash << std::endl;
		blocks[i].resize(length);

		int column = 0;
		for (char ch : hash)
		{
			char binary = 0;
			if (ch >= '0' && ch <= '9')
				binary = ch - '0';
			else
				binary = ch - 'a' + 10;

			int bit = 1;
			while (bit < (1 << 4))
			{
				if (bit & binary)
					blocks[i][column] = true;
				bit = bit << 1;
				++column;
			}
		}
	}

	for (int i = 0; i < length; ++i)
	{
		for (int j = 0; j < length; ++j)
		{
			if (blocks[i][j])
				*mOut << '#';
			else
				*mOut << '.';
		}
		*mOut << std::endl;
	}

	int numRegions = 0;
	for (int i = 0; i < length; ++i)
	{
		for (int j = 0; j < length; ++j)
		{
			if (blocks[i][j])
			{
				++numRegions;
				traverseRegion(blocks, i, j);
			}
		}
	}

	*mOut << numRegions << std::endl;
}
