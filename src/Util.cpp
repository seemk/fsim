#include "Util.hpp"
#include <fstream>

namespace util
{
	std::string getFileContent(const char* fileName)
	{
		std::ifstream in(fileName, std::ios::in | std::ios::binary);
		std::string contents;
		if (in)
		{
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
		}
		return contents;
	}
}