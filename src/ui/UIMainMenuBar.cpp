#include "UIMainMenuBar.hpp"

namespace utp::ui {
	UIMainMenuBar::UIMainMenuBar() : UIObject(ImVec2(), " ") {}

	bool UIMainMenuBar::begin() {
		return ImGui::BeginMainMenuBar();
	}

	void UIMainMenuBar::end() {
		ImGui::EndMainMenuBar();
	}
} // namespace utp::ui
