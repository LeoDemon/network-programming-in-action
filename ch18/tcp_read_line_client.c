//
// Created by DemonLee on 10/13/22.
//

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common/log.h"

void send_data(int socket_fd) {
    char msg[512];
    if (fgets(msg, sizeof(msg) - 1, stdin) != NULL) {
        size_t send_len = strlen(msg);
        ssize_t n_written = send(socket_fd, &msg, send_len, 0);
        fprintf(stdout, "send into buffer: %ld\n", n_written);
        if (n_written != send_len) {
            error_handling(stderr, "client send message failed");
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        error_handling(stderr, "Usage: socket_client <IP> <Port>");
    }

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    //serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        error_handling(stderr, "create socket failed");
    }

    int connect_rt = connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (connect_rt < 0) {
        error_handling(stderr, "connect to server failed");
    }
    send_data(socket_fd);
    close(socket_fd);

    return 0;
}
