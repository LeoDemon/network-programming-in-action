//
// Created by DemonLee on 10/13/22..
//

#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include "common/log.h"

size_t readline(int fd, char *buffer, size_t length) {
    bzero(buffer, length);
    char *buf_first = buffer;
    static char *buffer_pointer;
    ssize_t n_left = 0;
    static char read_buffer[512];
    char c;

    bzero(read_buffer, sizeof(read_buffer));
    // input: abcefg\n
    while (--length > 0) { //output: abcefg
    //while (length-- > 0) { //output: abcefg\n  --> char array overflow
        if (n_left <= 0) {
            ssize_t n_read = recv(fd, read_buffer, sizeof(read_buffer) - 1, 0);
            printf("n_read: [%ld], [%s]\n", n_read, read_buffer);
            if (n_read < 0) {
                if (errno == EINTR) {
                    length++;
                    continue;
                }
                return -1;
            }
            if (n_read == 0) {
                return 0;
            }
            buffer_pointer = read_buffer;
            n_left = n_read;
        }
        c = *buffer_pointer++;
        *buffer++ = c;
        n_left--;
        printf("length: [%ld], n_left: [%ld], c: [%c]\n", length, n_left, c);
        if (c == '\n') {
            *buffer = '\0';
            return buffer - buf_first;
        }
    }
    return -1;
}

void read_data(int sock_fd) {
    char msg[7];
    size_t read_len = readline(sock_fd, msg, sizeof(msg));
    if (read_len < 0) {
        error_logging(stderr, "read msg error");
        return;
    }
    printf("msg: [%s]\n", msg);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error_handling(stderr, "Usage: socket_server <Port>");
    }
    struct sockaddr_in client_addr, server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        error_handling(stderr, "create socket error");
    }
    int reuse = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        error_handling(stderr, "set socket reuse addr error");
    }

    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling(stderr, "bind error");
    }

    if (listen(listen_fd, 1024) == -1) {
        error_handling(stderr, "listen error");
    }

    socklen_t client_len;
    int client_fd;
    for (int i = 0; i < 10; i++) {
        client_len = sizeof(client_addr);
        client_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
        read_data(client_fd);
        close(client_fd);
    }
    close(listen_fd);

    return 0;
}
