#include "common.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

static void print_flag(struct addrinfo *ai);
static void print_family(struct addrinfo *ai);
static void print_socktype(struct addrinfo *ai);
static void print_protocol(struct addrinfo *ai);

int
main(int argc, char *argv[])
{
    int err;
    const char *str;
    char buf[INET_ADDRSTRLEN];
    struct addrinfo hint;
    struct addrinfo *ailist, *ai;
    struct sockaddr_in *sd;

    if (argc != 3)
        err_quit("Usage: %s node service", argv[0]);

    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_flags = AI_CANONNAME;

    err = getaddrinfo(argv[1], argv[2], &hint, &ailist);
    if (err != 0)
        err_exit(err, "getaddrinfo error");

    for (ai = ailist; ai != NULL; ai = ai->ai_next) {
        print_flag(ai);
        print_family(ai);
        print_socktype(ai);
        print_protocol(ai);

        printf("\n\thost %s", ai->ai_canonname ? ai->ai_canonname : "-");

        if (ai->ai_family == AF_INET) {
            sd = (struct sockaddr_in *)ai->ai_addr;
            str = inet_ntop(AF_INET, &sd->sin_addr, buf, INET_ADDRSTRLEN);
            if (str == NULL)
                err_sys("inet_ntop error");

            printf(" address %s", str);
            printf(" port %hhu\n", ntohs(sd->sin_port));
        }
    }

    exit(0);
}

static void
print_flag(struct addrinfo *ai)
{
    printf("flags");

    if (ai->ai_flags == 0) {
        printf(" 0");
    } else {
        if (ai->ai_flags & AI_ADDRCONFIG)
            printf(" addrconfig");

        if (ai->ai_flags & AI_ALL)
            printf(" all");

        if (ai->ai_flags & AI_CANONNAME)
            printf(" cannonical");

        if (ai->ai_flags & AI_NUMERICHOST)
            printf(" numeric host");

        if (ai->ai_flags & AI_NUMERICSERV)
            printf(" numeric server");

        if (ai->ai_flags & AI_PASSIVE)
            printf(" passive");

        if (ai->ai_flags & AI_V4MAPPED)
            printf(" v4 mapped");
    }
}

static void
print_family(struct addrinfo *ai)
{
    printf(" family");
    switch (ai->ai_family) {
    case AF_INET:
        printf(" inet");
        break;
    case AF_INET6:
        printf(" inet6");
        break;
    case AF_UNIX:
        printf(" unix");
        break;
    case AF_UNSPEC:
        printf(" unspec");
        break;
    default:
        printf(" unknown");
    }
}

static void
print_socktype(struct addrinfo *ai)
{
    printf(" socket type");
    switch (ai->ai_socktype) {
    case SOCK_DGRAM:
        printf(" DGRAM");
        break;
    case SOCK_RAW:
        printf(" RAW");
        break;
    case SOCK_SEQPACKET:
        printf(" SEQPACKET");
        break;
    case SOCK_STREAM:
        printf(" STREAM");
        break;
    default:
        printf(" UNKNOWN");
    }
}

static void
print_protocol(struct addrinfo *ai)
{
    printf(" protocol");
    switch (ai->ai_protocol) {
    case IPPROTO_IP:
        printf(" ip");
        break;
    case IPPROTO_IPV6:
        printf(" ipv6");
        break;
    case IPPROTO_ICMP:
        printf(" icmp");
        break;
    case IPPROTO_RAW:
        printf(" raw");
        break;
    case IPPROTO_TCP:
        printf(" tcp");
        break;
    case IPPROTO_UDP:
        printf(" udp");
        break;
    default:
        printf(" unknown");
    }
}

