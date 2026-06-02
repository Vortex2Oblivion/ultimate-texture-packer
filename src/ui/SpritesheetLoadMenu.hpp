#pragma once

#include <vector>

#include "Button.hpp"
#include "data/Frame.hpp"
#include "eventpp/callbacklist.h"

namespace utp::ui {
	class SpritesheetLoadMenu {
	public:
		SpritesheetLoadMenu(float x, float y, float width, float height);
		~SpritesheetLoadMenu();

		bool open = false;
		bool disabled = false;

		float x;
		float y;
		float width;
		float height;

		eventpp::CallbackList<void()> onClose;

		Button loadSpritesheet;
		Button loadXML;
		Button repack;


		static float padding;

		void draw();
		void screenCenter();

	protected:
		Texture selectedSpritesheetPreview{};
		Image spritesheetImage{};

		std::vector<Rectangle> rectsToDraw = {};
		std::vector<data::Frame> frames = {};

		Camera2D camPreview{};
	};
} // namespace utp::ui
