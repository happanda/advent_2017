#include "advent.h"


struct BinarySquareArray
{
	typedef uint32_t Index;

	BinarySquareArray(Index size)
		: mSize(size)
	{
		mArray.resize(mSize * mSize);
		std::fill(mArray.begin(), mArray.end(), false);
	}

	BinarySquareArray& operator=(BinarySquareArray const& rhs) = default;
	
	char& at(Index x, Index y)
	{
		return mArray.at(y * mSize + x);
	}

	char const& at(Index x, Index y) const
	{
		return mArray.at(y * mSize + x);
	}

	Index size() const
	{
		return mSize;
	}

	uint32_t countTrue() const
	{
		return std::count(mArray.begin(), mArray.end(), 1);
	}

	uint32_t hash() const
	{
		uint32_t hash{ 0 };
		uint32_t two{ 1 };
		for (auto c : mArray)
		{
			if (c)
			{
				hash += two;
			}
			two *= 2;
		}
		return hash;//std::_Hash_bytes(static_cast<const unsigned char*>(static_cast<const void*>(mArray.data())), sizeof(char) * mSize);
	}

	char* data()
	{
		return mArray.data();
	}

	std::string toString() const
	{
		std::ostringstream oss;
		for (Index j = 0; j < mSize; ++j)
		{
			for (Index i = 0; i < mSize; ++i)
			{
				oss << (at(i, j) ? 'o' : '.');
			}
			if (j < mSize)
				oss << std::endl;
		}
		return oss.str();
	}

private:
	Index mSize{ 0 };
	std::vector<char> mArray;
};

std::list<BinarySquareArray*> newListFromString(std::string const& str)
{
	auto const size = str.find('/');
	std::list<BinarySquareArray*> sql;
	if (size == 2)
		sql.push_back(new BinarySquareArray(2));
	else if (size == 3)
		sql.push_back(new BinarySquareArray(3));
	else
	{
		sql.push_front(new BinarySquareArray(2));
		sql.push_back(new BinarySquareArray(2));
	}

	int x = 0, y = 0;
	for (uint32_t j = 0; j < str.size(); ++j)
	{
		if (str[j] == '/')
		{
			++y;
			if (size == 4 && y == 2)
			{
				sql.push_front(new BinarySquareArray(2));
				sql.push_back(new BinarySquareArray(2));
			}
		}
		else
		{
			if (str[j] == '#')
			{
				if (size < 4)
				{
					sql.back()->at(x, y) = true;
				}
				else
				{
					if (x < 2)
						sql.front()->at(x, y % 2) = true;
					else
						sql.back()->at(x % 2, y % 2) = true;
				}
			}
			x = (x + 1) % size;
		}
	}
	return sql;
}

namespace std
{
	template <>
	struct hash<BinarySquareArray>
	{
		size_t operator()(BinarySquareArray const& bsp) const noexcept
		{
			return bsp.hash();
		}
	};
}

bool operator==(BinarySquareArray const& lhs, BinarySquareArray const& rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	return lhs.hash() == rhs.hash();
}

bool operator!=(BinarySquareArray const& lhs, BinarySquareArray const& rhs)
{
	return !(lhs == rhs);
}

bool operator<(BinarySquareArray const& lhs, BinarySquareArray const& rhs)
{
	assert(lhs.size() == rhs.size() && "Less can't compare mix-sized squares");
	for (BinarySquareArray::Index y = 0; y < lhs.size(); ++y)
		for (BinarySquareArray::Index x = 0; x < lhs.size(); ++x)
		{
			if (lhs.at(x, y) < rhs.at(x, y))
				return true;
			if (lhs.at(x, y) > rhs.at(x, y))
				return false;
		}
	return false;
}

void lexLowest(BinarySquareArray& bs)
{
	std::array<std::array<uint8_t, 4>, 7> perms2;
	perms2[0] = { 1, 3, 0, 2 };
	perms2[1] = { 3, 2, 1, 0 };
	perms2[2] = { 2, 0, 3, 1 };
	perms2[3] = { 1, 0, 3, 2 };
	perms2[4] = { 0, 2, 1, 3 };
	perms2[5] = { 2, 3, 0, 1 };
	perms2[6] = { 3, 1, 2, 0 };

	std::array<std::array<uint8_t, 9>, 7> perms3;
	perms3[0] = { 2, 1, 0, 5, 4, 3, 8, 7, 6 };
	perms3[1] = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };
	perms3[2] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
	perms3[3] = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
	perms3[4] = { 6, 7, 8, 3, 4, 5, 0, 1, 2 };
	perms3[5] = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
	perms3[6] = { 8, 5, 2, 7, 4, 1, 6, 3, 0 };

	BinarySquareArray bsCopy = bs;
	if (bsCopy.size() == 2)
	{
		for (auto const& prm : perms2)
		{
			BinarySquareArray newBs(2);
			for (uint8_t i = 0; i < prm.size(); ++i)
				newBs.data()[prm[i]] = bs.data()[i];
			if (newBs < bsCopy)
				bsCopy = newBs;
		}
	}
	else
	{
		for (auto const& prm : perms3)
		{
			BinarySquareArray newBs(3);
			for (uint8_t i = 0; i < prm.size(); ++i)
				newBs.data()[prm[i]] = bs.data()[i];
			if (newBs < bsCopy)
				bsCopy = newBs;
		}
	}
	assert(bs.countTrue() == bsCopy.countTrue() && "True count changed?");
	bs = bsCopy;
}


void BugFix<21>::solve1st()
{
	std::list<BinarySquareArray*> allSquares;
	std::unordered_map<BinarySquareArray, std::list<BinarySquareArray*>> rules;

	BinarySquareArray start(3);
	start.at(1, 0) = 1;
	start.at(2, 1) = 1;
	start.at(0, 2) = 1;
	start.at(1, 2) = 1;
	start.at(2, 2) = 1;
	lexLowest(start);
	rules[start] = std::list<BinarySquareArray*>();

	std::string line;
	while (std::getline(*mIn, line))
	{
		auto sepPos = line.find(" => ");
		const std::string ruleLeft = line.substr(0, sepPos);
		const std::string ruleRight = line.substr(sepPos + 4);

		std::list<BinarySquareArray*> left = newListFromString(ruleLeft);
		std::list<BinarySquareArray*> right = newListFromString(ruleRight);
		allSquares.insert(allSquares.end(), left.begin(), left.end());
		allSquares.insert(allSquares.end(), right.begin(), right.end());

		lexLowest(*left.front());
		for (auto bs : right)
			lexLowest(*bs);

		assert(!right.empty() && "Empty rules?");
		//assert(rules.find(*left.front()) == rules.end() && "Overriding rule?");
		for (auto bs : right)
		{
			if (rules.find(*bs) == rules.end())
				rules[*bs] = std::list<BinarySquareArray*>();
		}
		if (left.front()->size() == 3 && rules.find(*left.front()) == rules.end())
			continue;
		rules[*left.front()] = std::move(right);
	}

	for (auto const& rule : rules)
	{
		for (uint8_t y = 0; y < 3; ++y)
		{
			for (uint8_t x = 0; x < 3; ++x)
			{
				if (x < rule.first.size() && y < rule.first.size())
					*mOut << (rule.first.at(x, y) ? 'o' : '.');
				else
					*mOut << " ";
			}
			*mOut << "   ";
			for (auto rbs : rule.second)
			{
				for (uint8_t x = 0; x < 3; ++x)
				{
					if (x < rbs->size() && y < rbs->size())
						*mOut << (rbs->at(x, y) ? 'o' : '.');
					else
						*mOut << " ";
				}
				*mOut << " ";
			}
			*mOut << std::endl;
		}
		*mOut << "--------------------" << std::endl;
	}

	std::list<BinarySquareArray*> queue;
	queue.push_back(&start);
	uint32_t step = 0;
	uint32_t sizePopped = 3;
	while (step < 5)
	{
		//*mOut << "Square:" << std::endl << queue.front()->toString() << std::endl;
		if (queue.front()->size() != sizePopped)
		{
			for (uint8_t y = 0; y < 3; ++y)
			{
				for (auto rbs : queue)
				{
					for (uint8_t x = 0; x < 3; ++x)
					{
						if (x < rbs->size() && y < rbs->size())
							*mOut << (rbs->at(x, y) ? 'o' : '.');
						else
							*mOut << " ";
					}
					*mOut << " ";
				}
				*mOut << std::endl;
			}
			*mOut << "=====================" << std::endl;
			++step;
			/*if (step == 5)
				break;*/
			sizePopped = queue.front()->size();
		}
		auto it = rules.find(*queue.front());
		assert(it != rules.end() && "Can't find a rule");
		assert(it->first.size() == queue.front()->size() && "Sizes don't match?");
		auto const& rightSide = rules[*queue.front()];
		for (auto bs : rightSide)
		{
			//*mOut << bs->toString();
			queue.push_back(bs);
		}
		//*mOut << std::endl;
		queue.erase(queue.begin());
	}

	uint32_t numPixelsOn = 0;
	while (!queue.empty())
	{
		numPixelsOn += queue.front()->countTrue();
		queue.erase(queue.begin());
	}
	*mOut << numPixelsOn << std::endl;
}

void BugFix<21>::solve2nd()
{
	std::list<BinarySquareArray*> allSquares;
	std::unordered_map<BinarySquareArray, std::list<BinarySquareArray*>> rules;

	std::string line;
	while (std::getline(*mIn, line))
	{
		auto sepPos = line.find(" => ");
		const std::string ruleLeft = line.substr(0, sepPos);
		const std::string ruleRight = line.substr(sepPos + 4);

		std::list<BinarySquareArray*> left = newListFromString(ruleLeft);
		//std::list<BinarySquareArray*> leftc = newListFromString(ruleLeft);
		std::list<BinarySquareArray*> right = newListFromString(ruleRight);
		allSquares.insert(allSquares.end(), left.begin(), left.end());
		allSquares.insert(allSquares.end(), right.begin(), right.end());

		for (uint8_t y = 0; y < 3; ++y)
		{
			for (uint8_t x = 0; x < 3; ++x)
			{
				if (x < left.front()->size() && y < left.front()->size())
					*mOut << (left.front()->at(x, y) ? 'o' : '.');
				else
					*mOut << " ";
			}
			*mOut << "   ";
			for (auto rbs : right)
			{
				for (uint8_t x = 0; x < 3; ++x)
				{
					if (x < rbs->size() && y < rbs->size())
						*mOut << (rbs->at(x, y) ? 'o' : '.');
					else
						*mOut << " ";
				}
				*mOut << " ";
			}
			*mOut << std::endl;
		}
		*mOut << std::endl;

		lexLowest(*left.front());
		for (auto bs : right)
			lexLowest(*bs);

		for (uint8_t y = 0; y < 3; ++y)
		{
			for (uint8_t x = 0; x < 3; ++x)
			{
				if (x < left.front()->size() && y < left.front()->size())
					*mOut << (left.front()->at(x, y) ? 'o' : '.');
				else
					*mOut << " ";
			}
			*mOut << "   ";
			for (auto rbs : right)
			{
				for (uint8_t x = 0; x < 3; ++x)
				{
					if (x < rbs->size() && y < rbs->size())
						*mOut << (rbs->at(x, y) ? 'o' : '.');
					else
						*mOut << " ";
				}
				*mOut << " ";
			}
			*mOut << std::endl;
		}
		*mOut << "--------------------" << std::endl;

		assert(!right.empty() && "Empty rules?");
		assert(rules.find(*left.front()) == rules.end() && "Overriding rule?");
		rules[*left.front()] = std::move(right);
		//*mOut << left.front()->toString() << std::endl;
	}

	BinarySquareArray start(3);
	start.at(1, 0) = 1;
	start.at(2, 1) = 1;
	start.at(0, 2) = 1;
	start.at(1, 2) = 1;
	start.at(2, 2) = 1;
	lexLowest(start);

	std::queue<BinarySquareArray*> queue;
	queue.push(&start);
	uint32_t step = 0;
	uint32_t sizePopped = 3;
	while (step < 18)
	{
		if (queue.front()->size() != sizePopped)
		{
			++step;
			if (step == 18)
				break;
			sizePopped = queue.front()->size();
		}
		auto it = rules.find(*queue.front());
		assert(it != rules.end() && "Can't find a rule");
		assert(it->first.size() == queue.front()->size() && "Sizes don't match?");
		auto const& rightSide = rules[*queue.front()];
		for (auto bs : rightSide)
		{
			queue.push(bs);
		}
		queue.pop();
	}

	uint32_t numPixelsOn = 0;
	while (!queue.empty())
	{
		numPixelsOn += queue.front()->countTrue();
		queue.pop();
	}
	*mOut << numPixelsOn << std::endl;
}
