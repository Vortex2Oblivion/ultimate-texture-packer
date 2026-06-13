#pragma once

#include <string>

#include "Rect.h"
#include "raylib.h"

namespace utp::data {
	struct Frame {
		mutable float x;
		mutable float y;
		mutable float width;
		mutable float height;
		mutable float frameX;
		mutable float frameY;
		mutable float frameWidth;
		mutable float frameHeight;
		mutable bool rotated;
		mutable std::string name;

		bool operator==(const Frame &frame) const {
			return frame.x == this->x && frame.y == this->y && frame.width == this->width && frame.height == this->height;
		}

		explicit operator Rectangle() const { return Rectangle{.x = x, .y = y, .width = width, .height = height}; }
		explicit operator rbp::Rect() const {
			return rbp::Rect{.x = static_cast<int>(x),
							 .y = static_cast<int>(y),
							 .width = static_cast<int>(width),
							 .height = static_cast<int>(height)};
		}
	};
} // namespace utp::data
