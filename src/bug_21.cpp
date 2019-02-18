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

	struct BinarySquareArrayView
	{
		BinarySquareArrayView(BinarySquareArray* bsa, Index x, Index y, Index size)
			: mArray(bsa), mX(x), mY(y), mSize(size)
		{
		}

		BinarySquareArrayView const& operator=(BinarySquareArray const& rhs)
		{
			assert(mSize == rhs.size());
			for (Index j = 0; j < mSize; ++j)
			{
				for (Index i = 0; i < mSize; ++i)
				{
					at(i, j) = rhs.at(i, j);
				}
			}
			return *this;
		}

		char& at(Index x, Index y)
		{
			return mArray->at(x + mX, y + mY);
		}

		char const& at(Index x, Index y) const
		{
			return mArray->at(x + mX, y + mY);
		}

		Index size() const
		{
			return mSize;
		}

		uint32_t hash() const
		{
			uint32_t hash{ 0 };
			uint32_t two{ 1 };
			for (Index y = 0; y < mY; ++y)
				for (Index x = 0; x < mX; ++x)
				{
					if (at(x, y))
					{
						hash += two;
					}
					two *= 2;
				}
			return hash;
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
		BinarySquareArray* mArray;
		Index mX{ 0 };
		Index mY{ 0 };
		Index mSize{ 0 };
	};

	BinarySquareArray(BinarySquareArrayView const& rhs)
	{
		mSize = rhs.size();
		mArray.resize(mSize * mSize);
		for (Index j = 0; j < mSize; ++j)
		{
			for (Index i = 0; i < mSize; ++i)
			{
				at(i, j) = rhs.at(i, j);
			}
		}
	}

private:
	Index mSize{ 0 };
	std::vector<char> mArray;
};

BinarySquareArray* newFromString(std::string const& str)
{
	auto const size = str.find('/');
	BinarySquareArray* sql = new BinarySquareArray(size);

	int x = 0, y = 0;
	for (uint32_t j = 0; j < str.size(); ++j)
	{
		if (str[j] == '/')
		{
			++y;
		}
		else
		{
			if (str[j] == '#')
			{
				sql->at(x, y) = true;
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

void lexLowest(BinarySquareArray::BinarySquareArrayView& bsv)
{
	BinarySquareArray bsCopy = bsv;
	lexLowest(bsCopy);
	bsv = bsCopy;
}

typedef BinarySquareArray::BinarySquareArrayView BSAView;

void BugFix<21>::solve1st()
{
	std::list<BinarySquareArray*> allSquares;
	std::unordered_map<BinarySquareArray, BinarySquareArray*> rules;

	BinarySquareArray screen(5000);
	BinarySquareArray screenCopy(screen.size());
	uint32_t screenSize = 3;

	BSAView startView(&screen, 0, 0, screenSize);
	startView.at(1, 0) = 1;
	startView.at(2, 1) = 1;
	startView.at(0, 2) = 1;
	startView.at(1, 2) = 1;
	startView.at(2, 2) = 1;
	lexLowest(startView);
	BinarySquareArray start = startView;
	rules[start] = nullptr;

	std::string line;
	while (std::getline(*mIn, line))
	{
		auto sepPos = line.find(" => ");
		const std::string ruleLeft = line.substr(0, sepPos);
		const std::string ruleRight = line.substr(sepPos + 4);

		BinarySquareArray* left = newFromString(ruleLeft);
		BinarySquareArray* right = newFromString(ruleRight);
		allSquares.push_back(left);
		allSquares.push_back(right);

		lexLowest(*left);
		rules[*left] = right;
	}

	/*for (auto const& rule : rules)
	{
		for (uint8_t y = 0; y < 4; ++y)
		{
			for (uint8_t x = 0; x < 3; ++x)
			{
				if (x < rule.first.size() && y < rule.first.size())
					*mOut << (rule.first.at(x, y) ? 'o' : '.');
				else
					*mOut << " ";
			}
			*mOut << "    ";
			for (uint8_t x = 0; x < 4; ++x)
			{
				if (x < rule.second->size() && y < rule.second->size())
					*mOut << (rule.second->at(x, y) ? 'o' : '.');
				else
					*mOut << " ";
			}
			*mOut << " ";
			*mOut << std::endl;
		}
		*mOut << "--------------------" << std::endl;
	}*/

	uint32_t step = 0;
	uint32_t sizePopped = 3;
	while (step < 5)
	{
		//*mOut << BSAView(&screen, 0, 0, 24).toString() << std::endl;
		screenCopy = screen;
		if (screenSize % 2 == 0)
		{
			for (uint32_t j = 0; j < screenSize / 2; ++j)
			{
				for (uint32_t i = 0; i < screenSize / 2; ++i)
				{
					BinarySquareArray bs = BSAView(&screen, i * 2, j * 2, 2);
					lexLowest(bs);
					auto const& substitute = *rules[bs];
					BSAView(&screenCopy, i * 3, j * 3, 3) = substitute;
				}
			}
			screenSize = screenSize / 2 * 3;
		}
		else
		{
			for (uint32_t j = 0; j < screenSize / 3; ++j)
			{
				for (uint32_t i = 0; i < screenSize / 3; ++i)
				{
					BinarySquareArray bs = BSAView(&screen, i * 3, j * 3, 3);
					lexLowest(bs);
					auto const& substitute = *rules[bs];
					BSAView(&screenCopy, i * 4, j * 4, 4) = substitute;
				}
			}
			screenSize = screenSize / 3 * 4;
		}
		screen = screenCopy;
		++step;
	}
	//*mOut << BSAView(&screen, 0, 0, 24).toString() << std::endl;

	uint32_t const numPixelsOn = screen.countTrue();
	*mOut << numPixelsOn << std::endl;
}

void BugFix<21>::solve2nd()
{
	// same but 18 steps
}
