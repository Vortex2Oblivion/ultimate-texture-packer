#pragma once

#include <vector>

#include "raylib.h"

namespace utp::ui {
	class ImageScrollList {
	public:
		ImageScrollList(float x, float y, float width, float height);

		~ImageScrollList();

		float x, y, width, height = 0.0f;

		void draw() const;

	protected:
		std::vector<Texture> textures = {};
	};
}
