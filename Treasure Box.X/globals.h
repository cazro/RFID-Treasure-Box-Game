#include "defines.h"
#ifndef GLOBALS
#define GLOBALS
volatile char userCombo[NUMBEROFKEYS];
volatile char time;
volatile signed char j;
volatile char mageCombo[NUMBEROFKEYS] = {'1','1','1','1'};
volatile char correctKeys;
volatile char command;
volatile char firstRun;
volatile char keys[16];
#endif
