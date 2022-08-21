//
// Created by DemonLee on 8/5/22.
//

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include "common/log.h"
#include "common/general.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        error_handling(stderr, "Usage: shutdown_client <IP> <Port>");
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0) {
        error_handling(stderr, "client socket connect failed");
    }

    char send_line[MAX_LINE], recv_line[MAX_LINE + 1];
    ssize_t n;

    fd_set read_mask;
    fd_set all_reads;

    FD_ZERO(&all_reads);
    FD_SET(0, &all_reads);
    FD_SET(socket_fd, &all_reads);

    for (;;) {
        read_mask = all_reads;
        int rc = select(socket_fd + 1, &read_mask, NULL, NULL, NULL);
        if (rc < 0) {
            error_logging(stderr, "select failed");
            break;
        }

        if (FD_ISSET(socket_fd, &read_mask)) {
            n = read(socket_fd, recv_line, MAX_LINE);
            if (n < 0) {
                error_logging(stderr, "read data error");
                break;
            } else if (n == 0) {
                error_logging(stderr, "server terminated");
                break;
            }
            recv_line[n] = '\0';
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(0, &read_mask)) {
            if (fgets(send_line, MAX_LINE, stdin) == NULL) {
                continue;
            }
            if (strncmp(send_line, "shutdown", 8) == 0) {
                FD_CLR(0, &all_reads);
                error_logging(stdout, "shutdown socket_fd now");
                if (shutdown(socket_fd, SHUT_WR)) {
                    error_logging(stderr, "shutdown failed");
                    break;
                }
            } else if (strncmp(send_line, "close", 5) == 0) {
                FD_CLR(0, &all_reads);
                if (close(socket_fd)) {
                    error_handling(stderr, "close failed");
                }
                sleep(6);
                exit(0);
            } else {
                size_t i = strlen(send_line);
                if (send_line[i - 1] == '\n') {
                    send_line[i - 1] = '\0';
                }
                printf("now sending: [%s]\n", send_line);
                ssize_t rt = write(socket_fd, send_line, strlen(send_line));
                if (rt < 0) {
                    error_logging(stderr, "write data error");
                    break;
                }
                printf("send bytes: %zu\n", rt);
            }
        }
    }
    error_logging(stdout, "close socket_fd now");
    close(socket_fd);

    return 0;
}
