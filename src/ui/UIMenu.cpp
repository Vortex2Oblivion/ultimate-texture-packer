
#include "UIMenu.hpp"

namespace utp::ui {
	UIMenu::UIMenu(const std::string &name) : UIObject(ImVec2(), name) {}

	bool UIMenu::begin() { return ImGui::BeginMenu(name.c_str(), enabled); }

	void UIMenu::end() { ImGui::EndMenu(); }
} // namespace utp::ui
