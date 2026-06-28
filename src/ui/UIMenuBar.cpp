#include "UIMenuBar.hpp"

namespace utp::ui {
	UIMenuBar::UIMenuBar(const ImVec2 pos) : UIObject(pos,  " ") {}

	bool UIMenuBar::begin() {
		return ImGui::BeginMenuBar();
	}

	void UIMenuBar::end() {
		ImGui::EndMenuBar();
	}
} // namespace utp::ui
