//
// Created by DemonLee on 5/25/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "common/log.h"

static int count;
#define MAX_LINE 4096

static void recvfrom_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error_handling(stderr, "Usage: udp_server <Port>");
    }

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(socket_fd));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
        error_handling(stderr, "bind error");
    }

    // Note: no listen function here

    signal(SIGINT, recvfrom_int);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char message[MAX_LINE];
    char send_line[MAX_LINE];

    for (;;) {
        ssize_t n = recvfrom(socket_fd, message, MAX_LINE, 0, (struct sockaddr *) &client_addr, &client_len);
        message[n] = '\0';
        printf("server received %zd bytes: %s\n", n, message);

        bzero(send_line, sizeof(send_line));
        sprintf(send_line, "Okay, we received your message: [%s]", message);
        printf("sending message: %s\n", send_line);

        ssize_t rt = sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr *) &client_addr, client_len);
        if (rt < 0) {
            error_logging(stderr, "server sendto failed");
        }

        count++;

        if (strcmp("bye", message) == 0) {
            break;
        }
    }
    close(socket_fd);
}