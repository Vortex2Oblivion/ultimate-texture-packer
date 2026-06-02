#pragma once

#include <memory>
#include <vector>

#include "Button.hpp"
#include "data/Frame.hpp"

namespace utp::ui {
	class SpritesheetLoadMenu {
	public:
		SpritesheetLoadMenu(float x, float y, float width, float height);
		~SpritesheetLoadMenu();

		bool open = false;

		float x;
		float y;
		float width;
		float height;

		Button loadSpritesheet;
		Button loadXML;
		Button repack;

		static float padding;

		void draw();
		void screenCenter();

	protected:
		Image spritesheetImage{};
		Texture selectedSpritesheetPreview{};

		std::vector<Rectangle> rectsToDraw = {};
		std::vector<data::Frame> frames = {};

		Camera2D camPreview{};
	};
} // namespace utp::ui
