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
#include "tcp_byte_order.h"

size_t readn(int fd, void *buffer, size_t length) {
    char *ptr = buffer;
    size_t count = length;
    ssize_t read_num;

    while (count > 0) {
        read_num = read(fd, ptr, count);
        if (read_num == 0) {
            // End-of-File，表示 socket 关闭
            break;
        } else if (read_num < 0) {
            if (EINTR == errno) {
                // 非阻塞情况，继续 read
                continue;
            }
            error_logging(stderr, "read from socket error");
            return -1;
        }
        count -= read_num;
        ptr += read_num;
    }
    return (length - count);
}

void read_data(int sockfd, int byte_order_flag) {
    struct message msg;
    bzero(&msg, sizeof(msg));

    size_t read_len_expect = sizeof(msg.len);
    size_t read_len_actual = readn(sockfd, &msg.len, read_len_expect);
    if (read_len_actual != read_len_expect) {
        error_logging(stderr, "read msg.len error");
        return;
    }

    // 字节序处理
    if (byte_order_flag) {
        msg.len = ntohl(msg.len);
    }
    printf("msg.len: %d\n", msg.len);

    read_len_actual = readn(sockfd, &msg.buf, msg.len);
    if (read_len_actual != msg.len) {
        error_logging(stderr, "read msg.buf error");
        return;
    }

    display_message(&msg);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        error_handling(stderr, "Usage: socket_server <Port> <ByteOrderFlag>");
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

    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling(stderr, "bind error");
    }

    if (listen(listen_fd, 1024) == -1) {
        error_handling(stderr, "listen error");
    }

    socklen_t client_len;
    int clientfd;
    for (int i = 0; i < 10; i++) {
        client_len = sizeof(client_addr);
        clientfd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
        read_data(clientfd, atoi(argv[2]));
        close(clientfd);
    }
    close(listen_fd);

    return 0;
}
