#include "raylib.h"

typedef enum {
  OPTIONS_OK = 0,
  OPTIONS_YES_NO,
  OPTIONS_YES_NO_CANCEL
} BoxOption;


//Returns width of option
int DrawOption(char *text, int x, int y) {
  DrawRectangle(x,y, MeasureText(text, 10)+10, 20, DARKGRAY);
  DrawText(text, x+5,y+2, 10, WHITE);
  return MeasureText(text, 10)+10;
}

int InitChooseMessageBox(char *text, BoxOption options) {
  InitWindow(MeasureText(text, 20)+20, 100, "Message Box");
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText(text,10,10,20,WHITE);
  switch (options) {
  case OPTIONS_OK:
    DrawOption("OK",10,20);
    break;
  case OPTIONS_YES_NO:
    DrawOption("No",10+DrawOption("Yes", 10, 20),20);
    break;
  case OPTIONS_YES_NO_CANCEL:
    DrawOption("Cancel", DrawOption("No",10+DrawOption("Yes", 10, 20),20),20);
    break;
  }
  EndDrawing();
  int k;
  while (!WindowShouldClose()) {
    BeginDrawing();
    k = GetKeyPressed();
    if (k>=KEY_ONE & k<=KEY_NINE) {
      k-=KEY_ONE;
      switch (k) {
        case 0: return 0; break;
        case 1: if (options>OPTIONS_OK) return 1; break;
        case 2: if (options>OPTIONS_YES_NO) return 2; break;
      }
    }
    EndDrawing();
  }
}
