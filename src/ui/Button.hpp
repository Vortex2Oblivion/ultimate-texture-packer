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

		bool pressed = false;
		bool disabled = false;

		int icon = -1;

		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;

		Color color = BLACK;
		std::string tooltip;
		std::string text;

		eventpp::CallbackList<void()> onPress;

		static bool globalDisabled;

	private:
		static bool drawTooltip;
	};
} // namespace utp::ui
