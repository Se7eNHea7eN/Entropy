#pragma once
#include "Framework/Common/EntropyCore.hpp"

namespace Entropy {
	class EntropyApp {
	public:
		EntropyApp();
		~EntropyApp();
		int run(int _argc, const char* const* _argv);
		EntropyCore* entropyCore;
	};
}
