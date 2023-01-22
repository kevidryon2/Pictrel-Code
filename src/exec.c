#include "raylib.h"
#include <stdio.h>
#include "advmath.h"
#include "font.h"

FILE *fp;


void nextInst() {

}

int main(int argc, char **argv) {
  InitWindow(512,512,"Pictrel Code");
  while (WindowShouldClose()) {
    BeginDrawing();
    nextInst();
    EndDrawing();
  }
}
