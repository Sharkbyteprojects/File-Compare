#include "fileStructCmp.h"

#include <string.h>
#include <stdlib.h>
#include <fstream>
#define BUFFERSIZE 250

struct rO {
	bool ok;
	char data;
};

class milordFiler {
private:
	std::ifstream input;
	size_t fileSize = 0;
	size_t innerCount = 0;
	size_t outerCount = 0;
	char bufferGod[BUFFERSIZE + 1];


	bool fI = true;
	void killHub() {
		if (input.is_open())
			input.close();
		fileSize = 0;
		innerCount = 0;
		outerCount = 0;
		memset(bufferGod, 0, BUFFERSIZE + 1);
		fI = true;
	}
public:
	bool openFiler(const std::filesystem::path& p) {
		killHub();
		fileSize = std::filesystem::file_size(p);
			input.open(p, std::ios::binary);

			if (!input.is_open()) {
				std::cerr << "Error opening the file!\n";
				return 0;
			}
		return 1;
	}

	rO readByte() {
		rO r{ 0, 0 };
		auto toRead = fileSize - outerCount;
		if (toRead > BUFFERSIZE) {
			toRead = BUFFERSIZE;
		}
		if (innerCount + outerCount >= fileSize) {
			r.data = 0;
			r.ok = false;
			return r;
		}
		else if (innerCount >= toRead || fI) {
				input.read(bufferGod, toRead);
			outerCount += innerCount;
			innerCount = 0;
			fI = false;
		}
			r.data = bufferGod[innerCount];
		r.ok = true;
		innerCount++;
		return r;
	}

	~milordFiler() {
		killHub();
	}
};

void Check2F(const std::filesystem::path& p1, const std::filesystem::path& p2) {
	milordFiler mf1;
	mf1.openFiler(p1);
	auto f = mf1.readByte();
	while (f.ok) {
		char n[2]{ 0,0 };
		n[0] = f.data;
		std::cout << n;

		f = mf1.readByte();
	}
}
