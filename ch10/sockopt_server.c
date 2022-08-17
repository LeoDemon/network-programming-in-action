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

#define READ_SIZE 1024

void bind_and_listen(char *const *argv, int listen_fd);

void read_data(int sock_fd, int flag) {
    if (flag) {
        printf("input> ");
        getchar();
    }

    long total = 0;
    ssize_t n;
    char buf[READ_SIZE];
    for (;;) {
        bzero(buf, sizeof(buf));
        n = read(sock_fd, buf, READ_SIZE);
        if (n == 0) {
            // End-of-File，表示 socket 关闭
            logging("client socket closed");
            break;
        } else if (n < 0) {
            if (EINTR == errno) {
                // 非阻塞情况，继续 read
                continue;
            }
            error_logging(stderr, "read from socket error");
            break;
        }
        total += n;
    }
    char msg[100];
    bzero(msg, sizeof(msg));
    sprintf(msg, "read data size: [%ld]", total);
    logging(msg);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error_handling(stderr, "Usage: sockopt_server <Port>");
    }
    int flag = 0;
    if (3 == argc) {
        flag = 1;
    }

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind_and_listen(argv, listen_fd);

    struct sockaddr_in client_addr;
    socklen_t client_len;
    int client_fd;
    for (;;) {
        client_len = sizeof(client_addr);
        client_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
        read_data(client_fd, flag);
        close(client_fd);
    }

    close(listen_fd);
    return 0;
}

void bind_and_listen(char *const *argv, int listen_fd) {
    struct sockaddr_in server_addr;
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
}
