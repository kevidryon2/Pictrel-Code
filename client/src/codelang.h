#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "font.h"

//OOOOOOIC SSSSSSSS DDDDDDDD
//I = 0:
//S = 0: SSSSSSSS VVVVVVVV (Constant)
//S > 0: RRRRRRRR (Register)
//D = 0: STDOUT
//D > 0: RRRRRRRR (Register)
//
//I = 1:
//S = 0: STDIN
//S > 0: IIIIIIII (Indirect)
//D = 0: STDERR
//D > 0: IIIIIIII (Indirect)
//


FILE *fp;
bool nextFrame, stopped;
unsigned char memory[255];
bool c;
int pc = 0;
short stacktt[256];
unsigned char sp;

char getsrcarg(char cmd) {
  char s;
  s = fgetc(fp);
  pc++;
  switch ((cmd%4)>>1) { //switch (I)
    case 0:
      if (s) {
        s = memory[s-1];
      } else {
        s = fgetc(fp);
        pc++;
      }
      break;
    case 1:
      if (s) {
        s = memory[memory[s]];
      } else {
        s = fgetc(stdin);
      }
      break;
  }
  return s;
}

short getwsrcarg(char cmd) {
  short s;
  s = fgetc(fp);
  pc++;
  switch ((cmd%4)>>1) { //switch (I)
    case 0:
      if (s) {
        s = memory[s-1]+memory[s]*256;
      } else {
        char l = fgetc(fp);
        char h = fgetc(fp);
        s = l+h*256;
        pc+=2;
      }
      break;
    case 1:
      if (s) {
        s = memory[memory[s]];
      } else {
        char l = fgetc(stdin);
        char h = fgetc(stdin);
        s = l*256+h;
      }
      break;
  }
  return s;
}

void setdestarg(char cmd, char data) {
  char d;
  d = fgetc(fp);
  pc++;
  switch ((cmd%4)>>1) { //switch (I)
    case 0:
      if (d) {
        memory[fgetc(fp)] = data;
        pc++;
      } else {
        fputc(data, stdout);
      }
      break;
    case 1:
      if (d) {
        memory[memory[fgetc(fp)]] = data;
        pc++;
      } else {
        fputc(data, stderr); //TODO: Make this line of code set value of save file
      }
      break;
  }
}

Color pal(char index) {
  struct {
    unsigned r : 3;
    unsigned g : 3;
    unsigned b : 2;
  } rgbp;
  memcpy(&rgbp, &index, 1);
  return (Color){rgbp.r*85,rgbp.g*85,rgbp.b*127,255};
}

void nop(char cmd) {};

void add(char cmd) {
  setdestarg(cmd, getsrcarg(cmd)+getsrcarg(cmd));
}

void sub(char cmd) {
  char v = getsrcarg(cmd); //To ensure 1st source is the minuend
  setdestarg(cmd, v-getsrcarg(cmd));
}

void mul(char cmd) {
  setdestarg(cmd, getsrcarg(cmd)*getsrcarg(cmd));
}

void divi(char cmd) {
  char v = getsrcarg(cmd); //To ensure 1st source is the dividend
  setdestarg(cmd, v/(unsigned)getsrcarg(cmd));
}

void mod(char cmd) {
  char v = getsrcarg(cmd);
  setdestarg(cmd, v%getsrcarg(cmd));
}

void power(char cmd) {
  char v = getsrcarg(cmd);
  setdestarg(cmd, pow(v,getsrcarg(cmd)));
}

void or(char cmd) {
  setdestarg(cmd, getsrcarg(cmd)|getsrcarg(cmd));
}

void and(char cmd) {
  setdestarg(cmd, getsrcarg(cmd)&getsrcarg(cmd));
}

void xor(char cmd) {
  setdestarg(cmd, getsrcarg(cmd)^getsrcarg(cmd));
}

void inc(char cmd) {
  setdestarg(cmd, getsrcarg(cmd)+1);
}

void dec(char cmd) {
  setdestarg(cmd, getsrcarg(cmd)-1);
}

void cp(char cmd) {
  setdestarg(cmd, getsrcarg(cmd));
}

void equ(char cmd) {
  c = (getsrcarg(cmd) == (unsigned)getsrcarg(cmd));
}

void lt(char cmd) {
  c = (getsrcarg(cmd) < (unsigned)getsrcarg(cmd));
}

void mt(char cmd) {
  c = (getsrcarg(cmd) > (unsigned)getsrcarg(cmd));
}

void jp(char cmd) {
  fseek(fp, (unsigned)getwsrcarg(cmd), SEEK_SET);
}

void jr(char cmd) {
  fseek(fp, getsrcarg(cmd), SEEK_CUR);
}

void cmputc(char cmd) {
  unsigned char c = getsrcarg(cmd);
  unsigned char x = getsrcarg(cmd);
  unsigned char y = getsrcarg(cmd);
  unsigned char fg = getsrcarg(cmd);
  unsigned char bg = getsrcarg(cmd);
  cputc(c, x, y, pal(fg), pal(bg));
}

void cmsin(char cmd) {
  //sin((<src>/255)*(π/2))*255
  float f = getsrcarg(cmd);
  f = (f/255)*(PI/2);
  f = (sin(f)*255);
  setdestarg(cmd, (int)f);
}

void cmsqrt(char cmd) {
  setdestarg(cmd, sqrt(getsrcarg(cmd)));
}

void uscr(char cmd) {
  EndDrawing();
  BeginDrawing();
}

void line(char cmd) {
  int x1 = (unsigned)getsrcarg(cmd);
  int y1 = (unsigned)getsrcarg(cmd);
  int x2 = (unsigned)getsrcarg(cmd);
  int y2 = (unsigned)getsrcarg(cmd);
  int col = (unsigned)getsrcarg(cmd);
  DrawLineEx((Vector2){x1*scalefactor,y1*scalefactor},(Vector2){x2*scalefactor,y2*scalefactor},scalefactor,pal(col));
}

void circ(char cmd) {
  int x = (unsigned)getsrcarg(cmd);
  int y = (unsigned)getsrcarg(cmd);
  int rad = (unsigned)getsrcarg(cmd);
  int col = (unsigned)getsrcarg(cmd);
  DrawCircle(x, y, rad, pal(col));
}

void rect(char cmd) {
  int x = (unsigned)getsrcarg(cmd);
  int y = (unsigned)getsrcarg(cmd);
  int w = (unsigned)getsrcarg(cmd);
  int h = (unsigned)getsrcarg(cmd);
  int col = (unsigned)getsrcarg(cmd);
  for (int xp=0; xp<w; xp++) {
    for (int yp=0; yp<h; yp++) {
      pixel(xp+x, yp+y, pal(col));
    }
  }
}

void cmemcpy(char cmd) {
  int from = (unsigned)getsrcarg(cmd);
  int to = (unsigned)getsrcarg(cmd);
  int count = (unsigned)getsrcarg(cmd);
  for (int i=0; i<count; i++) {
    memory[to+i] = *memory+from+i; //pointers >:D
  }
}

void romcpy(char cmd) {
  int from = (unsigned)getwsrcarg(cmd);
  int to = (unsigned)getsrcarg(cmd);
  int count = (unsigned)getwsrcarg(cmd);
  int ogseek = ftell(fp);
  fseek(fp,from,SEEK_SET);
  for (int i=0; i<count; i++) {
    memory[to+i] = fgetc(fp);
  }
  fseek(fp,ogseek,SEEK_SET);
}

void memz(char cmd) {
  int addr = (unsigned)getsrcarg(cmd);
  int count = (unsigned)getsrcarg(cmd);
  for (int i=0; i<count; i++) {
    memory[addr+i] = 0;
  }
}

void end(char cmd) {
  nextFrame = true;
  stopped = true;
  printf("Program stopped.\n");
}

void doYouWishToContinue() {
  printf("Do you wish to continue?\n");
  char option[2];
  scanf("%2s", option);
  if (option[0]=='n') {
    CloseWindow();
    exit(127);
  }
}

void ill(char cmd) {
  printf("Illegal instruction %02x at %04lx. ", (unsigned)cmd, ftell(fp));
  doYouWishToContinue();
};

void stackOverflow() {
  printf("Stack overflow at %04lx. ", ftell(fp));
  doYouWishToContinue();
}

void stackUnderflow() {
  printf("Stack underflow at %04lx. ", ftell(fp));
  doYouWishToContinue();
}

void call(char cmd) {
  if (sp==255) {
    stackOverflow();
  }
  stacktt[sp] = ftell(fp);
  sp++;
  fseek(fp, getwsrcarg(cmd), SEEK_SET);
}

void ret(char cmd) {
  if (!sp) {
    stackUnderflow();
  }
  sp--;
  fseek(fp, stacktt[sp], SEEK_SET);
}

void push(char cmd) {
  if (sp==255) {
    stackOverflow();
  }
  stacktt[sp] = ftell(fp);
  sp++;
}

void pop(char cmd) {
  if (!sp) {
    stackUnderflow();
  }
  sp--;
  setdestarg(cmd, stacktt[sp]);
}

void cpo(char cmd) {
  nop(cmd); //TODO: implement online variables (first, TODO: create website frontend, TODO: create website server (backend), TODO: implement users)
}

void setfdestarg(char cmd, char data) {
  char d;
  d = fgetc(fp);
  char *flatcfont = cfont; //pointers >:D
  pc++;
  switch ((cmd%4)>>1) { //switch (I)
    case 0:
      if (d) {
        char v = fgetc(fp);
        flatcfont[v+fgetc(fp)*256] = data;
        pc++;
      } else {
        ill(cmd);
      }
      break;
    case 1:
      if (d) {
        char v = fgetc(fp);
        flatcfont[memory[v]+memory[fgetc(fp)]*256] = data;
        pc++;
      } else {
        ill(cmd);
      }
      break;
  }
}

void cpf(char cmd) {
  setfdestarg(cmd, getsrcarg(cmd));
}

void rep(char cmd) {
  for (int i=0; i<getsrcarg(cmd); i++) {
    nextInst();
  }
}

void (*instfn[64])(char) = {
  nop,add,sub,mul,divi,mod,power,or,and,xor,inc,dec,cp,cpo,cpf,equ,lt,
  mt,jp,jr,call,ret,push,pop,cmputc,cmsin,cmsqrt,uscr,line,circ,rect,ill,ill,
  ill,ill,ill,ill,ill,ill,ill,ill,ill,ill,ill,ill,ill,ill,ill,ill,
  ill,ill,ill,ill,ill,ill,ill,ill,ill,ill,rep,cmemcpy,romcpy,memz,end
};

typedef struct {
  unsigned int o : 6;
  unsigned int i : 1;
  unsigned int c : 1;
} Cmd;

unsigned char data[16384];

void nextInst() {
  Cmd cmd;
  unsigned char cmdc;
  cmdc = data[pc];
  memcpy(&cmd,&cmdc,1);
  printf("%03d %02x ", pc, cmdc);
  if (cmd.c) {
    if (c) {
      instfn[(cmdc/4)%64](cmdc); //pointers >:DDD
    }
  } else {
    printf("%d", (cmdc/4)%64);
    instfn[(cmdc/4)%64](cmdc); //pointers >:DDD
  }
  printf("\n");
  pc++;
}
