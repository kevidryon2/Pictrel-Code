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
void power(char cmd);
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
void uscr(char cmd);
void line(char cmd);
void circ(char cmd);
void rect(char cmd);
void rep(char cmd);
void cmemcpy(char cmd);
void romcpy(char cmd);
void memz(char cmd);
void end(char cmd);

void ill(char cmd);

Color blockcolors[64] = {
  BLANK, RED, YELLOW, GREEN
};

char blocknames[64][8] = {
  "", "TEST"
};

Color calccol(int inst) {
  int r = (inst%2)*255;
  int g = (inst%4)*127;
  int b = (inst%8)*225;
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

void draw_pcsl(unsigned char block, int x, int y, int scale, Color color) {
  int c = block%2;
  int indirect = (block%4)/2;
  int inst = block/4;
  Color bg = calccol(inst);
  DrawRectangle(x,y,64,64,bg);
  DrawText(blocknames[inst], x+10, y+2, 10, color);
  /*Shape *cmd = blockshapes[inst];
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
  }*/
}
