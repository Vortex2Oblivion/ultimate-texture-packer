#pragma once

#include "eventpp/callbacklist.h"
#include "raylib.h"

namespace utp::ui {
	class RenderArea {
	public:
		explicit RenderArea(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
		~RenderArea();

		bool canDrag = true;

		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;

		eventpp::CallbackList<void()> onDraw;
		eventpp::CallbackList<void()> onMouseDown;
		eventpp::CallbackList<void()> onMouseUp;


		void draw();
		void resetCamPos();

	protected:
		Camera2D cam{};
		bool dragging = false;
	};
} // namespace utp::ui
