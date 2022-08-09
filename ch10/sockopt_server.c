//
// Created by DemonLee on 8/7/22.
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

    for (;;) {
        bzero(buf, sizeof(buf));
        n = read(sockfd, buf, 1024);
        if (n == 0) {
            // End-of-File，表示 socket 关闭
            printf("client socket closed\n");
            return;
        } else if (n < 0) {
            if (EINTR == errno) {
                // 非阻塞情况，继续 read
                continue;
            }
            error_logging(stderr, "read from socket error");
            return;
        }
        if ('\n' == buf[n - 1]) {
            buf[n-1] = '\0';
        }
        printf("read data: [%s]\n", buf);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error_handling(stderr, "Usage: sockopt_server <Port>");
    }
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_addr, server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling(stderr, "bind error");
    }
    if (listen(listen_fd, 1024) == -1) {
        error_handling(stderr, "listen error");
    }

    socklen_t client_len;
    int client_fd;
    for (;;) {
        client_len = sizeof(client_addr);
        client_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
        read_data(client_fd);
        close(client_fd);
    }

    close(listen_fd);
    return 0;
}
