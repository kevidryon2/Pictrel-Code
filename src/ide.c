#include "raylib.h"
#include "codeblocks.h"
#include <stdio.h>

#define DIR_LEFT 0
#define DIR_RIGHT 1

unsigned char code[16384][9];
int row_lengths[9];
int num_rows, cx, cy, scroll, t, kht, sb, temp1, temp2, temp3, temp4;
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
    if (!(cy<num_rows)) {
      num_rows=cy;
    }
    code[cx+scroll/64][cy] = sb;
    break;
  case KEY_O: sb--; break;
  case KEY_P: sb++; break;
  }
}

void update() {
  printf("\n");
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
  DrawText(TextFormat("Pictrel Code IDE v0.1.1\t\tX:%d    Y:%d    T:%d    B:%d",cx+(scroll/64),cy,t,sb),15,14,20,WHITE);
  //DrawText("Press R to run code",GetRenderWidth()-228,14,20,WHITE);
  unsigned char codeblock;
  for (int y=0; y<num_rows; y++) {
    for (int x=0; x<(GetRenderWidth()/64); x++) {
      draw_pcsl(code[x][y], x*64, y*64+45, 4, BLACK);
    }
  }
  draw_pcsl(sb, cx*64, cy*64+45, 64/16, dark?WHITE:BLACK);
  DrawRectangle(cx*64, cy*64+45, 64, 64, (Color){255,255,255,30+t%60});
}

int main() {
  for (int i=0; i<10; i++) {
    code[16383][i] = 255;
  };
	InitWindow(1280,720,"Pictrel Code IDE");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		update();
		BeginDrawing();
		draw();
		EndDrawing();
	}
}
