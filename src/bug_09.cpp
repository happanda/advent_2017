#include "advent.h"


enum class State
{
	None,
	Garbage,
	Group,
	Cancelling
};

struct StateMachine
{
	StateMachine()
	{
		mStack.push_back(State::None);
	}

	void tick(char symbol)
	{
		State curState = mStack.back();
		switch (curState)
		{
		case State::None:
			tickNone(symbol);
			break;
		case State::Garbage:
			tickGarbage(symbol);
			break;
		case State::Group:
			tickGroup(symbol);
			break;
		case State::Cancelling:
			tickCancelling(symbol);
			break;
		}
	}

	int getGroupSum() const
	{
		return mGroupSum;
	}

	int getGarbageCount() const
	{
		return mGarbageCount;
	}

private:
	void tickNone(char symbol)
	{
		switch (symbol)
		{
		case '!':
			enterCancelling();
			break;
		case '{':
			enterGroup();
			break;
		case '<':
			enterGarbage();
			break;
		}
	}

	void tickGarbage(char symbol)
	{
		switch (symbol)
		{
		case '!':
			enterCancelling();
			break;
		case '>':
			exitGarbage();
			break;
		default:
			++mGarbageCount;
			break;
		}
	}

	void tickGroup(char symbol)
	{
		switch (symbol)
		{
		case '!':
			enterCancelling();
			break;
		case '{':
			enterGroup();
			break;
		case '}':
			exitGroup();
			break;
		case '<':
			enterGarbage();
			break;
		}
	}

	void tickCancelling(char symbol)
	{
		exitCancelling();
	}

	void enterGarbage()
	{
		mStack.push_back(State::Garbage);
	}

	void exitGarbage()
	{
		mStack.pop_back();
	}

	void enterGroup()
	{
		if (mGroup++ == 0)
		{
			mStack.push_back(State::Group);
		}
		mGroupSum += mGroup;
	}

	void exitGroup()
	{
		if (--mGroup == 0)
			mStack.pop_back();
	}

	void enterCancelling()
	{
		mStack.push_back(State::Cancelling);
	}

	void exitCancelling()
	{
		mStack.pop_back();
	}

	std::deque<State> mStack;
	int mGroup{ 0 };
	int mGroupSum{ 0 };
	int mGarbageCount{ 0 };
};


void BugFix<9>::solve1st()
{
	StateMachine machine;

	std::string line;
	*mIn >> line;

	for (auto symbol : line)
	{
		machine.tick(symbol);
	}

	*mOut << machine.getGroupSum() << std::endl;
}

void BugFix<9>::solve2nd()
{
	StateMachine machine;

	std::string line;
	*mIn >> line;

	for (auto symbol : line)
	{
		machine.tick(symbol);
	}

	*mOut << machine.getGarbageCount() << std::endl;
}
