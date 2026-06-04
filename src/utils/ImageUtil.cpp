#include "ImageUtil.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace utp::utils {
	Image ImageUtil::crop(const Image &image, Rectangle crop) {
		// Security check to avoid program crash
		if ((image.data == nullptr) || (image.width == 0) || (image.height == 0))
			return image;

		// Security checks to validate crop rectangle
		if (crop.x < 0) {
			crop.width += crop.x;
			crop.x = 0;
		}
		if (crop.y < 0) {
			crop.height += crop.y;
			crop.y = 0;
		}
		if ((crop.x + crop.width) > static_cast<float>(image.width))
			crop.width = static_cast<float>(image.width) - crop.x;
		if ((crop.y + crop.height) > static_cast<float>(image.height))
			crop.height = static_cast<float>(image.height) - crop.y;
		if ((crop.x > static_cast<float>(image.width)) || (crop.y > static_cast<float>(image.height))) {
			TraceLog(LOG_WARNING, "IMAGE: Failed to crop, rectangle out of bounds");
			return image;
		}

		if (image.mipmaps > 1)
			TraceLog(LOG_WARNING, "Image manipulation only applied to base mipmap level");
		if (image.format >= PIXELFORMAT_COMPRESSED_DXT1_RGB)
			TraceLog(LOG_WARNING, "Image manipulation not supported for compressed formats");
		else {
			const int bytesPerPixel = GetPixelDataSize(1, 1, image.format);

			auto *croppedData = static_cast<unsigned char *>(RL_MALLOC(static_cast<int>(crop.width * crop.height) * bytesPerPixel));

			// OPTION 1: Move cropped data line-by-line
			for (int y = static_cast<int>(crop.y), offsetSize = 0; y < static_cast<int>(crop.y + crop.height); y++) {
				memcpy(croppedData + offsetSize, static_cast<unsigned char *>(image.data) + (y * image.width + static_cast<int>(crop.x)) * bytesPerPixel,
					   static_cast<int>(crop.width) * bytesPerPixel);
				offsetSize += static_cast<int>(crop.width) * bytesPerPixel;
			}

			return Image{.data = croppedData, .width = static_cast<int>(crop.width), .height = static_cast<int>(crop.height), .mipmaps = image.mipmaps, .format = image.format};
		}
		return image;
	}
} // namespace utp::utils
