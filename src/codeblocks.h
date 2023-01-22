#include "raylib.h"
#include <stdint.h>
#include "advmath.h"
#include "codelang.h"
#include "font.h"

int a;
bool c;
int labeled_loc;

//Instructions that require a source parameter (S != 11)
void nop(int s, int d);
void add(int s, int d);
void subtract(int s, int d);
void multiply(int s, int d);
void divide(int s, int d);
void modulo(int s, int d);
void logor(int s, int d);
void logand(int s, int d);
void set(int s, int d);
void equal(int s, int d);
void less(int s, int d);
void jump(int s, int d);
void call(int s, int d);
void drawch(int s, int d);
void push(int s, int d);
void calcsin(int s, int d);

//Instructions that require only a dest parameter or nothing (S == 11)

void pop(int s, int d);
void keypressed(int s, int d);
void keydown(int s, int d);
void mousex(int s, int d);
void mousey(int s, int d);
void mouseleftdown(int s, int d);
void mouserightdown(int s, int d);
void mousewheel(int s, int d);
void updatescreen(int s, int d);
void copymem(int s, int d);
void zeromem(int s, int d);
void stdior(int s, int d);
void zeroc(int s, int d);
void invertc(int s, int d);
void random(int s, int d);
void end(int s, int d);

Color blockcolors[16] = {
  BLANK, RED, YELLOW, GREEN
};

char blocknames[16][8] = {
  "", "TEST"
};

unsigned char blockfn[256] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x1a, 0x1b, 0x1d, 0x1e, 0x00, 0x1f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x00, 0x19, 0x1b, 0x1c, 0x1e, 0x00, 0x1f,

};

void (*instfn[32])() = {
  nop, add, subtract, multiply, divide, modulo, logor, logand, set, equal, less, jump, call, drawch, push, calcsin,
  pop, keypressed, keydown, mousex, mousey, mouseleftdown, mouserightdown, mousewheel, updatescreen, copymem, zeromem, stdior, zeroc, invertc, random, end
};

Color calccol(int inst) {
  int r = (inst%2)*127;
  int g = (inst%4)*96;
  int b = (inst%8)*112;
  return (Color){r,g,b,inst?255:0};
}

//See PCSL.md for documentation
typedef struct {
  unsigned command : 4;
  unsigned x : 4;
  unsigned y : 4;
  unsigned x2 : 4;
  unsigned y2 : 4;
  unsigned unused : 4;
} Shape;

Shape blockshapes[16][8] = {
    {},
    {{1,2,2,5,5}}
};

void draw_pcsl(unsigned char block, int x, int y, int scale, Color color) {
  int inst = block%16; block <<= 4;
  int srcmode = block%4; block <<= 2;
  int destmode = block%2; block <<= 1;
  int c = block%2; block <<= 1;
  Shape *cmd = blockshapes[inst];
  Color bg = calccol(inst);
  DrawRectangle(x,y,64,64,bg);
  DrawText(blocknames[inst], x+2, y+2, 5, color);
  for (int i=0; i<8; i++) {
    switch (cmd[i].command) {
      case 1: DrawRectangle(cmd[i].x*scale+x, cmd[i].y*scale+y, cmd[i].x2*scale, cmd[i].y2*scale, color); break;
      case 2: DrawRectangleLinesEx((Rectangle){cmd[i].x*scale+x, cmd[i].y*scale+y, cmd[i].x2*scale, cmd[i].y2*scale}, scale, color); break;
      case 3: DrawRectangle(cmd[i].x*scale+x, cmd[i].y*scale+y, cmd[i].x2*scale, cmd[i].y2*scale, bg); break;
      case 4: DrawLineEx((Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){cmd[i].x2*scale,cmd[i].y2*scale}, scale, color); break;
      case 5: DrawLineEx((Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){cmd[i].x2*scale,cmd[i].y2*scale}, scale, bg); break;
      case 6: DrawLineBezierQuad((Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){(cmd[i].x+(cmd[i].x2/2))*scale,(cmd[i].y-(cmd[i].y2/2))*scale}, scale, color); break;
      case 7: DrawLineBezierQuad((Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){(cmd[i].x+(cmd[i].x2/2))*scale,(cmd[i].y+(cmd[i].y2/2))*scale}, scale, color); break;
      case 8: DrawLineBezierQuad((Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){(cmd[i].x-(cmd[i].x2/2))*scale,(cmd[i].y+(cmd[i].y2/2))*scale}, scale, color); break;
      case 9: DrawLineBezierQuad((Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){cmd[i].x*scale+x,cmd[i].y*scale+y}, (Vector2){(cmd[i].x+(cmd[i].x2/2))*scale,(cmd[i].y+(cmd[i].y2/2))*scale}, scale, color); break;
      case 10: DrawCircle(cmd[i].x+8*scale, cmd[i].y+8*scale, ((cmd[i].x2+cmd[i].y2)/2)*scale, color); break;
      case 11: DrawCircleLines(cmd[i].x+8*scale, cmd[i].y+8*scale, ((cmd[i].x2+cmd[i].y2)/2)*scale, color); break;
      case 12: DrawCircle(cmd[i].x+8*scale, cmd[i].y+8*scale, ((cmd[i].x2+cmd[i].y2)/2)*scale, bg); break;
    }
  }
}
