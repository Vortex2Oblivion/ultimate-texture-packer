#include "RenderArea.hpp"


#include "raygui.h"
#include "raylib.h"
#include "raymath.h"

namespace utp::ui {
	RenderArea::RenderArea(const float x, const float y, const float width, const float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;

		cam = {.offset = Vector2Zero(), .target = Vector2Zero(), .rotation = 0.0f, .zoom = 1.0f};
	}

	RenderArea::~RenderArea() = default;

	void RenderArea::draw() {
		BeginScissorMode(std::floor(x), std::floor(y), std::floor(width), std::floor(height));

		BeginMode2D(cam);

		const auto area = Rectangle{.x = x, .y = y, .width = width, .height = height};

		if (CheckCollisionPointRec(GetMousePosition(), area)) {
			const float wheel = GetMouseWheelMove();
			if (wheel != 0.0f) {
				constexpr float maxZoom = 3.0f;
				constexpr float minZoom = 0.1f;

				const Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);
				cam.offset = GetMousePosition();
				cam.target = mouseWorldPos;

				const float scale = 0.1f * wheel;

				cam.zoom = Clamp(expf(logf(cam.zoom) + scale), minZoom, maxZoom);
			}
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), area)) {
				cam.target -= GetMouseDelta() / cam.zoom;
			}
		}

		onDraw();

		EndMode2D();

		EndScissorMode();

		DrawRectangleLinesEx(area, static_cast<float>(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), BLACK);
	}
} // namespace utp::ui
