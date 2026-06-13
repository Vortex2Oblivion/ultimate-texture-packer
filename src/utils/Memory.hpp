#pragma once

#include <cstddef>

namespace utp::utils {
	class Memory {
		public:
			static size_t getPeakRSS();
			static size_t getCurrentRSS();
	};
}
