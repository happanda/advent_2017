#include <fstream>
#include <iomanip>
#include <sstream>
#include "io.h"

std::unique_ptr<std::ifstream> getInput(int bugNum)
{
	std::stringstream sstr;
	sstr << "io/bug_" << std::setfill('0') << std::setw(2) << bugNum;
	sstr << ".in";
	std::string fileName = sstr.str();

	return std::make_unique<std::ifstream>(fileName);
}

std::unique_ptr<std::ofstream> getOutput(int bugNum)
{
	std::stringstream sstr;
	sstr << "io/bug_" << std::setfill('0') << std::setw(2) << bugNum;
	sstr << ".out";
	std::string fileName = sstr.str();

	return std::make_unique<std::ofstream>(fileName);
}
