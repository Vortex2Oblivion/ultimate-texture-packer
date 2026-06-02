#include "SpritesheetLoadMenu.hpp"

#include <iostream>

#include "pugixml.hpp"
#include "raygui.h"
#include "raylib.h"
#include "raymath.h"
#include "utils/FileUtil.hpp"

namespace utp::ui {
	float SpritesheetLoadMenu::padding = 5;

	SpritesheetLoadMenu::SpritesheetLoadMenu(const float x, const float y, const float width, const float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;

		const auto buttonWidth = width / 3 - padding * 3;
		constexpr auto buttonHeight = 48.0f;

		this->loadSpritesheet = Button(10, 10, buttonWidth, buttonHeight, "Load Spritesheet", "", BLACK);
		this->loadXML = Button(10, 10, buttonWidth, buttonHeight, "Load XML", "", BLACK);
		this->repack = Button(10, 10, buttonWidth, buttonHeight, "Repack", "Repack the spritesheet.", BLACK);

		loadSpritesheet.onPress.append([this] {
			auto [outPath, result] = utils::FileUtil::openFileDialog({{"Image file", "png"}});

			if (result == NFD_OKAY) {
				loadSpritesheet.tooltip = outPath;
				UnloadTexture(selectedSpritesheetPreview);
				selectedSpritesheetPreview = LoadTexture(outPath.c_str());
			} else {
				TraceLog(LOG_ERROR, NFD_GetError());
			}
		});

		loadXML.onPress.append([this, x, y] {
			auto [outPath, result] = utils::FileUtil::openFileDialog({{"XML file", "xml"}});

			if (result == NFD_OKAY) {
				loadXML.tooltip = outPath;
				rectsToDraw.clear();
				pugi::xml_document doc;
				doc.load_file(outPath.c_str());

				for (auto frame: doc.child("TextureAtlas").children("SubTexture")) {
					rectsToDraw.push_back(Rectangle{.x = frame.attribute("x").as_float() + static_cast<float>(x) + this->x,
													.y = frame.attribute("y").as_float() + static_cast<float>(y) + this->y,
													.width = frame.attribute("width").as_float(),
													.height = frame.attribute("height").as_float()});
					frames.push_back(data::Frame{.x = frame.attribute("x").as_float(),
												 .y = frame.attribute("y").as_float(),
												 .width = frame.attribute("width").as_float(),
												 .height = frame.attribute("height").as_float(),
												 .frameX = frame.attribute("frameX").as_float(),
												 .frameY = frame.attribute("frameY").as_float(),
												 .frameWidth = frame.attribute("frameWidth").as_float(),
												 .frameHeight = frame.attribute("frameHeight").as_float(),
												 .rotated = frame.attribute("rotated").as_bool(),
												 .name = frame.attribute("name").as_string()});
				}
			} else {
				TraceLog(LOG_ERROR, NFD_GetError());
			}
		});

		const auto pos = Vector2{.x = static_cast<float>(x), .y = static_cast<float>(y)};

		this->camPreview = Camera2D{.offset = pos, .target = pos, .rotation = 0.0, .zoom = 1.0};
	}

	SpritesheetLoadMenu::~SpritesheetLoadMenu() = default;

	void SpritesheetLoadMenu::screenCenter() {
		x = (static_cast<float>(GetRenderWidth()) - width) / 2.0f;
		y = (static_cast<float>(GetRenderHeight()) - height) / 2.0f;
		camPreview.offset = camPreview.target =
				Vector2{.x = static_cast<float>(GetRenderWidth()) / 2.0f, .y = static_cast<float>(GetRenderHeight()) / 2.0f};
	}

	void SpritesheetLoadMenu::draw() {
		if (!open) {
			return;
		}

		if (disabled) {
			GuiDisable();
		}

		const auto position = Rectangle{.x = x, .y = y, .width = this->width, .height = this->height};

		open = !GuiWindowBox(position, "Select files");
		if (!open) {
			onClose();
			return;
		}

		constexpr float statusbarHeight = 24.0f;
		const auto clipRect = Rectangle{.x = x + padding,
										.y = y + statusbarHeight + loadSpritesheet.height + padding * 2,
										.width = this->width - padding * 2,
										.height = this->height - statusbarHeight - loadSpritesheet.height - padding * 3};

		const bool canDrag = IsTextureValid(selectedSpritesheetPreview);

		if (canDrag) {

			const float wheel = GetMouseWheelMove();

			// stolen from raylib demo lmao
			// https://www.raylib.com/examples/core/loader.html?name=core_2d_camera_mouse_zoom
			if (wheel != 0.0f) {
				constexpr float maxZoom = 3.0f;
				constexpr float minZoom = 0.1f;

				const Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camPreview);
				camPreview.offset = GetMousePosition();
				camPreview.target = mouseWorldPos;

				const float scale = 0.1f * wheel;

				camPreview.zoom = Clamp(expf(logf(camPreview.zoom) + scale), minZoom, maxZoom);
			}


			BeginScissorMode(static_cast<int>(clipRect.x), static_cast<int>(clipRect.y), static_cast<int>(clipRect.width),
							 static_cast<int>(clipRect.height));

			BeginMode2D(camPreview);

			DrawTexture(selectedSpritesheetPreview, static_cast<int>(x), static_cast<int>(y), WHITE);

			for (const auto rect: rectsToDraw) {
				DrawRectanglePro(rect, Vector2Zero(), 0.0f, ColorAlpha(BLUE, 0.1));
			}

			EndMode2D();

			DrawRectangleLinesEx(clipRect, static_cast<float>(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), BLACK);

			EndScissorMode();
		}

		loadSpritesheet.x = x + padding * 3.0f + (this->width - x * 2.0f) / 3.0f;
		loadSpritesheet.y = position.y + statusbarHeight + padding;
		loadSpritesheet.draw();

		loadXML.x = loadSpritesheet.x + loadSpritesheet.width + padding;
		loadXML.y = loadSpritesheet.y;
		loadXML.draw();

		repack.x = loadXML.x + loadXML.width + padding;
		repack.y = loadXML.y;
		repack.draw();

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (CheckCollisionPointRec(GetMousePosition(), clipRect) || disabled) && canDrag) {
			camPreview.target -= GetMouseDelta() / camPreview.zoom;
			disabled = loadSpritesheet.disabled = loadXML.disabled = repack.disabled = true;
		} else {
			disabled = loadSpritesheet.disabled = loadXML.disabled = repack.disabled = false;
		}
		GuiEnable();
	}
} // namespace utp::ui
