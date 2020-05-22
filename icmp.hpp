#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h> 

#include "constants/constants.h"

#define ICMP_SIZE (sizeof(ICMP))

class ICMP {
    public:
        unsigned char type;
        unsigned char code;
        unsigned short checksum;
        unsigned short id;
        unsigned short sequence; 
        struct timeval timestamp;
};

class IP {
    public:
        #if __BYTE_ORDER == __LITTLE_ENDIAN || __BYTE_ORDER == __BIG_ENDIAN
        unsigned char   hlen:4; // Header length
        unsigned char   version:4; // IP version <ipv4>
        #endif
        unsigned char   tos; // Service type
        unsigned short  len; // Total length
        unsigned short  id;  // Identity
        unsigned short  offset; // Offset
        unsigned char   ttl; // Duration time
        unsigned char   protocol; // Protocal
        unsigned short  checksum; // The sum for validation
        struct in_addr ipsrc; // ip source address
        struct in_addr ipdst; // ip destination address
};

/**
 * @brief A quit function in case of any exceptions happen
 * @param line      The line which causes the error
 * @param format    A string that specify error ouput format & content
*/
static void die(int line, const char* format, ...) {
    va_list vargs;
    va_start(vargs, format);
    fprintf(stderr, "%d: ", line);
    vfprintf(stderr, format, vargs);
    va_end(vargs);
    exit(EXIT_FAILURE);
}

unsigned short checkSum(unsigned short *, int);

float timediff(struct timeval *, struct timeval *);

void pack(ICMP *, int);

int unpack(char *, int, char *);