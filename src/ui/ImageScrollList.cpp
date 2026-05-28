#include "ImageScrollList.hpp"

#include "raymath.h"

namespace utp::ui {
	ImageScrollList::ImageScrollList(const float x, const float y, const float width, const float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	ImageScrollList::~ImageScrollList() {
		for (const auto texture: textures) {
			UnloadTexture(texture);
		}
	};

	void ImageScrollList::draw() const {
		BeginScissorMode(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
		for (const auto texture: this->textures) {
			DrawTexturePro(
				texture,
				Rectangle{
					.x = 0.0f, .y = 0.0f, .width = static_cast<float>(texture.width),
					.height = static_cast<float>(texture.height)
				},
				Rectangle{
					.x = x, .y = y, .width = static_cast<float>(texture.width),
					.height = static_cast<float>(texture.height)
				},
				Vector2Zero(),
				0.0f,
				WHITE
			);
		}
		EndScissorMode();
	}
}
