#include "raylib.h"
#include <stdint.h>
#include "advmath.h"

Color blockcolors[32] = {
  BLANK, RED, YELLOW, GREEN
};

char blocknames[32][8] = {
  "", "TEST"
};

Color calccol(int inst) {
  int r = (inst%2)*255;
  int g = (inst%4)*191;
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

Shape blockshapes[32][8] = {
    {},
    {{1,2,2,5,5}}
};

void draw_pcsl(unsigned char block, int x, int y, int scale, Color color) {
  int inst = block%32; block <<= 5;
  int addrmode = block%4; block <<= 2;
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
