#include "advent.h"


void BugFix<17>::solve1st()
{
	const int NumSteps = 359;
	const int NumNumbers = 2017;

	std::vector<int> nums;
	nums.push_back(0);
	int curPos = 0;

	for (int i = 1; i <= NumNumbers; ++i)
	{
		int pos = (NumSteps + curPos) % nums.size();
		auto insPlace = nums.begin();
		std::advance(insPlace, pos);
		nums.insert(insPlace + 1, i);
		curPos = pos + 1;
	}
	int resPos = 0;
	for (size_t i = 0; i < nums.size(); ++i)
	{
		if (nums[i] == NumNumbers)
		{
			if (i != nums.size() - 1)
				resPos = i + 1;
		}
	}

	*mOut << nums[resPos] << std::endl;
}

void BugFix<17>::solve2nd()
{
	const int NumSteps = 359;
	const int NumNumbers = 50000000;

	int zeroPlace = 0;
	int numAfterZero = 0;

	int curPos = 0;
	for (int i = 1; i <= NumNumbers; ++i)
	{
		int pos = (NumSteps + curPos) % i;
		if (pos < zeroPlace)
			++zeroPlace;
		else if (pos == zeroPlace)
			numAfterZero = i;
		curPos = pos + 1;
	}

	*mOut << numAfterZero << std::endl;
}