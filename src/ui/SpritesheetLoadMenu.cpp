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

		loadSpritesheet = Button(10, 10, buttonWidth, buttonHeight, "Load Spritesheet", "", BLACK);
		loadXML = Button(10, 10, buttonWidth, buttonHeight, "Load XML", "", BLACK);
		repack = Button(10, 10, buttonWidth, buttonHeight, "Repack", "Repack the spritesheet.", BLACK);

		renderArea = RenderArea();

		loadSpritesheet.onPress.append([this] {
			auto [outPath, result] = utils::FileUtil::openFileDialog({{"Image file", "png"}});

			if (result == NFD_ERROR) {
				TraceLog(LOG_ERROR, NFD_GetError());
				return;
			}
			loadSpritesheet.tooltip = outPath;
			UnloadTexture(selectedSpritesheetPreview);
			selectedSpritesheetPreview = LoadTexture(outPath.c_str());
		});

		loadXML.onPress.append([this, x, y] {
			auto [outPath, result] = utils::FileUtil::openFileDialog({{"XML file", "xml"}});

			if (result == NFD_ERROR) {
				TraceLog(LOG_ERROR, NFD_GetError());
				return;
			}

			loadXML.tooltip = outPath;
			rectsToDraw.clear();
			pugi::xml_document doc;
			doc.load_file(outPath.c_str());

			for (auto frame: doc.child("TextureAtlas").children("SubTexture")) {
				rectsToDraw.push_back(Rectangle{.x = frame.attribute("x").as_float() + static_cast<float>(x),
												.y = frame.attribute("y").as_float() + static_cast<float>(y),
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
		});

		onClose.append([this] {
			UnloadTexture(selectedSpritesheetPreview);
			selectedSpritesheetPreview = {};
			frames.clear();
			rectsToDraw.clear();
		});

		renderArea.onDraw.append([this, x, y] {
			DrawTexture(selectedSpritesheetPreview, static_cast<int>(x), static_cast<int>(y), WHITE);

			for (const auto rect: rectsToDraw) {
				DrawRectanglePro(rect, Vector2Zero(), 0.0f, ColorAlpha(BLUE, 0.1));
			}
		});
	}

	SpritesheetLoadMenu::~SpritesheetLoadMenu() = default;

	void SpritesheetLoadMenu::screenCenter() {
		x = (static_cast<float>(GetRenderWidth()) - width) / 2.0f;
		y = (static_cast<float>(GetRenderHeight()) - height) / 2.0f;
	}

	void SpritesheetLoadMenu::draw() {
		if (!open) {
			return;
		}

		DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), ColorAlpha(BLACK, 0.5f));

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


		renderArea.x = x + padding;
		renderArea.y = y + statusbarHeight + loadSpritesheet.height + padding * 2;
		renderArea.width = this->width - padding * 2;
		renderArea.height = this->height - statusbarHeight - loadSpritesheet.height - padding * 3;

		renderArea.draw();


		loadSpritesheet.x = x + padding * 3.0f + (this->width - x * 2.0f) / 3.0f;
		loadSpritesheet.y = position.y + statusbarHeight + padding;
		loadSpritesheet.draw();

		loadXML.x = loadSpritesheet.x + loadSpritesheet.width + padding;
		loadXML.y = loadSpritesheet.y;
		loadXML.draw();

		repack.x = loadXML.x + loadXML.width + padding;
		repack.y = loadXML.y;
		repack.draw();
	}
} // namespace utp::ui
