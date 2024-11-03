#pragma once

#include <iostream>
#include <filesystem>
#include <stdint.h>

namespace file {
	enum dType {
		unierror, size, bytes
	};
	union dataType {
		char sig;
		struct {
			size_t sizeA;
			size_t sizeB;
		};
		uint64_t diffcount;
	};
	struct Diffe {
		dType type;
		dataType data;
	};
};
file::Diffe Check2F(const std::filesystem::path&, const std::filesystem::path&);
