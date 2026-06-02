#pragma once

#include <vector>

#include "raylib.h"
#include "eventpp/callbacklist.h"

namespace utp::ui {
	class ImageScrollList {
	public:
		ImageScrollList(float x, float y, float width, float height);
		explicit ImageScrollList(Rectangle bounds);

		~ImageScrollList();

		float x, y, width, height = 0.0f;
		float steps = 20.0f;

		Texture currentTexture = {};
		std::vector<Texture> textures = {};

		eventpp::CallbackList<void()> onSelect;

		void draw();

	protected:
		float scrollY = 0.0f;
		float clickTimer = 0.0f;
		bool clickTimerRunning = false;
	};
}
