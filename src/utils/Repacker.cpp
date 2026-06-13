#include "Repacker.hpp"

#include "ImageUtil.hpp"
#include "MaxRectsBinPack.h"
#include "raymath.h"

namespace utp::utils {
	std::vector<data::Frame> Repacker::repack(Image &dst, const Image &src, const int width, const int height,
											  const std::vector<data::Frame> &frames, const bool allowRotate, const bool crop) {
		dst = GenImageColor(width, height, BLANK);
		auto packer = rbp::MaxRectsBinPack(width, height, allowRotate);

		std::vector<data::Frame> packedFrames = {};
		std::vector<data::Frame> outputFrames = {};

		float croppedWidth = 0.0f;
		float croppedHeight = 0.0f;

		// TODO: rewrite this to properly pack duplicated frames
		for (const auto &frame: frames) {
			Rectangle packedRect{};
			rbp::Rect _rect{};

			bool wasRotated;

			Image imageCropped{};

			for (const auto &packed: packedFrames) {
				if (frame == packed) {
					goto end;
				}
			}
			_rect = packer.Insert(static_cast<int>(frame.width), static_cast<int>(frame.height),
								  rbp::MaxRectsBinPack::RectBottomLeftRule);

			wasRotated =
					static_cast<float>(_rect.width) == frame.height && static_cast<float>(_rect.height) == frame.width && allowRotate;

			packedRect = Rectangle{.x = static_cast<float>(_rect.x),
								   .y = static_cast<float>(_rect.y),
								   .width = static_cast<float>(_rect.width),
								   .height = static_cast<float>(_rect.height)};

			imageCropped = ImageUtil::crop(src, static_cast<Rectangle>(frame));

			if (wasRotated) {
				ImageRotateCW(&imageCropped);
			}

			if (packedRect.width != 0 && packedRect.height != 0) {

				ImageDrawImagePro(&dst, imageCropped,
								  Rectangle{.x = 0.0f,
											.y = 0.0f,
											.width = static_cast<float>(imageCropped.width),
											.height = static_cast<float>(imageCropped.height)},
						  Rectangle{.x = packedRect.x, .y = packedRect.y, .width = packedRect.width, .height = packedRect.height},
						  Vector2Zero(), 0.0f, WHITE);

				packedFrames.push_back(frame);
				croppedWidth = std::max(croppedWidth, packedRect.x + packedRect.width);
				croppedHeight = std::max(croppedHeight, packedRect.y + packedRect.height);
			}

			UnloadImage(imageCropped);

			outputFrames.push_back(data::Frame{.x = packedRect.x,
											   .y = packedRect.y,
											   .width = packedRect.width,
											   .height = packedRect.height,
											   .frameX = frame.frameX,
											   .frameY = frame.frameY,
											   .frameWidth = frame.frameWidth,
											   .frameHeight = frame.frameHeight,
											   .rotated = wasRotated,
											   .name = frame.name});
		end:;
		}

		if (crop) {
			ImageCrop(&dst, Rectangle{.x = 0.0f, .y = 0.0f, .width = croppedWidth, .height = croppedHeight});
		}

		return outputFrames;
	}
} // namespace utp::utils
