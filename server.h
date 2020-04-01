#include <stdio.h>
#include <string.h>
#include <string>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef LOCALHOST
#define LOCALHOST "127.0.0.1"
#endif

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 8888
#endif

#ifndef DEFAULT_MAX_CLIENT
#define DEFAULT_MAX_CLIENT 20
#endif

#ifndef PORT_EXP_LIMIT
#define PORT_EXP_LIMIT 16
#endif

#ifndef SERVER_F_INCLUDED
#define SERVER_F_INCLUDED

void printHelp();
static void die(int line, const char* format, ...);
static void initializeServer(unsigned int port, unsigned int max_client);

#endif