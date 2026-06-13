#include "StringUtil.hpp"

#include <array>
#include "raylib.h"

namespace utp::utils {
	std::string StringUtil::formatBytes(size_t bytes, const uint8_t precision) {
		const std::array<std::string, 5> suffix = {"B", "KB", "MB", "GB", "TB"};

		int i = 0;
		auto dblBytes = static_cast<double>(bytes);

		if (bytes > 1024) {
			for (i = 0; (bytes / 1024) > 0 && i < suffix.size() - 1; i++, bytes /= 1024) {
				dblBytes = static_cast<double>(bytes) / 1024.0;
			}
		}
		return TextFormat("%.*lf %s", precision, dblBytes, suffix[i].c_str());
	}
}
