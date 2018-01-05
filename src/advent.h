#pragma once
#include "io.h"


template <int bugNum>
struct BugFix
{
	BugFix()
		: mOut(getOutput(bugNum))
	{
	}

	void solve1()
	{
		mIn = std::move(getInput(bugNum));
		solve1st();
		mIn->close();
	}

	void solve2()
	{
		mIn = std::move(getInput(bugNum));
		solve2nd();
		mIn->close();
	}

private:
	void solve1st();
	void solve2nd();

	std::unique_ptr<std::ifstream> mIn;
	std::unique_ptr<std::ofstream> mOut;
};
