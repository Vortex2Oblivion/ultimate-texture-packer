
#pragma once
#include "UIObject.hpp"

namespace utp::ui {
	class UIMenu : public UIObject {
	public:
		explicit UIMenu(const std::string &name);
		bool enabled = true;

	protected:
		bool begin() override;
		void end() override;
	};
} // namespace utp::ui
