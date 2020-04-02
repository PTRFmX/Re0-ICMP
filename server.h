#include <stdio.h>
#include <string.h>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SERVER_F_INCLUDED
#define SERVER_F_INCLUDED

void printHelp();
static void die(int line, const char* format, ...);
static void initializeServer(unsigned int port, unsigned int max_client);

#endif