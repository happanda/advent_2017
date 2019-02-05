#include "advent.h"
using std::int64_t;


constexpr int numRegisters = 26;


enum class ECommand
{
	Snd,
	Set,
	Add,
	Mul,
	Mod,
	Rcv,
	Jgz
};

struct Command
{
	int64_t arguments[2];
	char    letters[2];
	Command()
		: arguments{ 0, 0 }
		, letters{ 0, 0 }
	{ }
	virtual ~Command() { }

	void parseArg(int num, std::istream& args)
	{
		std::string arg;
		args >> arg;
		if (arg[0] >= 'a' && arg[0] <= 'z')
			letters[num] = arg[0];
		else
			arguments[num] = std::stoi(arg);
	}

	int64_t& getRegisterBy0(int64_t* registers)
	{
		return registers[static_cast<int>(letters[0] - 'a')];
	}

	int64_t& getRegisterBy1(int64_t* registers)
	{
		return registers[static_cast<int>(letters[1] - 'a')];
	}

	int64_t getArg0(int64_t* registers)
	{
		if (letters[0] != 0)
			return registers[static_cast<int>(letters[0] - 'a')];
		else return arguments[0];
	}

	int64_t getArg1(int64_t* registers)
	{
		if (letters[1] != 0)
			return registers[static_cast<int>(letters[1] - 'a')];
		else return arguments[1];
	}

	virtual void execute(int64_t* registers) = 0;

	virtual bool instructionOffset(int& out_value) const
	{
		return false;
	}

	void dbgPrint(std::ostream& stream)
	{
		stream << dbgName() << " ";
		if (letters[0] != 0)
			stream << letters[0];
		else
			stream << arguments[0];
		if (letters[1] != 0)
			stream << " " << letters[1];
		else if (arguments[1] != 0)
			stream << " " << arguments[1];
		stream << std::endl;
	}

protected:
	virtual const char* dbgName() const = 0;
};

struct Snd : public Command
{
	Snd(int64_t& storeFreq)
		: mStoreFreq(storeFreq) { }
	
	void execute(int64_t* registers) override
	{
		mStoreFreq = getArg0(registers);
	}

	const char* dbgName() const override
	{
		return "Snd";
	}

private:
	int64_t& mStoreFreq;
};

struct Set : public Command
{
	void execute(int64_t* registers) override
	{
		getRegisterBy0(registers) = getArg1(registers);
	}

	const char* dbgName() const override
	{
		return "Set";
	}
};

struct Add : public Command
{
	void execute(int64_t* registers) override
	{
		getRegisterBy0(registers) += getArg1(registers);
	}

	const char* dbgName() const override
	{
		return "Add";
	}
};

struct Mul : public Command
{
	void execute(int64_t* registers) override
	{
		getRegisterBy0(registers) *= getArg1(registers);
	}

	const char* dbgName() const override
	{
		return "Mul";
	}
};

struct Mod : public Command
{
	void execute(int64_t* registers) override
	{
		getRegisterBy0(registers) %= getArg1(registers);
	}

	const char* dbgName() const override
	{
		return "Mod";
	}
};

struct Rcv : public Command
{
	Rcv(bool& storeRecovered)
		: mStoreRecovered(storeRecovered) { }

	void execute(int64_t* registers) override
	{
		if (getArg0(registers) != 0)
			mStoreRecovered = true;
	}

	const char* dbgName() const override
	{
		return "Rcv";
	}

private:
	bool& mStoreRecovered;
};

struct Jgz : public Command
{
	void execute(int64_t* registers) override
	{
		if (getArg0(registers) > 0)
		{
			offset = static_cast<int>(getArg1(registers));
		}
		else
			offset = 1;
	}

	bool instructionOffset(int& out_value) const override
	{
		out_value = offset;
		return true;
	}

	const char* dbgName() const override
	{
		return "Jgz";
	}
private:
	int offset{ 0 };
};

struct Program;
Command* createCommand(std::istream& args, Program& program);

struct Program
{
	Program(std::istream& input)
	{
		Command* command = nullptr;
		while (command = createCommand(input, *this))
			mCommands.push_back(command);

		for (int i = 0; i < numRegisters; ++i)
			mRegisters[i] = 0;
		mInstruction = 0;
	}

	void execute()
	{
		mCommands[mInstruction]->execute(mRegisters.data());
		int offset{ 0 };
		if (mCommands[mInstruction]->instructionOffset(offset))
			mInstruction += offset;
		else
			++mInstruction;
	}

	int size() const
	{
		return mCommands.size();
	}

	int instruction() const
	{
		return mInstruction;
	}

	int64_t recentFreq() const
	{
		return mRecentFreq;
	}

	bool recovered() const
	{
		return mRecovered;
	}

	void dbgPrint(std::ostream& stream)
	{
		mCommands[mInstruction]->dbgPrint(stream);
		stream << " ";
		for (size_t i = 0; i < mRegisters.size(); ++i)
		{
			if (i > 0)
				stream << " ";
			stream << "[" << (char)('a' + i) << "] " << mRegisters[i];
		}
		stream << std::endl << " Instruction: " << mInstruction;
		stream << std::endl << std::endl;
	}

	int64_t mRecentFreq{ 0 };
	bool mRecovered{ false };
private:
	std::vector<Command*> mCommands;
	std::array<std::int64_t, numRegisters> mRegisters;
	int mInstruction{ 0 };
};

Command* createCommand(std::istream& args, Program& program)
{
	Command* result = nullptr;
	std::string command;
	if (args >> command)
	{
		if (command == "snd")
		{
			result = new Snd(program.mRecentFreq);
			result->parseArg(0, args);
		}
		else if (command == "rcv")
		{
			result = new Rcv(program.mRecovered);
			result->parseArg(0, args);
		}
		else
		{
			if (command == "set")
				result = new Set;
			else if (command == "add")
				result = new Add;
			else if (command == "mul")
				result = new Mul;
			else if (command == "mod")
				result = new Mod;
			else if (command == "jgz")
				result = new Jgz;
			if (result)
			{
				result->parseArg(0, args);
				result->parseArg(1, args);
			}
		}
	}
	return result;
}

void BugFix<18>::solve1st()
{
	//std::ofstream dbgLog("dbgLog.txt");
	Program program(*mIn);

	while (!program.recovered() && program.instruction() >= 0 && program.instruction() < static_cast<int>(program.size()))
	{
		//program.dbgPrint(dbgLog);
		program.execute();
	}
		
	*mOut << program.recentFreq() << std::endl;
}



struct SndDuet : public Command
{
	SndDuet(std::queue<int64_t>& inbox, int64_t& sndCounter)
		: mInbox(inbox), mSndCounter(sndCounter) { }

	void execute(int64_t* registers) override
	{
		mInbox.push(getArg0(registers));
		++mSndCounter;
	}

	const char* dbgName() const override
	{
		return "Snd";
	}

private:
	std::queue<int64_t>& mInbox;
	int64_t& mSndCounter;
};

struct RcvDuet : public Command
{
	RcvDuet(std::queue<int64_t>& inbox, bool& isHalt)
		: mInbox(inbox), mIsHalt(isHalt) { }

	void execute(int64_t* registers) override
	{
		if (!mInbox.empty())
		{
			getRegisterBy0(registers) = mInbox.front();
			mInbox.pop();
			mOffset = 1;
			mIsHalt = false;
		}
		else
		{
			mOffset = 0;
			mIsHalt = true;
		}
	}

	bool instructionOffset(int& out_value) const override
	{
		out_value = mOffset;
		return true;
	}

	const char* dbgName() const override
	{
		return "Rcv";
	}

private:
	int mOffset{ 0 };
	std::queue<int64_t>& mInbox;
	bool& mIsHalt;
};

struct ProgramDuet;
Command* createCommandDuet(std::istream& args, ProgramDuet& program);

struct ProgramDuet
{
	ProgramDuet(int id, std::istream& input)
		: mProgId(id)
	{
		Command* command = nullptr;
		while (command = createCommandDuet(input, *this))
			mCommands.push_back(command);

		for (int i = 0; i < numRegisters; ++i)
			mRegisters[i] = 0;
		mRegisters['p' - 'a'] = mProgId;
		mInstruction = 0;
	}

	void execute()
	{
		if (mInstruction < 0 || mInstruction > static_cast<int>(mCommands.size()))
		{
			mIsHalt = true;
			return;
		}
		mCommands[mInstruction]->execute(mRegisters.data());
		int offset{ 0 };
		if (mCommands[mInstruction]->instructionOffset(offset))
			mInstruction += offset;
		else
			++mInstruction;
	}

	int size() const
	{
		return mCommands.size();
	}

	int instruction() const
	{
		return mInstruction;
	}

	int progId() const
	{
		return mProgId;
	}

	void dbgPrint(std::ostream& stream)
	{
		stream << "#" << mProgId << std::endl;
		mCommands[mInstruction]->dbgPrint(stream);
		stream << " ";
		for (size_t i = 0; i < mRegisters.size(); ++i)
		{
			if (i > 0)
				stream << " ";
			stream << "[" << (char)('a' + i) << "] " << mRegisters[i];
		}
		stream << std::endl << " Instruction: " << mInstruction;
		stream << std::endl << std::endl;
	}

	int64_t mRecentFreq{ 0 };
	bool mRecovered{ false };
	bool mIsHalt{ false };
	int64_t mSndCounter{ 0 };
	static std::queue<int64_t> sInbox[2]; // bad design, but I'm not willing to make it better
private:
	std::vector<Command*> mCommands;
	std::array<std::int64_t, numRegisters> mRegisters;
	int mInstruction{ 0 };
	const int mProgId{ -1 };
};

std::queue<int64_t> ProgramDuet::sInbox[2];

Command* createCommandDuet(std::istream& args, ProgramDuet& program)
{
	Command* result = nullptr;
	std::string command;
	if (args >> command)
	{
		if (command == "snd")
		{
			result = new SndDuet(ProgramDuet::sInbox[1 - program.progId()], program.mSndCounter);
			result->parseArg(0, args);
		}
		else if (command == "rcv")
		{
			result = new RcvDuet(ProgramDuet::sInbox[program.progId()], program.mIsHalt);
			result->parseArg(0, args);
		}
		else
		{
			if (command == "set")
				result = new Set;
			else if (command == "add")
				result = new Add;
			else if (command == "mul")
				result = new Mul;
			else if (command == "mod")
				result = new Mod;
			else if (command == "jgz")
				result = new Jgz;
			if (result)
			{
				result->parseArg(0, args);
				result->parseArg(1, args);
			}
		}
	}
	return result;
}

void BugFix<18>::solve2nd()
{
	//std::ofstream dbgLog("dbgLog.txt");
	std::stringstream sstr;
	sstr << mIn->rdbuf();

	ProgramDuet program0(0, sstr);
	sstr.clear();
	sstr.seekg(0, std::ios::beg);
	ProgramDuet program1(1, sstr);

	while (true)
	{
		//program0.dbgPrint(dbgLog);
		//program1.dbgPrint(dbgLog);
		program0.execute();
		program1.execute();
		if (program0.mIsHalt && program1.mIsHalt)
			break;
	}

	*mOut << program1.mSndCounter << std::endl;
}