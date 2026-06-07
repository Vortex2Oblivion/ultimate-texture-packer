#pragma once

#include <string>
#include "raylib.h"

namespace utp::data {
	struct Frame {
		float x;
		float y;
		float width;
		float height;
		float frameX;
		float frameY;
		float frameWidth;
		float frameHeight;
		bool rotated;
		std::string name;

		bool operator==(const Frame &frame) const {
			return frame.x == this->x && frame.y == this->y && frame.width == this->width && frame.height == this->height;
		}

		operator Rectangle() const { return Rectangle{.x = x, .y = y, .width = width, .height = height}; }
	};
} // namespace utp::data
