#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"
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

	loadMenu.onClose.append([&loadFilesButton] { loadFilesButton.disabled = false; });

	loadMenu.repack.onPress.append([&scroll, &loadMenu] {
		Image dst;
		const Image src = LoadImageFromTexture(loadMenu.selectedSpritesheetPreview);

		utp::utils::Repacker::repack(dst, src, 8192, 8192, loadMenu.frames, false);

		Texture t = LoadTextureFromImage(dst);
		GenTextureMipmaps(&t);
		SetTextureFilter(t, TEXTURE_FILTER_TRILINEAR);
		scroll.textures.push_back(t);

		UnloadImage(dst);
		UnloadImage(src);

		loadMenu.open = false;
		loadMenu.onClose();
	});

	Texture bigPreview{};

	scroll.onSelect.append([&scroll, &bigPreview] { bigPreview = scroll.currentTexture; });

	Rectangle canvas = {.x = loadFilesButton.x + loadFilesButton.width,
						.y = loadFilesButton.y + loadFilesButton.height,
						.width = screenWidth,
						.height = screenHeight};

	Camera2D camPreview = {.offset = Vector2Zero(), .target = Vector2Zero(), .rotation = 0.0f, .zoom = 1.0f};

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);

		BeginScissorMode(static_cast<int>(canvas.x), static_cast<int>(canvas.y), static_cast<int>(canvas.width), static_cast<int>(canvas.height));
		BeginMode2D(camPreview);
		if (IsTextureValid(bigPreview)) {
			const float wheel = GetMouseWheelMove();
			if (wheel != 0.0f) {
				constexpr float maxZoom = 3.0f;
				constexpr float minZoom = 0.1f;

				const Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camPreview);
				camPreview.offset = GetMousePosition();
				camPreview.target = mouseWorldPos;

				const float scale = 0.1f * wheel;

				camPreview.zoom = Clamp(expf(logf(camPreview.zoom) + scale), minZoom, maxZoom);
			}
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), canvas)) {
				camPreview.target -= GetMouseDelta() / camPreview.zoom;
			}
			DrawTexture(bigPreview, 0, 0, WHITE);
		}
		EndMode2D();
		EndScissorMode();
		DrawRectangleLinesEx(canvas, static_cast<float>(GuiGetStyle(STATUSBAR, BORDER_WIDTH)), BLACK);

		loadFilesButton.draw();

		scroll.draw();
		loadMenu.draw();
		EndDrawing();
	}

	CloseWindow();
	NFD_Quit();

	return 0;
}
