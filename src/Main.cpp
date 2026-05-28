#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"
#include "ui/ImageScrollList.hpp"
#include "utils/FileUtil.hpp"

int main() {
	constexpr int screenWidth = 1280;
	constexpr int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "Ultimate Texture Packer");
	NFD_Init();

	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	auto scroll = utp::ui::ImageScrollList(
			Rectangle{.x = screenWidth - 200.0f, .y = 150.0f, .width = 200.0, .height = screenHeight - 150.0f});

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);
		scroll.draw();
		DrawTexturePro(scroll.currentTexture,
					   Rectangle{.x = 0.0f,
								 .y = 0.0f,
								 .width = static_cast<float>(scroll.currentTexture.width),
								 .height = static_cast<float>(scroll.currentTexture.height)},
					   Rectangle{.x = scroll.x,
								 .y = 0.0f,
								 .width = scroll.width,
								 .height = static_cast<float>(scroll.currentTexture.height) * scroll.width /
										   static_cast<float>(scroll.currentTexture.width)},
					   Vector2Zero(), 0.0f, WHITE);
		if (IsKeyDown(KEY_SPACE)) {
			auto [outPath, result] = utp::utils::FileUtil::openFileDialog({{"Image file", "png"}});
			if (result == NFD_OKAY) {
				auto texture = LoadTexture(outPath.c_str());
				GenTextureMipmaps(&texture);
				SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
				scroll.textures.push_back(texture);
			} else {
				TraceLog(LOG_ERROR, NFD_GetError());
			}
		}
		EndDrawing();
	}

	CloseWindow();
	NFD_Quit();

	return 0;
}
