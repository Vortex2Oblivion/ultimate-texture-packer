#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "ui/ImageScrollList.hpp"
#include "ui/RenderArea.hpp"
#include "ui/SpritesheetLoadMenu.hpp"
#include "utils/FileUtil.hpp"
#include "utils/Repacker.hpp"

#if !defined(NDEBUG) || defined(_DEBUG)
#define DEBUG
#endif

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

	loadMenu.repack.onPress.append([&scroll, &loadMenu] {
		Image dst;
		const Image src = LoadImageFromTexture(loadMenu.selectedSpritesheetPreview);

		utp::utils::Repacker::repack(dst, src, 8192, 8192, loadMenu.frames, true);

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

#ifdef DEBUG
		DrawFPS(0, 0);
#endif
		EndDrawing();
	}

	CloseWindow();
	NFD_Quit();

	return 0;
}
