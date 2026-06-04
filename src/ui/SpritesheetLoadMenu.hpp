#pragma once

#include <vector>

#include "Button.hpp"
#include "RenderArea.hpp"
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

		Texture selectedSpritesheetPreview{};

		std::vector<data::Frame> frames = {};

		eventpp::CallbackList<void()> onClose;

		Button loadSpritesheet;
		Button loadXML;
		Button repack;

		RenderArea renderArea;


		static float padding;

		void draw();
		void screenCenter();

	protected:
		std::vector<Rectangle> rectsToDraw = {};

		Camera2D camPreview{};
	};
} // namespace utp::ui
