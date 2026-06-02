#include "Button.hpp"

#include "raygui.h"
namespace utp::ui {

	bool Button::drawTooltip = false;

	Button::Button(const float x, const float y, const float width, const float height, const std::string &text,
				   const std::string &tooltip, const Color color) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->text = text;
		this->tooltip = tooltip;
		this->color = color;
		if (!tooltip.empty()) {
			GuiEnableTooltip();
			GuiSetTooltip(this->tooltip.c_str());
		}
	}

	Button::~Button() = default;


	void Button::draw() {
		const auto hitbox = Rectangle{.x = x, .y = y, .width = width, .height = height};
		pressed = GuiButton(hitbox, text.c_str());
		if (pressed) {
			onPress();
			pressed = false;
		}
		if (icon != -1) {
			GuiDrawIcon(icon, static_cast<int>(x), static_cast<int>(y), static_cast<int>(width) / 16, color);
		}
		drawTooltip = CheckCollisionPointRec(GetMousePosition(), hitbox) && !tooltip.empty();
		if (drawTooltip) {
			GuiEnableTooltip();
			GuiSetTooltip(tooltip.c_str());
		}
		else {
			GuiDisableTooltip();
		}
	}
} // namespace utp::ui
