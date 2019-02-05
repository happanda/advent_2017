#include "advent.h"


struct Generator15
{
	Generator15(long long seed, long long factor, long long base, int check = 1)
		: mSeed(seed)
		, mBase(base)
		, mFactor(factor)
		, mCheck(check)
	{
	}

	long long genNext()
	{
		mSeed = (mSeed * mFactor) % mBase;
		return mSeed;
	}

	long long genNextModified()
	{
		do
		{
			mSeed = (mSeed * mFactor) % mBase;
		} while (mSeed % mCheck != 0);
		return mSeed;
	}

private:
	long long mSeed{ 0 };
	long long const mBase{ 1 };
	long long const mFactor{ 0 };
	int mCheck{ 1 };
};


void BugFix<15>::solve1st()
{
	long long seed0{ 0 }, seed1{ 0 };

	*mIn >> seed0 >> seed1;
	Generator15 gen0(seed0, 16807, 2147483647);
	Generator15 gen1(seed1, 48271, 2147483647);

	int equalCount{ 0 };
	const size_t total = 40'000'000;
	const long long twoPow = 65535;
	for (size_t i = 0; i < total; ++i)
	{
		long long const gen0Next = gen0.genNext();
		long long const gen1Next = gen1.genNext();
		//*mOut << gen0Next << "\t" << gen1Next << std::endl;
		long long one = gen0Next & twoPow;
		long long two = gen1Next & twoPow;
		if (one == two)
			++equalCount;
	}

	*mOut << equalCount << std::endl;
}

void BugFix<15>::solve2nd()
{
	long long seed0{ 0 }, seed1{ 0 };

	*mIn >> seed0 >> seed1;
	Generator15 gen0(seed0, 16807, 2147483647, 4);
	Generator15 gen1(seed1, 48271, 2147483647, 8);

	int equalCount{ 0 };
	const size_t total = 5'000'000;
	const long long twoPow = 65535;
	for (size_t i = 0; i < total; ++i)
	{
		long long const gen0Next = gen0.genNextModified();
		long long const gen1Next = gen1.genNextModified();
		//*mOut << gen0Next << "\t" << gen1Next << std::endl;
		long long one = gen0Next & twoPow;
		long long two = gen1Next & twoPow;
		if (one == two)
			++equalCount;
	}

	*mOut << equalCount << std::endl;
}