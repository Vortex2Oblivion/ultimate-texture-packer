#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raymath.h"
#include "ui/ImageScrollList.hpp"
#include "ui/RenderArea.hpp"
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

	scroll.onSelect.append([&scroll, &bigPreview] { bigPreview = scroll.currentTexture; });


	auto outputPreview = utp::ui::RenderArea(loadFilesButton.x + loadFilesButton.width, loadFilesButton.y + loadFilesButton.height,
											 static_cast<float>(GetRenderWidth()) - scroll.width, screenHeight);

	outputPreview.onDraw.append([&bigPreview] {
		if (IsTextureValid(bigPreview)) {
			DrawTexture(bigPreview, 0, 0, WHITE);
		}
	});


	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);

		outputPreview.draw();
		loadFilesButton.draw();
		scroll.draw();
		loadMenu.draw();

		EndDrawing();
	}

	CloseWindow();
	NFD_Quit();

	return 0;
}
