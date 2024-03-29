//
// Created by DemonLee on 8/7/22.
//

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "common/log.h"

# define MESSAGE_SIZE 20000000

void set_socket(int sock_fd, int linger_flag);

void display_socket_opt(int sock_fd);

void connect_server(char *const *argv, int sock_fd);

void send_msg(int sock_fd);

int main(int argc, char **argv) {
    if (argc < 3) {
        error_handling(stderr, "Usage: sockopt_client <IP> <Port> <linger_flag>");
    }

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    display_socket_opt(sock_fd);

    int linger_flag = (int) strtol(argv[3], (char **) NULL, 10);
    if (errno != 0) {
        error_handling(stderr, "convert linger_flag failed");
    }
    set_socket(sock_fd, linger_flag);
    display_socket_opt(sock_fd);

    connect_server(argv, sock_fd);
    send_msg(sock_fd);

    logging("close socket now");
    close(sock_fd);
    logging("close socket end...");

    return 0;
}

void send_msg(int sock_fd) {
    char *msg = malloc(MESSAGE_SIZE + 1);
    bzero(msg, MESSAGE_SIZE + 1);
    for (long i = 0; i < MESSAGE_SIZE; ++i) {
        msg[i] = '1';
    }
    logging("write data now");
    ssize_t write_len = send(sock_fd, msg, MESSAGE_SIZE + 1, 0);
    free(msg);

    char msg2[100];
    bzero(msg2, sizeof(msg2));
    sprintf(msg2, "write data end: %lu", write_len);
    logging(msg2);
}

void connect_server(char *const *argv, int sock_fd) {
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    //serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    int connect_rt = connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (connect_rt < 0) {
        error_handling(stderr, "connect failed");
    }
}

void set_socket(int sock_fd, int linger_flag) {
    if (!linger_flag) {
        return;
    }
    struct linger so_linger;
    so_linger.l_onoff = 1;
    if (linger_flag > 1) {
        so_linger.l_linger = 1;
    } else {
        so_linger.l_linger = 0;
    }
    int ret = setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
    if (ret < 0) {
        error_handling(stderr, "setsockopt failed");
    }
    printf("setsockopt success...\n");
}

void display_socket_opt(int sock_fd) {
    struct linger so_linger;
    bzero(&so_linger, sizeof(so_linger));
    socklen_t len = sizeof(so_linger);
    int ret = getsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &so_linger, &len);
    if (ret < 0) {
        error_handling(stderr, "getsockopt failed");
    }
    printf("getsockopt linger.l_onoff: %d, l_linger: %d\n", so_linger.l_onoff, so_linger.l_linger);
}
