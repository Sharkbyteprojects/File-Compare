#include "fileStructCmp.h"

#include <string.h>
#include <stdlib.h>
#include <fstream>
#define BUFFERSIZE 250
#define aBuf BUFFERSIZE + 1

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
	char* bufferGod = nullptr;


	bool fI = true;
	void killHub() {
		if (input.is_open())
			input.close();
		fileSize = 0;
		innerCount = 0;
		outerCount = 0;
		memset(bufferGod, 0, aBuf);
		fI = true;
	}
public:
	const size_t* siz() { return &fileSize; }
	milordFiler() {
		if (bufferGod == nullptr)
			bufferGod = (char*)malloc(aBuf);
	}
	bool openFiler(const std::filesystem::path& p) {
		if (bufferGod == nullptr) {
			std::cerr << "Buffer Alloc Error!\n";
			return 0;
		}
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
		if (bufferGod == nullptr) {
			std::cerr << "Error app corruption!\n";
			r.data = 4;
			r.ok = false;
			return r;
		}
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
		free(bufferGod);
		bufferGod = nullptr;
	}
};

file::Diffe Check2F(const std::filesystem::path& p1, const std::filesystem::path& p2) {
	milordFiler mf1, mf2;
	file::Diffe retn;
	memset(&retn, 0, sizeof(retn));

	if (!mf1.openFiler(p1)) {
		retn.type = file::dType::unierror;
		return retn;
	}
	if (!mf2.openFiler(p2)) {
		retn.type = file::dType::unierror;
		return retn;
	}

	if (*(mf1.siz()) != *(mf2.siz())) {
		retn.type = file::dType::size;
		retn.data.sizeA = *(mf1.siz());
		retn.data.sizeB = *(mf2.siz());
		return retn;
	}

	retn.type = file::dType::bytes;
	retn.data.diffcount = 0;

	auto f = mf1.readByte();
	auto f2 = mf2.readByte();
	while (f.ok && f2.ok) {
		if (f.data != f2.data)
			retn.data.diffcount++;

		f = mf1.readByte();
		f2 = mf2.readByte();
	}

	if (f.data == 4 || f2.data == 4) {
		retn.type = file::dType::unierror;
	}
	return retn;
}
