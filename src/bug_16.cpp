#include "advent.h"


struct ProgramDance
{
	static const int mSize{ 16 };

	ProgramDance()
	{
		mLetters.resize(mSize);
		char c = 'a';
		for (int i = 0; i < mSize; ++c, ++i)
		{
			mLetters[i] = c;
			mPositions[c - 'a'] = i;
		}
	}

	void spin(int idx)
	{
		mFirst = (mFirst + mSize - idx) % mSize;
	}

	void exchange(int l, int r)
	{
		l = (l + mFirst) % mSize;
		r = (r + mFirst) % mSize;

		std::swap(mPositions[mLetters[l] - 'a'], mPositions[mLetters[r] - 'a']);
		std::swap(mLetters[l], mLetters[r]);
	}

	void partner(char l, char r)
	{
		exchange((mPositions[l - 'a'] + mSize - mFirst) % mSize, (mPositions[r - 'a'] + mSize - mFirst) % mSize);
	}

	std::string getLetters() const
	{
		std::stringstream ss;
		for (size_t i = 0; i < mSize; ++i)
			ss << mLetters[(i + mFirst) % mSize];
		return ss.str();
	}

	std::array<int, mSize> getPositions() const
	{
		std::array<int, mSize> positions;
		for (size_t i = 0; i < mSize; ++i)
			positions[i] = (mPositions[i] + mSize - mFirst) % mSize;
		return positions;
	}

	int getSize() const
	{
		return mSize;
	}
	
private:
	std::string mLetters;
	int mFirst{ 0 };
	std::array<int, mSize> mPositions;
};

std::set<std::string> strings;

void BugFix<16>::solve1st()
{
	char command{ 0 }, garbage{ 0 };
	int from{ 0 }, to{ 0 };
	char left{ 0 }, right{ 0 };
	ProgramDance dance;

	while (*mIn >> command)
	{
		if (command == 's')
		{
			*mIn >> from;
			dance.spin(from);
		}
		else if (command == 'x')
		{
			*mIn >> from >> garbage >> to;
			dance.exchange(from, to);
		}
		else if (command == 'p')
		{
			*mIn >> left >> garbage >> right;
			dance.partner(left, right);
		}
		//*mOut << dance.getLetters() << std::endl;
		*mIn >> garbage;
	}
	*mOut << dance.getLetters() << std::endl;
}

void BugFix<16>::solve2nd()
{
	char command{ 0 }, garbage{ 0 };
	int from{ 0 }, to{ 0 };
	char left{ 0 }, right{ 0 };
	ProgramDance dance;

	while (*mIn >> command)
	{
		if (command == 's')
		{
			*mIn >> from;
			dance.spin(from);
		}
		else if (command == 'x')
		{
			*mIn >> from >> garbage >> to;
			dance.exchange(from, to);
		}
		else if (command == 'p')
		{
			*mIn >> left >> garbage >> right;
			dance.partner(left, right);
		}
		//*mOut << dance.getLetters() << std::endl;
		*mIn >> garbage;
	}
	auto positions = dance.getPositions();
	std::string firstDance = dance.getLetters();
	std::string letters = ProgramDance().getLetters();
	std::string letters2 = letters;

	strings.insert(letters);
	*mOut << letters << std::endl;

	for (size_t i = 0; i < 30; ++i)
	{
		letters2 = letters;
		for (int k = 0; k < dance.getSize(); ++k)
		{
			letters2[positions[k]] = letters[k];
		}
		letters = letters2;
		*mOut << letters << std::endl;

		if (strings.find(letters) != strings.end())
		{
			*mOut << i << " " << letters << std::endl;
			break;
		}
		strings.insert(letters);
	}

	*mOut << letters2 << std::endl; // gboiefnjpmkchald
}