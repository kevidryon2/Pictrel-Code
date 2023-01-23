#include "raylib.h"
#include "codeblocks.h"
#include <stdio.h>

#define DIR_LEFT 0
#define DIR_RIGHT 1
#define NUM_ROWS 9

unsigned char code[NUM_ROWS][16384] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
int num_rows = 2, cx, cy, scroll, t, kht, sb, temp1, temp2, temp3, temp4;
KeyboardKey kh;
bool dark = true;
int sit=0;
int sid=0;

void saveCode() {
  
}

void runCode() {
  key(KEY_S);
}

void key(KeyboardKey k) {
  if (IsKeyReleased(kh)) {
    kh=0;
    kht=0;
  }
  
  if (IsKeyPressed(k)) {
    kh=k;
    kht=1;
  }
                          
  switch (k) {
    case KEY_R: runCode(); break;
    case KEY_S: saveCode(); break;
  case KEY_UP: cy--; break;
  case KEY_DOWN: cy++; break;
  case KEY_LEFT: cx--; break;
  case KEY_RIGHT: cx++; break;
  case KEY_Z:;
    while (num_rows<cy+1) {
      num_rows++;
    }
    code[cy][cx+scroll/64] = sb;
    break;
  case KEY_X:
    if (cy<num_rows) {
      code[cy][cx+scroll/64] = 0;
    }
    break;
  case KEY_O: sb--; break;
  case KEY_P: sb++; break;
  }
}

void update() {
  key(GetKeyPressed());
  t++;
  if ((kht>30) & !(kht%4)) {
    key(kh);
  }
  if (cx>19) {
    cx=19;
    scroll+=64;
  } else if (cx<0) {
    cx=0;
    scroll-=64;
  }
  cx=(cx<0)?0:cx;
  cy=(cy<0)?0:cy;
  cy=(cy>8)?8:cy;
  scroll=(scroll<0)?0:scroll;
  if (kh>0) {
    kht++;
  }
}

void draw() {
	ClearBackground((Color){255-dark*208,255-dark*208,255-dark*208,255});
	DrawRectangle(0, 0, 1280, 45, (Color){24,24,32,255});
	DrawRectangle(0, 0, 1280, 35, (Color){36,36,48,255});
	DrawRectangle(0, 621, 1280, 92, (Color){24,24,32,255});
	DrawRectangle(0, 631, 1280, 92, (Color){36,36,48,255});
  DrawText(TextFormat("Pictrel Code IDE v0.3.0\t\tX:%d    Y:%d    T:%d    B:%d",cx+(scroll/64),cy,t,sb),15,14,20,WHITE);
  for (int y=0; y<num_rows; y++) {
    for (int x=0; x<(GetRenderWidth()/64); x++) {
      draw_pcsl(code[y][x+scroll/64], x*64, y*64+45, 4, BLACK);
    }
  }
  draw_pcsl(sb, cx*64, cy*64+45, 64/16, (blockcolors[sb].r+blockcolors[sb].g+blockcolors[sb].b<255)?WHITE:BLACK);
  DrawRectangle(cx*64, cy*64+45, 64, 64, (Color){255,255,255,30+t%60});
}

int main() {
	InitWindow(1280,720,"Pictrel Code IDE");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		update();
		BeginDrawing();
		draw();
		EndDrawing();
	}
}
