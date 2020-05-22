#include "icmp.hpp"

char buf[BUF_SIZE] = {0};

int main(int argc, char *argv[])
{
    struct hostent *host;
    ICMP sendicmp;
    struct sockaddr_in from;
    struct sockaddr_in to;
    int fromlen = 0;
    int sockfd;
    int nsend = 0;
    int nreceived = 0;
    int i, n;
    in_addr_t inaddr;

    memset(&from, 0, sizeof(struct sockaddr_in));
    memset(&to, 0, sizeof(struct sockaddr_in));

    if (argc < 2)
        die(__LINE__, "use : %s hostname/IP address \n", argv[0]);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
        die(__LINE__, "socket() error \n");
    to.sin_family = AF_INET;

    // Process ip and domain separately
    if ((inaddr = inet_addr(argv[1])) == INADDR_NONE)
    {
        // Domain detected
        if ((host = gethostbyname(argv[1])) == NULL)
            die(__LINE__, "gethostbyname() error \n");
        to.sin_addr = *(struct in_addr *)host->h_addr_list[0];
    }
    else
    {
        // IP detected
        to.sin_addr.s_addr = inaddr;
    }

    // Print message
    printf("ping %s (%s) : %d bytes of data.\n", argv[1], inet_ntoa(to.sin_addr), (int)ICMP_SIZE);

    // Send ICMP message for NUM times
    for (i = 0; i < NUM; i++)
    {
        memset(&sendicmp, 0, ICMP_SIZE);
        pack(&sendicmp, nsend);

        // Send ICMP message
        if (sendto(sockfd, &sendicmp, ICMP_SIZE, 0, (struct sockaddr *)&to, sizeof(to)) == -1)
        {
            printf("sendto() error \n");
            continue;
        }
        nsend++;

        printf("%d", nsend);

        // Receive ICMP message
        if ((n = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&from, (socklen_t *)&fromlen)) < 0)
        {
            printf("recvform() error \n");
            continue;
        }
        nreceived++;

        printf("%d", nreceived);

        if (unpack(buf, n, inet_ntoa(from.sin_addr)) == -1)
            printf("unpack() error \n");

        sleep(1);
    }

    printf("---  %s ping statistics ---\n", argv[1]);
    printf("%d packets transmitted, %d received, %%%d packet loss\n", nsend, nreceived,
           (nsend - nreceived) / nsend * 100);

    return 0;
}