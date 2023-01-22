#include "raylib.h"
#include <stdio.h>
#include "font.h"
#include "codeblocks.h"
#include <string.h>

FILE *fp;
bool nextFrame;

typedef struct {
  unsigned inst : 4;
  unsigned src : 2;
  unsigned dest : 1;
  unsigned c : 1;
} Cmd;

void nextInst() {
  Cmd cmd;
  char *cmdc;
  fread(&cmd, 1, 1, fp);
  memcpy(cmdc,&cmd,1);
  if (cmd.c) {
    if (c) {
      instfn[blockfn[*cmdc]](cmd.src, cmd.dest); //pointers >:DDD
    }
  } else {
    instfn[blockfn[*cmdc]](cmd.src, cmd.dest); //pointers >:DDD
  }
}

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
