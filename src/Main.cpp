#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main()
{
	
	InitWindow(1280, 720, "Ultimate Texture Packer");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}