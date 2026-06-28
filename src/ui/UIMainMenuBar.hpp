#pragma once

#include "UIObject.hpp"

namespace utp::ui {
	class UIMainMenuBar : public UIObject {
	public:
		explicit UIMainMenuBar();

	protected:
		bool begin() override;
		void end() override;
	};
} // namespace utp::ui
