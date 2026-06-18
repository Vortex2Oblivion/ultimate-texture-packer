#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include <iostream>
#include "pugixml.hpp"
#include "raygui.h"
#include "raymath.h"
#include "ui/ImageScrollList.hpp"
#include "ui/RenderArea.hpp"
#include "ui/SpritesheetLoadMenu.hpp"
#include "utils/FileUtil.hpp"
#include "utils/Memory.hpp"
#include "utils/Repacker.hpp"
#include "utils/StringUtil.hpp"

#if !defined(NDEBUG) || defined(_DEBUG)
#define DEBUG
#endif

int main() {
	constexpr int screenWidth = 1280;
	constexpr int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "Ultimate Texture Packer");

	NFD_Init();

	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	std::vector<utp::data::Frame> packedFrames = {};

	auto scroll = utp::ui::ImageScrollList(screenWidth - 200.0f, 150.0f, 200.0, screenHeight - 150.0f);

	auto loadMenu = utp::ui::SpritesheetLoadMenu(0.0f, 0.0f, 650.0f, 450.0f);
	loadMenu.screenCenter();

	constexpr auto buttonPadding = 4.0f;

	auto loadFilesButton = utp::ui::Button(10, 10, 48, 48, "", "Load Files", BLACK);
	loadFilesButton.icon = ICON_FILE_ADD;

	auto exportButton =
			utp::ui::Button(loadFilesButton.x + loadFilesButton.width + buttonPadding, loadFilesButton.y, 48, 48, "", "Export", BLACK);
	exportButton.icon = ICON_FILE_SAVE;

	auto outputPreview = utp::ui::RenderArea(loadFilesButton.x + loadFilesButton.width, loadFilesButton.y + loadFilesButton.height,
											 static_cast<float>(GetRenderWidth()) - scroll.width, screenHeight);

	loadFilesButton.onPress.append([&loadMenu, &loadFilesButton, &outputPreview] {
		loadMenu.open = true;
		loadFilesButton.disabled = true;
		outputPreview.canDrag = false;
	});

	loadMenu.onClose.append([&loadFilesButton, &outputPreview] {
		loadFilesButton.disabled = false;
		outputPreview.canDrag = true;
	});

	loadMenu.repack.onPress.append([&scroll, &loadMenu, &packedFrames] {
		Image dst;
		const Image src = LoadImageFromTexture(loadMenu.selectedSpritesheetPreview);

		packedFrames = utp::utils::Repacker::repack(dst, src, 8192, 8192, loadMenu.frames, false);

		Texture t = LoadTextureFromImage(dst);
#ifdef false
		GenTextureMipmaps(&t);
		SetTextureFilter(t, TEXTURE_FILTER_TRILINEAR);
#endif
		scroll.textures.push_back(t);

		UnloadImage(dst);
		UnloadImage(src);

		loadMenu.open = false;
		loadMenu.onClose();
	});

	Texture bigPreview{};

	scroll.onSelect.append([&scroll, &bigPreview, &outputPreview] {
		bigPreview = scroll.currentTexture;
		outputPreview.resetCamPos();
	});


	outputPreview.onDraw.append([&bigPreview, &packedFrames] {
		if (IsTextureValid(bigPreview)) {
			DrawTexture(bigPreview, 0, 0, WHITE);
			for (auto &frame: packedFrames) {
				DrawRectangleRec(static_cast<Rectangle>(frame), ColorAlpha(BLUE, 0.1));
#ifdef false
				DrawLine(frame.x, frame.y + frame.height, frame.x + frame.width, frame.y, RED);
#endif
			}
		}
	});

	exportButton.onPress.append([&packedFrames, &bigPreview] {
		// get a test document
		pugi::xml_document doc;
		doc.load_string("");

		// add a custom declaration node
		pugi::xml_node header = doc.prepend_child(pugi::node_declaration);
		header.append_attribute("version") = "1.0";
		header.append_attribute("encoding") = "UTF-8";

		pugi::xml_node textureAtlas = doc.append_child("TextureAtlas");
		textureAtlas.append_attribute("imagePath") = "";

		for (const auto &[x, y, width, height, frameX, frameY, frameWidth, frameHeight, rotated, name]: packedFrames) {
			pugi::xml_node subTexture = doc.child("TextureAtlas").append_child("SubTexture");
			subTexture.append_attribute("name") = name;
			subTexture.append_attribute("x") = x;
			subTexture.append_attribute("y") = y;
			subTexture.append_attribute("width") = width;
			subTexture.append_attribute("height") = height;
			subTexture.append_attribute("frameX") = frameX;
			subTexture.append_attribute("frameY") = frameY;
			subTexture.append_attribute("frameWidth") = frameWidth;
			subTexture.append_attribute("frameHeight") = frameHeight;
			subTexture.append_attribute("rotated") = rotated;
		}


		if (!doc.save_file("output.xml")) {
			TraceLog(LOG_ERROR, "Failed to save output.xml");
		}
		const auto img = LoadImageFromTexture(bigPreview);
		auto t = std::thread([img] {
			ExportImage(img, "output.png");
			UnloadImage(img);
		});
		t.detach();
	});


	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);

		outputPreview.draw();

		loadFilesButton.draw();
		exportButton.draw();

		scroll.draw();
		loadMenu.draw();

#ifdef DEBUG
		DrawFPS(0, 0);
		DrawText(utp::utils::StringUtil::formatBytes(utp::utils::Memory::getCurrentRSS()).c_str(), 0, 20, 20, LIME);
#endif
		EndDrawing();
	}

	CloseWindow();
	NFD_Quit();

	return 0;
}
