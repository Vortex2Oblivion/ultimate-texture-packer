#include "UIObject.hpp"

#include <iostream>

namespace utp::ui {
	UIObject::UIObject(const ImVec2 position, const std::string &name, const ImGuiWindowFlags flags) {
		this->position = position;
		this->name = name;
		this->flags = flags;
	};

	UIObject::~UIObject() = default;

	bool UIObject::begin() {
		ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
		return ImGui::Begin(name.c_str(), &open, flags);
	}

	void UIObject::end() { ImGui::End(); }

	void UIObject::update(const float delta) {
		for (const auto &child: children) {
			child->update(delta);
		}
	}

	void UIObject::draw(const float offsetX, const float offsetY) {
		if (!open) {
			return;
		}
		if (begin()) {
			for (const auto &child: children) {
				child->draw(offsetX, offsetY);
			}
			end();
		}
	}
} // namespace utp::ui
