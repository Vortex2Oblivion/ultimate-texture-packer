#pragma once

#include "eventpp/callbacklist.h"
#include "raylib.h"

namespace utp::ui {
	class RenderArea {
	public:
		RenderArea(float x, float y, float width, float height);
		~RenderArea();

		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;

		eventpp::CallbackList<void()> onDraw;

		void draw();

	protected:
		Camera2D cam{};
		bool dragging = false;
	};
} // namespace utp::ui
