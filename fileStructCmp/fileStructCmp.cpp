#include "fileStructCmp.h"
#include <stdio.h>
#include <vector>
#include <string.h>

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
	size_t wrongBytes;
	size_t readWrite_Errors;

	size_t sameFiles;
};

void compare(baseData* dc, const std::filesystem::path& p, const std::filesystem::path& p2, int layer = 0, bool volatiled = true) {
	dc->scannedDirs++;
	if (!std::filesystem::exists(p)) {
		dc->differences++;
		p_layer(layer);
		printf("\"%s\" does not exist", conv_file(p));
		return;
	}
	if (!std::filesystem::exists(p2)) {
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
				if (p1l[i].p2 != "") continue;
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
		file::Diffe d = Check2F(p, p2);
		if (d.type == file::dType::size) {
			p_layer(layer);
			printf("Different file size on \"%s\"(%zu bytes) and \"%s\"(%zu bytes)\n", conv_file(p), d.data.sizeA, conv_file(p2), d.data.sizeB);
			dc->differences++;
			dc->fileInternDifferences++;
		}
		else if (d.type == file::dType::bytes) {
			if (d.data.diffcount > 0) {
				dc->differences++;
				dc->fileInternDifferences++;
				dc->wrongBytes += d.data.diffcount;
				p_layer(layer);
				printf("File \"%s\" and \"%s\" are differend by %zu bytes\n", conv_file(p), conv_file(p2), d.data.diffcount);
			}
			else {
				if (volatiled) {
					p_layer(layer);
					printf("File \"%s\" and \"%s\" are the same\n", conv_file(p), conv_file(p2));
				}
				dc->sameFiles++;
			}
		}
		else {
			dc->readWrite_Errors++;
			p_layer(layer);
			printf("An Error occured during processing File \"%s\" and \"%s\"\n", conv_file(p), conv_file(p2));
		}
	}
}


int main(int argc, char* argv[])
{
	if (argc < 3) {
		std::cout << "Missing Arguments\nUsage:\n\t" << argv[0] << " <folder1> <folder2> <optional \"v\" for verbose>" << std::endl;
		return 2;
	}
	baseData data;
	memset(&data, 0, sizeof(data));
	compare(&data, argv[1], argv[2], 0, argc >= 4 ? (argv[3][0] == 'v' ? true : false) : false);
	printf(
		"\n\n--------------------------------------------------------\n"
		"Scanned Dirs: %zu\n"
		"Differences: %zu\n"
		"Different Files: %zu\n"
		"\tMore Files in A: %zu\n\tMore Files in B: %zu\n"
		"Type Differences (Folder/File): %zu\n"
		"!!File Read Errors: %zu\n"
		"Correct Files: %zu\n"
		"Odd Bytes: %zu\n",
		data.scannedDirs,
		data.differences,
		data.fileInternDifferences,
		data.additionalFilesOnA,
		data.lessFilesOnA,
		data.typeDifferences,
		data.readWrite_Errors,
		data.sameFiles,
		data.wrongBytes
	);
	return 0;
}
