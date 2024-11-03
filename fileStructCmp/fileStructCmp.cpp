#include "fileStructCmp.h"

int main(int argc, char* argv[])
{
	Check2F("test.txt", ".");
	return 0;
	if (argc < 3) {
		std::cout << "Missing Arguments\nUsage:\n\t" << argv[0] << " <folder1> <folder2>" << std::endl;
		return 2;
	}
	return 0;
}
