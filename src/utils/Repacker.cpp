#include "Repacker.hpp"

#include "MaxRectsBinPack.h"

namespace utp::utils {
	void Repacker::repack(Image &dst, const Image &src, const int width, const int height, const std::vector<data::Frame> &frames,
						  const bool allowRotate, const bool crop) {

		dst = GenImageColor(width, height, BLANK);
		auto packer = rbp::MaxRectsBinPack(width, height, allowRotate);

		std::vector<data::Frame> packedFrames = {};

		float croppedWidth = 0.0f;
		float croppedHeight = 0.0f;

		for (const auto &frame: frames) {
			Rectangle packedRect{};
			rbp::Rect _rect{};
			for (const auto &packed: packedFrames) {
				if (frame == packed) {
					goto end;
				}
			}
			_rect = packer.Insert(static_cast<int>(frame.width), static_cast<int>(frame.height),
								  rbp::MaxRectsBinPack::RectBottomLeftRule);

			packedRect = Rectangle{.x = static_cast<float>(_rect.x),
								   .y = static_cast<float>(_rect.y),
								   .width = static_cast<float>(_rect.width),
								   .height = static_cast<float>(_rect.height)};

			if (packedRect.width != 0 && packedRect.height != 0) {

				ImageDraw(&dst, src, Rectangle{.x = frame.x, .y = frame.y, .width = frame.width, .height = frame.height},
						  {.x = packedRect.x, .y = packedRect.y, .width = packedRect.width, .height = packedRect.height}, WHITE);

				packedFrames.push_back(frame);
				croppedWidth = std::max(croppedWidth, packedRect.x + packedRect.width);
				croppedHeight = std::max(croppedHeight, packedRect.y + packedRect.height);
			}
			packedFrames.push_back(frame);
		end:;
		}

		if (crop) {
			ImageCrop(&dst, Rectangle{.x = 0.0f, .y = 0.0f, .width = croppedWidth, .height = croppedHeight});
		}
	}
} // namespace utp::utils
