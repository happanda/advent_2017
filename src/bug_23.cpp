#include "advent.h"
namespace bug23
{
	using std::int64_t;

	static uint64_t sNumMulExecuted = 0;
	constexpr int numRegisters = 8;


	enum class ECommand
	{
		Set,
		Sub,
		Mul,
		Jnz
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

	struct Sub : public Command
	{
		void execute(int64_t* registers) override
		{
			getRegisterBy0(registers) -= getArg1(registers);
		}

		const char* dbgName() const override
		{
			return "Sub";
		}
	};

	struct Mul : public Command
	{
		void execute(int64_t* registers) override
		{
			getRegisterBy0(registers) *= getArg1(registers);
			++sNumMulExecuted;
		}

		const char* dbgName() const override
		{
			return "Mul";
		}
	};

	struct Jnz : public Command
	{
		void execute(int64_t* registers) override
		{
			if (getArg0(registers) != 0)
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
			return "Jnz";
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

		int64_t getRegister(uint32_t registerID)
		{
			return mRegisters[registerID];
		}

		void setRegister(uint32_t registerID, int64_t value)
		{
			mRegisters[registerID] = value;
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
			if (command == "set")
				result = new Set;
			else if (command == "sub")
				result = new Sub;
			else if (command == "mul")
				result = new Mul;
			else if (command == "jnz")
				result = new Jnz;
			if (result)
			{
				result->parseArg(0, args);
				result->parseArg(1, args);
			}
		}
		return result;
	}
}

void BugFix<23>::solve1st()
{
	using namespace bug23;
	sNumMulExecuted = 0;
	//std::ofstream dbgLog("dbgLog.txt");
	Program program(*mIn);

	while (!program.recovered() && program.instruction() >= 0 && program.instruction() < static_cast<int>(program.size()))
	{
		//program.dbgPrint(dbgLog);
		program.execute();
	}

	*mOut << sNumMulExecuted << std::endl;
}

void BugFix<23>::solve2nd()
{
	uint32_t result = 0;
	for (uint32_t k = 108100; k <= 125100; k += 17)
	{
		uint32_t numProds = 0;

		uint32_t const sqrtK = static_cast<uint32_t>(std::floor(std::sqrt(k)));
		for (uint32_t x = 2; x < sqrtK; ++x)
		{
			if (k % x == 0)
			{
				++result;
				break;
			}
		}
	}
	*mOut << result << std::endl;
}
