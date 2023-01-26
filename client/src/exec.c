#include "raylib.h"
#include <stdio.h>
#include "font.h"
#include "codeblocks.h"
#include <string.h>

int main(int argc, char **argv) {
	if (argc<2) {
		printf("Usage: pictrel-code <file>\n");
		return 1;
	}
	fp = fopen(argv[1], "r");
	if (!fp) {
		printf("Error: File %s is inaccessible\n", argv[1]);
	}
	InitWindow(512,512,"Pictrel Code");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);
	printf("Reading file...\n");
	int prglen = fread(data,1,16384,fp);
	printf("Read %d bytes from program file.\n", prglen);
	for (int i=0; i<prglen; i++) {
		printf("%02x ", data[i]);
	}
	printf("\n");
	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing();
	while (!stopped) nextInst();
	while (!WindowShouldClose()) {
		BeginDrawing();
		DrawText("Program ended.",0,0,20,RED);
		EndDrawing();
	}
}
