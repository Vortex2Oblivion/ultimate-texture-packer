#pragma once

#include <string>

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
	};
} // namespace utp::data
