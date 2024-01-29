#ifndef _CHAMPION_H_
#define _CHAMPION_H_

#include "op.h"
#include "vm.h"

typedef struct champion {
  int id;
  unsigned char registers[REG_NUMBER][REG_SIZE]; // Registers of the program
  int counter; // Program counter
  int carry_flag; 
} champion_t;

#endif