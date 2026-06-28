#include "UIMenuItem.hpp"

namespace utp::ui {
	UIMenuItem::UIMenuItem(const std::string &name) : UIObject(ImVec2(), name) {}

	bool UIMenuItem::begin() { return ImGui::MenuItem(name.c_str(), nullptr, &pressed); }

	void UIMenuItem::end() {
		if (pressed) {
			onPress();
			if (mode == UIMenuItemMode::TOGGLE) {
				pressed = false;
			}
		}
	}
} // namespace utp::ui
