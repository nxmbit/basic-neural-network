#ifndef USER_IO_H
#define USER_IO_H

// Clearing screen
#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif


int input_integer(int range_min, int range_max, const char* input_message);
char *input_string(int max_len, const char* input_message);
double input_double(const char* input_message);

#endif
