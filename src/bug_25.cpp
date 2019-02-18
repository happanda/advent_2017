#include "advent.h"

uint32_t constexpr sChecksumAfter = 12964419;

enum class MoveDirection
{
	Left,
	Right
};

struct TuringState
{
	char id{ 0 };
	struct {
		bool value{ false };
		MoveDirection move{ MoveDirection::Right };
		char nextState{ 0 };
	} actions[2];
};

struct TuringTape
{
	void left()
	{
		--ptr;
		if (ptr < 0)
		{
			tape.push_front(false);
			ptr = 0;
		}
	}

	void right()
	{
		++ptr;
		if (ptr == tape.size())
		{
			tape.push_back(false);
		}
	}

	bool get() const
	{
		return tape[ptr];
	}

	void set(bool val)
	{
		tape[ptr] = val;
	}

	std::deque<bool> tape{ std::deque<bool>(1, false)};
	int32_t ptr{ 0 };
};

void BugFix<25>::solve1st()
{
	std::map<char, TuringState> machine;

	// adapted input for easier parsing
	std::string line;
	char curState{ 0 };
	uint8_t curRead{ 0 };
	while (std::getline(*mIn, line))
	{
		if (line[0] == ':')
		{
			curState = line[1];
			machine[curState];
		}
		else if (line[0] == '#')
		{
			curRead = line[1] - '0';
		}
		else if (line[0] == 'w')
		{
			machine[curState].actions[curRead].value = line[1] == '1';
		}
		else if (line[0] == 'm')
		{
			machine[curState].actions[curRead].move = (line[1] == 'L' ? MoveDirection::Left : MoveDirection::Right);
		}
		else if (line[0] == 'n')
		{
			machine[curState].actions[curRead].nextState = line[1];
		}
	}

	TuringTape tape;
	tape.set(0);
	curState = 'A';
	for (uint32_t k = 0; k < sChecksumAfter; ++k)
	{
		assert(machine.find(curState) != machine.end() && "Missing state");
		TuringState const& state = machine[curState];
		uint8_t const actionId = tape.get() ? 1 : 0;
		tape.set(state.actions[actionId].value);
		if (state.actions[actionId].move == MoveDirection::Left)
			tape.left();
		else
			tape.right();
		curState = state.actions[actionId].nextState;
	}

	uint32_t numOnes{ 0 };
	for (auto const& v : tape.tape)
		if (v)
			++numOnes;
	*mOut << numOnes << std::endl;
}

void BugFix<25>::solve2nd()
{
	*mOut << "" << std::endl;
}
