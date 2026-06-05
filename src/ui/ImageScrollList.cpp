#include "ImageScrollList.hpp"

#include <iostream>
#include "raymath.h"

namespace utp::ui {
	ImageScrollList::ImageScrollList(const float x, const float y, const float width, const float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	ImageScrollList::ImageScrollList(const Rectangle bounds) : ImageScrollList(bounds.x, bounds.y, bounds.width, bounds.height) {}

	ImageScrollList::~ImageScrollList() = default;

	void ImageScrollList::draw() {
		framePassed = true;
		constexpr float thickness = 1.0f;
		const auto backgroundRect = Rectangle{.x = x, .y = y, .width = width, .height = height};

		if (CheckCollisionPointRec(GetMousePosition(), backgroundRect)) {
			scrollY -= GetMouseWheelMove() * height / steps;
		}

		DrawRectangleRec(Rectangle{.x = x, .y = 0.0f, .width = width, .height = static_cast<float>(GetRenderHeight())}, DARKGRAY);

		BeginScissorMode(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));

		float drawY = scrollY;
		for (size_t i = 0; i < textures.size(); i++) {
			const auto texture = textures[i];

			const float scale = width / std::fmaxf(static_cast<float>(texture.height), static_cast<float>(texture.width));


			const auto scaledWidth = static_cast<float>(texture.width) * scale;
			const auto scaledHeight = static_cast<float>(texture.height) * scale;

			const auto hitbox = Rectangle{.x = x, .y = y + drawY, .width = width, .height = scaledHeight};

			DrawRectangleRec(hitbox, i % 2 == 0 ? LIGHTGRAY : GRAY);

			DrawTexturePro(texture,
						   Rectangle{.x = 0.0f,
									 .y = 0.0f,
									 .width = static_cast<float>(texture.width),
									 .height = static_cast<float>(texture.height)},
						   Rectangle{.x = x, .y = y + drawY, .width = scaledWidth, .height = scaledHeight}, Vector2Zero(), 0.0f,
						   WHITE);

			if (CheckCollisionPointRec(GetMousePosition(), hitbox)) {
				DrawRectangleRec(hitbox, ColorAlpha(WHITE, sin(static_cast<float>(GetTime()) * 2.0f) / 3.0f + 1.0f / 3.0f));
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					currentTexture = texture;
					if (!clickTimerRunning) {
						clickTimerRunning = true;
						framePassed = false;
					}
				}
			}

			drawY += scaledHeight;
		}
		EndScissorMode();


		const bool heightBigger = currentTexture.height > currentTexture.width;

		float scale;

		if (heightBigger) {
			scale = (static_cast<float>(GetRenderHeight()) - height) / static_cast<float>(currentTexture.height);
		} else {
			scale = width / static_cast<float>(currentTexture.width);
		}

		DrawTexturePro(currentTexture,
					   Rectangle{.x = 0.0f,
								 .y = 0.0f,
								 .width = static_cast<float>(currentTexture.width),
								 .height = static_cast<float>(currentTexture.height)},
					   Rectangle{.x = x,
								 .y = 0.0f,
								 .width = static_cast<float>(currentTexture.width) * scale,
								 .height = static_cast<float>(currentTexture.height) * scale},
					   Vector2Zero(), 0.0f, WHITE);
		DrawRectangleLinesEx(backgroundRect, thickness, BLACK);
		DrawRectangleLinesEx(Rectangle{.x = backgroundRect.x,
									   .y = 0.0f,
									   .width = backgroundRect.width,
									   .height = static_cast<float>(GetRenderHeight())},
							 thickness, BLACK);

		const auto resetClickTimer = [this] {
			clickTimer = 0.0f;
			clickTimerRunning = false;
		};

		constexpr float timeToSelect = 0.33f;
		if (clickTimerRunning) {
			clickTimer += GetFrameTime();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && framePassed) {
				onSelect();
				resetClickTimer();
			}
		}

		if (clickTimer >= timeToSelect) {
			resetClickTimer();
		}
	}
} // namespace utp::ui
