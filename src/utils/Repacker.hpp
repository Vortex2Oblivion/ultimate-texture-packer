#pragma once

#include <vector>

#include "data/Frame.hpp"
#include "raylib.h"

namespace utp::utils {
	class Repacker {
	public:
		static std::vector<data::Frame> repack(Image &dst, const Image &src, int width, int height, const std::vector<data::Frame> &frames, bool allowRotate = true, bool crop = true);
	};
} // namespace utp::utils