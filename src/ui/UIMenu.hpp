#pragma once

#include "UIObject.hpp"

namespace utp::ui {
	class UIMenu : public UIObject {
	public:
		explicit UIMenu(const std::string &name);
		bool enabled = true;

		UTP_BEGIN_END
	};
} // namespace utp::ui
