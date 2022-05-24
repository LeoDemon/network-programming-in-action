//
// Created by DemonLee on 5/22/22.
//

#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include "common/log.h"

void read_data(int sockfd) {
    ssize_t n;
    char buf[1024];

    int time = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        n = read(sockfd, buf, 1024);
        if (n == 0) {
            // End-of-File，表示 socket 关闭
            return;
        } else if (n < 0) {
            if (EINTR == errno) {
                // 非阻塞情况，继续 read
                continue;
            }
            error_logging(stderr, "read from socket error");
            return;
        }
        time++;
        fprintf(stdout, "1K read for %d, len=[%zd]\n", time, n);
        usleep(1000);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error_handling(stderr, "Usage: socket_server <Port>");
    }
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_addr, server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling(stderr, "bind error");
    }
    if (listen(listenfd, 1024) == -1) {
        error_handling(stderr, "listen error");
    }

    socklen_t client_len;
    int clientfd;
    for (;;) {
        client_len = sizeof(client_addr);
        clientfd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len);
        read_data(clientfd);
        close(clientfd);
    }
    // close(listenfd);
}
