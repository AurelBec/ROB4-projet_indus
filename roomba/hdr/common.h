#ifndef __Common__Common__
#define __Common__Common__

#include <stdio.h>
#include <unordered_map>
#include <array>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <thread>
#include <math.h>

#define PI 3.14159265

unsigned char getch();
int kbhit(void);
void eventf(char *event, int value);
void conv_to_bin(int val, char* pStrBin);
int binsign_to_int( char *str);
bool is_in_array(char val);


#endif
