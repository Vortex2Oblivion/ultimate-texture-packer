#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"
#include "ui/ImageScrollList.hpp"
#include "ui/SpritesheetLoadMenu.hpp"
#include "utils/FileUtil.hpp"

int main() {
	constexpr int screenWidth = 1280;
	constexpr int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "Ultimate Texture Packer");
	NFD_Init();

	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	auto scroll = utp::ui::ImageScrollList(
			Rectangle{.x = screenWidth - 200.0f, .y = 150.0f, .width = 200.0, .height = screenHeight - 150.0f});

	auto loadMenu = utp::ui::SpritesheetLoadMenu(0.0f, 0.0f, 650.0f, 450.0f);
	loadMenu.screenCenter();

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);
		if (IsKeyDown(KEY_SPACE)) {
			loadMenu.open = true;
		}
		scroll.draw();
		loadMenu.draw();
		DrawFPS(0, 0);
		EndDrawing();
	}

	CloseWindow();
	NFD_Quit();

	return 0;
}
