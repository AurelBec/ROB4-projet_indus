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
#include <sys/select.h>

unsigned char getch();
int kbhit(void);
