#include "Repacker.hpp"

#include <iostream>

#include "ImageUtil.hpp"
#include "MaxRectsBinPack.h"
#include "raymath.h"

namespace utp::utils {
	std::vector<data::Frame> Repacker::repack(Image &dst, const Image &src, const int width, const int height,
											  const std::vector<data::Frame> &frames, const bool allowRotate, const bool crop) {
		dst = GenImageColor(width, height, BLANK);
		auto packer = rbp::MaxRectsBinPack(width, height,
#ifdef false
										   allowRotate
#else
										   false
#endif
		);

		std::vector<data::Frame> packedFrames = {};
		std::vector<data::Frame> outputFrames = {};

		std::vector<Image> rawFrames = {};

		int croppedWidth = 0;
		int croppedHeight = 0;

		size_t i = 0;

		for (const auto &frame: frames) {
			bool wasPacked = false;

			Image newFrameImage = ImageUtil::crop(src, static_cast<Rectangle>(frame));

			/*const float lastWidth = frame.width;
			const float lastHeight = frame.height;*/

			for (const auto rawFrame: rawFrames) {
				if (ImageUtil::equals(rawFrame, newFrameImage)) {
					wasPacked = true;
					break;
				}
			}

			rbp::Rect repackedRect = wasPacked ? static_cast<rbp::Rect>(packedFrames[i - 1])
											   : packer.Insert(static_cast<int>(frame.width), static_cast<int>(frame.height),
															   rbp::MaxRectsBinPack::RectBestAreaFit);

			//std::cout << frame.name << ": " << frame.width << std::endl;

			// data::Frame packedFrameData = frame;

			auto newFrameData = data::Frame{.x = static_cast<float>(repackedRect.x),
											.y = static_cast<float>(repackedRect.y),
											.width = static_cast<float>(repackedRect.width),
											.height = static_cast<float>(repackedRect.height),
											.frameX = frame.frameX,
											.frameY = frame.frameY,
											.frameWidth = frame.frameWidth,
											.frameHeight = frame.frameHeight,
											.rotated = false,
											.name = frame.name};

			// frame.frameX -= std::abs(lastWidth - newFrameData.width);
			// frame.frameY -= lastHeight - newFrameData.height;

			bool trimmed = frame.width < frame.frameWidth || frame.height < frame.frameHeight;

			//newFrameData.frameWidth = std::max(newFrameData.frameWidth, newFrameData.width - newFrameData.frameX);
			//newFrameData.frameHeight = std::max(newFrameData.frameHeight, newFrameData.height - newFrameData.frameY);

			// TODO: fix frame x offsets
			//newFrameData.frameX += (newFrameData.width - frame.width);
			newFrameData.frameY += newFrameData.height - frame.height;

			if (!wasPacked) {
				// std::cout << newFrameData.x << std::endl;
			}

			if (!wasPacked) {
				ImageDrawImagePro(&dst, newFrameImage,
								  Rectangle{.x = 0.0f,
											.y = 0.0f,
											.width = static_cast<float>(newFrameImage.width),
											.height = static_cast<float>(newFrameImage.height)},
								  Rectangle{.x = static_cast<float>(repackedRect.x),
											.y = static_cast<float>(repackedRect.y),
											.width = static_cast<float>(repackedRect.width),
											.height = static_cast<float>(repackedRect.height)},
								  Vector2Zero(), 0.0f, WHITE);

				packedFrames.push_back(newFrameData);

				croppedWidth = std::max(croppedWidth, repackedRect.x + repackedRect.width);
				croppedHeight = std::max(croppedHeight, repackedRect.y + repackedRect.height);

				i++;
			}


			rawFrames.push_back(newFrameImage);
			outputFrames.push_back(newFrameData);
		}

		for (const auto frame: rawFrames) {
			UnloadImage(frame);
		}

		if (crop) {
			ImageCrop(&dst, Rectangle{.x = 0.0f,
									  .y = 0.0f,
									  .width = static_cast<float>(croppedWidth),
									  .height = static_cast<float>(croppedHeight)});
		}

		return outputFrames;
	}
} // namespace utp::utils
