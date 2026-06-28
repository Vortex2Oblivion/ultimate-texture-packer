#pragma once
#include "UIObject.hpp"

namespace utp::ui {
	class UIMenuBar : public UIObject {
	public:
		explicit UIMenuBar(ImVec2 pos);

	protected:
		bool begin() override;
		void end() override;
	};
} // namespace utp::ui
