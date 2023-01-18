#include "raylib.h"
#include <stdint.h>

Color blockcolors[256] = {
  BLANK, RED, YELLOW, GREEN
};

char blocknames[256][8] = {
  "", "TEST"
};

//See PCSL.md for documentation
typedef struct {
  int command : 4;
  int x : 4;
  int y : 4;
  int x2 : 4;
  int y2 : 4;
  int unused : 4;
} Shape;

Shape blockshapes[256][16] = {
    {},
    {{1,2,2,5,5}}
};

void draw_pcsl(unsigned char block, int x, int y, int scale, Color color) {
  Shape *cmd = blockshapes[block];
  Color bg = blockcolors[block];
  DrawRectangle(x,y,64,64,bg);
  /*DrawText(blocknames[block], x+2, y+2, 5, color);
  for (int i=0; i<16; i++) {
    switch (cmd[i].command) {
      case 1: DrawRectangle(cmd[i].x*scale, cmd[i].y*scale, cmd[i].x2*scale, cmd[i].y2*scale, color); break;
      case 2: DrawRectangleLines(cmd[i].x*scale, cmd[i].y*scale, cmd[i].x2*scale, cmd[i].y2*scale, color); break;
      case 3: DrawRectangle(cmd[i].x*scale, cmd[i].y*scale, cmd[i].x2*scale, cmd[i].y2*scale, bg); break;
      case 4: DrawLineEx((Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){cmd[i].x2*scale,cmd[i].y2*scale}, 3, color); break;
      case 5: DrawLineBezierQuad((Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){(cmd[i].x+(cmd[i].x2/2))*scale,(cmd[i].y-(cmd[i].y2/2))*scale}, 3, color); break;
      case 6: DrawLineBezierQuad((Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){(cmd[i].x+(cmd[i].x2/2))*scale,(cmd[i].y+(cmd[i].y2/2))*scale}, 3, color); break;
      case 7: DrawLineBezierQuad((Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){(cmd[i].x-(cmd[i].x2/2))*scale,(cmd[i].y+(cmd[i].y2/2))*scale}, 3, color); break;
      case 8: DrawLineBezierQuad((Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){cmd[i].x*scale,cmd[i].y*scale}, (Vector2){(cmd[i].x+(cmd[i].x2/2))*scale,(cmd[i].y+(cmd[i].y2/2))*scale}, 3, color); break;
      case 9: DrawCircle(cmd[i].x+8*scale, cmd[i].y+8*scale, ((cmd[i].x2+cmd[i].y2)/2)*scale, color); break;
      case 10: DrawCircleLines(cmd[i].x+8*scale, cmd[i].y+8*scale, ((cmd[i].x2+cmd[i].y2)/2)*scale, color); break;
      case 11: DrawCircle(cmd[i].x+8*scale, cmd[i].y+8*scale, ((cmd[i].x2+cmd[i].y2)/2)*scale, bg); break;
    }
  }*/
}
