#include "defines.h"
#ifndef GLOBALS
#define GLOBALS
volatile char userCombo[NUMBEROFKEYS];
volatile char time;
volatile signed char j;
volatile char mageCombo[NUMBEROFKEYS] = {'1','2','3','4'};
volatile char correctKeys;
volatile char command;
volatile char firstRun;
volatile char keys[16];
volatile int seed = 3213;
char scrollDelay = 150;
#endif
