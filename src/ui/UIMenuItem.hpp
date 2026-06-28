
#pragma once
#include "UIObject.hpp"
#include "eventpp/callbacklist.h"

namespace utp::ui {
	enum class UIMenuItemMode : int { BUTTON, TOGGLE };
	class UIMenuItem : public UIObject {
	public:
		explicit UIMenuItem(const std::string &name);
		bool enabled = true;

	protected:
		bool pressed = false;
		bool begin() override;
		void end() override;

	public:
		UIMenuItemMode mode = UIMenuItemMode::TOGGLE;
		eventpp::CallbackList<void()> onPress;
	};
} // namespace utp::ui
