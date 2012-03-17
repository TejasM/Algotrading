/*
 * This file contains auxiliary function declarations
 */

#include<iostream>
#include "utils.h"

// stops cmd.exe from terminating
void pause() {
    getchar();
}

// wait before continuing
void delay(int seconds) {
  int t = time(NULL);
  while ( (time(NULL)-t) < seconds );
}

// clear screen
void clear() {
  system("CLS");
}
