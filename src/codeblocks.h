#include "raylib.h"
#include <stdint.h>
#include "advmath.h"
#include "codelang.h"
#include "font.h"

int a;
bool c;
int labeled_loc;

void nop(char cmd);
void add(char cmd);
void sub(char cmd);
void mul(char cmd);
void divi(char cmd);
void mod(char cmd);
void or(char cmd);
void and(char cmd);
void xor(char cmd);
void inc(char cmd);
void dec(char cmd);
void cp(char cmd);
void cpo(char cmd);
void cpf(char cmd);
void equ(char cmd);
void lt(char cmd);
void mt(char cmd);
void jp(char cmd);
void jr(char cmd);
void call(char cmd);
void ret(char cmd);
void push(char cmd);
void pop(char cmd);
void cmputc(char cmd);
void cmsin(char cmd);
void cmsqrt(char cmd);
void rep(char cmd);
void cmemcpy(char cmd);
void romcpy(char cmd);
void memz(char cmd);
void end(char cmd);

void illinst();

Color blockcolors[64] = {
  BLANK, RED, YELLOW, GREEN
};

char blocknames[64][8] = {
  "", "TEST"
};

void (*instfn[64])() = {
  nop, add, subtract, multiply, divide, modulo, logor, logand, set, equal, less, jump, call, drawch, push, calcsin,
  pop, keypressed, keydown, mousex, mousey, mouseleftdown, mouserightdown, mousewheel, updatescreen, copymem, zeromem, stdior, zeroc, invertc, ran, end
};

Color calccol(int inst) {
  int r = (inst%2)*63;
  int g = (inst%4)*48;
  int b = (inst%8)*56;
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

Shape blockshapes[64][8] = {
    {},
    {{1,2,2,5,5}}
};

void draw_pcsl(unsigned char block, int x, int y, int scale, Color color) {
  int c = block%2; block <<= 1;
  int indirect = block%2; block <<= 1;
  int inst = block%64; block <<= 6;
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
