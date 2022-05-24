//
// Created by DemonLee on 5/22/22.
//

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common/log.h"

//# define MESSAGE_SIZE 1024000
# define MESSAGE_SIZE 10240000

void send_data(int sockfd) {
    char *query = malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; ++i) {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp = query;
    size_t remaining = strlen(query);
    while (remaining) {
        ssize_t n_written = send(sockfd, cp, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written < 0) {
            error_handling(stderr, "send failed");
            break;
        }
        remaining -= n_written;
        cp += +n_written;
    }
    free(query);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error_handling(stderr, "Usage: socket_client <IP> <Port>");
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    //serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    int connect_rt = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (connect_rt < 0) {
        error_handling(stderr, "connect failed");
    }
    send_data(sockfd);
    close(sockfd);

    return 0;
}
