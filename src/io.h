#pragma once
#include <fstream>
#include <memory>


std::unique_ptr<std::ifstream> getInput(int bugNum);
std::unique_ptr<std::ofstream> getOutput(int bugNum);

struct IO
{

private:
	std::unique_ptr<std::ifstream> mIn;
	std::unique_ptr<std::ofstream> mOut;
};
