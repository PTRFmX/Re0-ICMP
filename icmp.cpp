#include <stdio.h>
#include "icmp.hpp"

/**
 * Check the sum of data length
 * @param addr pointer to data buffer
 * @param len data length
 * @return the sum for validation
 */
unsigned short checkSum(unsigned short *addr, int len)
{
    unsigned int sum = 0;
    while (len > 1)
    {
        sum += *addr++;
        len -= 2;
    }
    if (len == 1)
        sum += *(unsigned char *)addr;

    // Add the first 16 bits and last 16 bits in sum (32 bits in total)
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (unsigned short)~sum;
}

/**
 * Calculate the time difference
 * @param begin start time
 * @param end end time
 * @return The time difference
 */
float timediff(struct timeval *begin, struct timeval *end)
{
    int n = (end->tv_sec - begin->tv_sec) * MLN + (end->tv_usec - begin->tv_usec);
    return (float)(n / THD); // Convert to millisecond
}

/**
 * Pack a given ICMP message
 * @param icmp pointer to the ICMP message
 * @param sequence ICMP sequence number
 */
void pack(ICMP *icmp, int sequence)
{
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->checksum = 0;
    icmp->id = getpid();
    icmp->sequence = sequence;
    gettimeofday(&icmp->timestamp, 0);
    icmp->checksum = checkSum((unsigned short *)icmp, ICMP_SIZE);
}

/**
 * Unpack a given ICMP message
 * @param buf  pointer to the IP message buffer
 * @param len  length of the IP message 
 * @param addr Target ip address
 */
int unpack(char *buf, int len, char *addr)
{
    int ipheadlen;
    IP *ip;
    ICMP *icmp;
    float rtt; // Time difference
    struct timeval end; // Time when receiving the ICMP message

    ip = (IP *)buf;

    // Calculate IP header length
    ipheadlen = ip->hlen << 2;

    // Construct ICMP message
    icmp = (ICMP *)(buf + ipheadlen);

    // Calculate ICMP message header length
    len -= ipheadlen;

    // ICMP message header length can not be less than 8
    if (len < 8)
    {
        die(__LINE__, "ICMP packets\'s length is less than 8 \n");
        return -1;
    }

    // Confirm ICMP echo
    if (icmp->type != ICMP_ECHOREPLY ||
        icmp->id != getpid())
    {
        die(__LINE__, "ICMP packets are not send by us \n");
        return -1;
    }

    // Calculate time difference
    gettimeofday(&end, 0);
    rtt = timediff(&icmp->timestamp, &end);

    // Print message
    printf("%d bytes from %s : icmp_seq=%u ttl=%d rtt=%fms \n",
           len, addr, icmp->sequence, ip->ttl, rtt);

    return 0;
}