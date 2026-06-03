#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "ui/ImageScrollList.hpp"
#include "ui/SpritesheetLoadMenu.hpp"
#include "utils/FileUtil.hpp"
#include "utils/Repacker.hpp"

int main() {
	constexpr int screenWidth = 1280;
	constexpr int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "Ultimate Texture Packer");

	NFD_Init();

	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));


	auto scroll = utp::ui::ImageScrollList(screenWidth - 200.0f, 150.0f, 200.0, screenHeight - 150.0f);

	auto loadMenu = utp::ui::SpritesheetLoadMenu(0.0f, 0.0f, 650.0f, 450.0f);
	loadMenu.screenCenter();

	auto loadFilesButton = utp::ui::Button(10, 10, 48, 48, "", "Load Files", BLACK);
	loadFilesButton.icon = ICON_FILE_ADD;

	loadFilesButton.onPress.append([&loadMenu, &loadFilesButton] {
		loadMenu.open = true;
		loadFilesButton.disabled = true;
	});

	loadMenu.onClose.append([&loadFilesButton] {
		loadFilesButton.disabled = false;
	});

	loadMenu.repack.onPress.append([&scroll, &loadMenu] {
		Image dst;
		const Image src = LoadImageFromTexture(loadMenu.selectedSpritesheetPreview);

		utp::utils::Repacker::repack(dst, src, 8192, 8192, loadMenu.frames, false);

		scroll.textures.push_back( LoadTextureFromImage(dst));

		UnloadImage(dst);
		UnloadImage(src);

		loadMenu.open = false;
		loadMenu.onClose();
	});

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);

		loadFilesButton.draw();

		scroll.draw();
		loadMenu.draw();
		EndDrawing();
	}

	CloseWindow();
	NFD_Quit();

	return 0;
}