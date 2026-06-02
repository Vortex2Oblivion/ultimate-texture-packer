#pragma once

#include <string>
#include "eventpp/callbacklist.h"
#include "raylib.h"

namespace utp::ui {
	class Button {
	public:
		explicit Button(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f, const std::string &text = "",
						const std::string &tooltip = "", Color color = BLANK);
		~Button();
		void draw();

		float x;
		float y;
		float width;
		float height;
		Color color = BLACK;
		std::string tooltip;
		std::string text;
		int icon = -1;

		bool pressed = false;
		bool disabled = false;

		eventpp::CallbackList<void()> onPress;

	private:
		static bool drawTooltip;
	};
} // namespace utp::ui
