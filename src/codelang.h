#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

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
char memory[255];
