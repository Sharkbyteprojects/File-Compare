#include "fileStructCmp.h"
#include <stdio.h>
#include <vector>

const char* dir_strmut(bool dir) { return dir ? "Directory" : "File"; }
void p_layer(int layer) { for (uint64_t i = 0; i < layer; i++) std::cout << "  "; std::cout << "| "; }
#define conv_file(x) x.string().c_str()
struct pair {
	std::filesystem::path p1;
	std::filesystem::path p2;
};

struct baseData {
	size_t scannedDirs;
	size_t typeDifferences;
	size_t differences;

	size_t additionalFilesOnA;
	size_t lessFilesOnA;

	size_t fileInternDifferences;
};

void compare(baseData* dc, std::filesystem::path& p, std::filesystem::path& p2, int layer = 0, bool volatiled = true) {
	dc->scannedDirs++;
	if (std::filesystem::exists(p)) {
		dc->differences++;
		p_layer(layer);
		printf("\"%s\" does not exist", conv_file(p));
		return;
	}
	if (std::filesystem::exists(p2)) {
		dc->differences++;
		p_layer(layer);
		printf("\"%s\" does not exist", conv_file(p2));
		return;
	}
	bool dir = std::filesystem::is_directory(p);
	bool dir2 = std::filesystem::is_directory(p2);

	if (volatiled) {
		p_layer(layer);
		printf("Comparing %s \"%s\" and %s \"%s\"\n", dir_strmut(dir), conv_file(p), dir_strmut(dir2), conv_file(p2));
	}

	if (dir != dir2) {
		dc->differences++;
		dc->typeDifferences++;
		p_layer(layer);
		printf("%s \"%s\" and %s \"%s\" are different Types\n", dir_strmut(dir), conv_file(p), dir_strmut(dir2), conv_file(p2));
	}
	else if (dir && dir2) {
		if (volatiled) {
			p_layer(layer);
			std::cout << "Walking directory \"" << conv_file(p) << "\" and \"" << conv_file(p2) << "\"\n";
		}
		std::vector<pair> p1l;
		for (const auto& entry : std::filesystem::directory_iterator(p)) {
			pair p;
			p.p1 = entry.path();
			p.p2 = "";
			p1l.push_back(p);
		}
		for (const auto& entry : std::filesystem::directory_iterator(p2)) {
			bool found = false;
			for (size_t i = 0; i < p1l.size(); i++) {
				if (p1l[i].p2 == "") continue;
				if (p1l[i].p1.filename().string() == entry.path().filename().string()) {
					found = true;
					p1l[i].p2 = entry.path();
					break;
				}
			}

			if (!found) {
				p_layer(layer);
				printf("Object \"%s\" only exists on Side B\n", conv_file(entry.path()));
				dc->differences++;
				dc->lessFilesOnA++;
			}
		}

		for (size_t i = 0; i < p1l.size(); i++) {
			if (p1l[i].p2 == "") {
				p_layer(layer);
				printf("Object \"%s\" only exists on Side A\n", conv_file(p1l[i].p1));
				dc->differences++;
				dc->additionalFilesOnA++;
				continue;
			}
			compare(dc, p1l[i].p1, p1l[i].p2, layer + 1, volatiled);
		}
	}
	else {
		//File Compare
		auto d = Check2F(p, p2);
	}
}

int main(int argc, char* argv[])
{
	Check2F("test.txt", "test.txt");
	return 0;
	if (argc < 3) {
		std::cout << "Missing Arguments\nUsage:\n\t" << argv[0] << " <folder1> <folder2>" << std::endl;
		return 2;
	}
	//compare()
	return 0;
}
