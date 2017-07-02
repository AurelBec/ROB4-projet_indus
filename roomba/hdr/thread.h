#ifndef __Thread__Thread__
#define __Thread__Thread__

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
#include <mutex>

void eventf(char *event, int value);
void *listen_cmd_usr(void *cmd);

typedef struct Cmd_usr {
	int port;
	char key;
	bool check;
	std::mutex *mtx;
} Cmd_usr;

#endif
