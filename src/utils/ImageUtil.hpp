#pragma once
#include "raylib.h"

namespace utp::utils {
	class ImageUtil {
	public:
		// copy of raylib's ImageCrop function, but doesn't modify the original image.
		static Image crop(const Image &image, Rectangle crop);
		static bool equals(const Image &a, const Image& b);
	};
} // namespace utp::utils
