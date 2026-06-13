#pragma once

#include <cstdint>
#include <string>

namespace utp::utils {
	class StringUtil {
		public:
			static std::string formatBytes(size_t bytes, uint8_t precision = 2);
	};
} // namespace utp::utils
