/*
 * This file contains auxiliary function definitions
 */

#ifndef UTILS_H
#define UTILS_H

#ifndef MAX
  #define MAX(a,b) ( (a > b) ? a : b )
#endif

#ifndef MIN
  #define MIN(a,b) ( (a > b) ? b : a )
#endif

#ifndef ABS
  #define ABS(a) ( (a > 0) ? a : -a )
#endif

// stops cmd.exe from terminating
void pause();

// wait before continuing
void delay(int seconds);

// clear screen
void clear();

#endif
