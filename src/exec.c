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
  BeginDrawing();
  ClearBackground(BLACK);
  EndDrawing();
  while (!WindowShouldClose()) {
    BeginDrawing();
    while (!nextFrame) nextInst();
    EndDrawing();
  }
}
