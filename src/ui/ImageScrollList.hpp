#pragma once

#include <vector>

#include "raylib.h"

namespace utp::ui {
	class ImageScrollList {
	public:
		ImageScrollList(float x, float y, float width, float height);
		ImageScrollList(Rectangle bounds);

		~ImageScrollList();

		float x, y, width, height = 0.0f;
		float steps = 20.0f;

		Texture currentTexture = {};
		std::vector<Texture> textures = {};

		void draw();

	protected:
		float scrollY = 0.0f;
	};
}
