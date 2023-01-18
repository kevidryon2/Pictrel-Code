#include "raylib.h"

void update() {
	//Add your code here!
}

void draw() {
	ClearBackground(BLACK);
	//Add your code here!
}

int main() {
	InitWindow(256,256,"Pictrel Code");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		update();
		BeginDrawing();
		draw();
		EndDrawing();
	}
}
